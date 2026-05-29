#include "jogo.h"

#include <string.h>
#include <time.h>

#include "config.h"
#include "interface.h"
#include "jogador.h"
#include "obstaculos.h"
#include "pista.h"
#include "pontuacao.h"
#include "raylib.h"
#include "ranking.h"

#define MULTIPLICADOR_INTERVALO_ENGARRAFAMENTO 0.65f
#define MULTIPLICADOR_VELOCIDADE_CHUVA 1.25f
#define INTERVALO_MINIMO_NORMAL 0.32f
#define INTERVALO_MINIMO_ENGARRAFAMENTO 0.30f
#define DISTANCIA_MINIMA_OBSTACULO_MESMA_FAIXA 240.0f
#define LIMITE_SUPERIOR_BANDA_SPAWN (-140.0f)
#define LIMITE_INFERIOR_BANDA_SPAWN 220.0f

static bool sementeAleatoriaConfigurada = false;

static float ObterMultiplicadorVelocidadeChuva(const EstadoJogo *jogo)
{
    return (jogo != NULL && jogo->chuvaAtiva) ? MULTIPLICADOR_VELOCIDADE_CHUVA : 1.0f;
}

/* Eu devolvo multiplicador maior quando a chuva esta ativa, senao deixa em 1.0 (sem efeito). */

static float CalcularProximoIntervaloObstaculo(const EstadoJogo *jogo)
{
    float intervaloBase = 1.05f - (jogo->tempoSobrevivencia * 0.020f);
    float variacaoAleatoria = (float)GetRandomValue(0, 30) / 100.0f;
    float intervaloMinimo = INTERVALO_MINIMO_NORMAL;

    if (intervaloBase < INTERVALO_MINIMO_NORMAL) {
        intervaloBase = INTERVALO_MINIMO_NORMAL;
    }

    float intervalo = intervaloBase + variacaoAleatoria;

    if (jogo->engarrafamentoAtivo) {
        intervalo *= MULTIPLICADOR_INTERVALO_ENGARRAFAMENTO;
        intervaloMinimo = INTERVALO_MINIMO_ENGARRAFAMENTO;
    }

    if (intervalo < intervaloMinimo) {
        intervalo = intervaloMinimo;
    }

    return intervalo;
}

/* Eu encurto o intervalo conforme o tempo passa e somo variacao aleatoria pra nao ficar igual. */

static TipoObstaculo SortearTipoObstaculo(const EstadoJogo *jogo)
{
    int chanceOnibus = (jogo != NULL && jogo->engarrafamentoAtivo) ? 50 : 30;

    if (GetRandomValue(1, 100) <= chanceOnibus) {
        return OBSTACULO_ONIBUS;
    }

    return OBSTACULO_CARRO;
}

/* Eu sorteio carro ou onibus; no engarrafamento a chance de onibus sobe (50% vs 30%). */

float CalcularAumentoVelocidadePorTempo(float tempoSobrevivencia)
{
    float aumento = tempoSobrevivencia * 5.0f;

    if (aumento > 220.0f) {
        aumento = 220.0f;
    }

    return aumento;
}

/* Eu fiz essa conta uma vez so, ai obstaculo e cenario usam a mesma regra de aceleracao. */

static bool DeveGerarObstaculoExtra(const EstadoJogo *jogo)
{
    int chanceObstaculoExtra = (jogo != NULL && jogo->engarrafamentoAtivo) ? 30 : 20;

    return GetRandomValue(1, 100) <= chanceObstaculoExtra;
}

/* Eu decido se spawno um segundo obstaculo no mesmo ciclo; chance maior no engarrafamento. */

static bool ObstaculoEstaNaBandaSpawn(const Obstaculo *obstaculo)
{
    return obstaculo != NULL &&
        obstaculo->posicaoY >= LIMITE_SUPERIOR_BANDA_SPAWN &&
        obstaculo->posicaoY <= LIMITE_INFERIOR_BANDA_SPAWN;
}

/* Eu checo se o obstaculo esta na zona de spawn, usado pra nao bloquear todas as faixa de cima. */

static bool ExisteObstaculoProximoNaFaixa(const ListaObstaculos *lista, int faixa)
{
    if (lista == NULL) {
        return false;
    }

    for (const Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        if (obstaculo->faixa == faixa &&
            obstaculo->posicaoY <= DISTANCIA_MINIMA_OBSTACULO_MESMA_FAIXA) {
            return true;
        }
    }

    return false;
}

/* Eu evito spawnar dois obstaculos colados na mesma faixa checando distancia minima. */

static int ContarFaixasBloqueadasNaBandaSpawn(
    const ListaObstaculos *lista,
    const bool faixasReservadas[QUANTIDADE_FAIXAS]
)
{
    bool faixasBloqueadas[QUANTIDADE_FAIXAS] = { false };
    int quantidadeBloqueadas = 0;

    for (int faixa = 0; faixa < QUANTIDADE_FAIXAS; faixa++) {
        if (faixasReservadas != NULL && faixasReservadas[faixa]) {
            faixasBloqueadas[faixa] = true;
        }
    }

    if (lista != NULL) {
        for (const Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
            if (obstaculo->faixa >= 0 &&
                obstaculo->faixa < QUANTIDADE_FAIXAS &&
                ObstaculoEstaNaBandaSpawn(obstaculo)) {
                faixasBloqueadas[obstaculo->faixa] = true;
            }
        }
    }

    for (int faixa = 0; faixa < QUANTIDADE_FAIXAS; faixa++) {
        if (faixasBloqueadas[faixa]) {
            quantidadeBloqueadas++;
        }
    }

    return quantidadeBloqueadas;
}

/* Eu conto quantas faixa estao bloqueadas na banda de spawn, somando reservadas + ocupadas. */

static bool FaixaPodeReceberObstaculo(
    const ListaObstaculos *lista,
    int faixa,
    const bool faixasReservadas[QUANTIDADE_FAIXAS]
)
{
    bool faixasComCandidata[QUANTIDADE_FAIXAS] = { false };

    if (faixa < 0 || faixa >= QUANTIDADE_FAIXAS) {
        return false;
    }

    if (faixasReservadas != NULL && faixasReservadas[faixa]) {
        return false;
    }

    if (ExisteObstaculoProximoNaFaixa(lista, faixa)) {
        return false;
    }

    if (faixasReservadas != NULL) {
        for (int i = 0; i < QUANTIDADE_FAIXAS; i++) {
            faixasComCandidata[i] = faixasReservadas[i];
        }
    }

    faixasComCandidata[faixa] = true;
    return ContarFaixasBloqueadasNaBandaSpawn(lista, faixasComCandidata) < QUANTIDADE_FAIXAS;
}

/* Eu valido a faixa olhando limite, reserva, vizinhanca e se ela nao fecharia todas as faixa. */

static bool SortearFaixaDisponivel(
    const ListaObstaculos *lista,
    const bool faixasReservadas[QUANTIDADE_FAIXAS],
    int *faixaEscolhida
)
{
    int faixasDisponiveis[QUANTIDADE_FAIXAS] = { 0 };
    int quantidadeDisponiveis = 0;

    if (faixaEscolhida == NULL) {
        return false;
    }

    for (int faixa = 0; faixa < QUANTIDADE_FAIXAS; faixa++) {
        if (FaixaPodeReceberObstaculo(lista, faixa, faixasReservadas)) {
            faixasDisponiveis[quantidadeDisponiveis] = faixa;
            quantidadeDisponiveis++;
        }
    }

    if (quantidadeDisponiveis == 0) {
        return false;
    }

    *faixaEscolhida = faixasDisponiveis[GetRandomValue(0, quantidadeDisponiveis - 1)];
    return true;
}

/* Eu listo so as faixa validas e sorteio uma delas, garantindo que o spawn vai dar certo. */

static bool GerarObstaculoNaFaixa(EstadoJogo *jogo, int faixa)
{
    TipoObstaculo tipo = SortearTipoObstaculo(jogo);
    float aumentoPorTempo = CalcularAumentoVelocidadePorTempo(jogo->tempoSobrevivencia);
    float variacaoVelocidade = (float)GetRandomValue(-20, 35);

    return AdicionarObstaculo(&jogo->obstaculos, faixa, jogo->velocidadeBase + aumentoPorTempo + variacaoVelocidade, tipo);
}

/* Eu sorteio tipo + variacao, somo o aumento por tempo e mando o obstaculo entrar na lista. */

static void GerarObstaculosAleatorios(EstadoJogo *jogo)
{
    bool faixasReservadas[QUANTIDADE_FAIXAS] = { false };
    int faixaPrincipal = 0;
    int faixaExtra = 0;

    if (!SortearFaixaDisponivel(&jogo->obstaculos, faixasReservadas, &faixaPrincipal)) {
        return;
    }

    if (!GerarObstaculoNaFaixa(jogo, faixaPrincipal)) {
        return;
    }

    faixasReservadas[faixaPrincipal] = true;

    if (DeveGerarObstaculoExtra(jogo) &&
        SortearFaixaDisponivel(&jogo->obstaculos, faixasReservadas, &faixaExtra)) {
        GerarObstaculoNaFaixa(jogo, faixaExtra);
    }
}

/* Eu spawno um obstaculo principal e, com alguma chance, mais um extra em outra faixa livre. */

static void AtualizarGeracaoObstaculos(EstadoJogo *jogo, float delta)
{
    jogo->tempoGerarObstaculo += delta;

    if (jogo->tempoGerarObstaculo >= jogo->intervaloObstaculo) {
        GerarObstaculosAleatorios(jogo);
        jogo->tempoGerarObstaculo = 0.0f;
        jogo->intervaloObstaculo = CalcularProximoIntervaloObstaculo(jogo);
    }
}

/* Eu acumulo delta no contador; quando bate o intervalo, spawno e sorteio o proximo. */

static void AtualizarDeslocamentoCenario(EstadoJogo *jogo, float delta)
{
    float aumentoPorTempo = CalcularAumentoVelocidadePorTempo(jogo->tempoSobrevivencia);

    jogo->deslocamentoCenario += (jogo->velocidadeBase + aumentoPorTempo) *
        ObterMultiplicadorVelocidadeChuva(jogo) * delta;
}

/* Eu rolo o cenario pra baixo aplicando velocidade base + aumento por tempo + bonus de chuva. */

void InicializarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    if (!sementeAleatoriaConfigurada) {
        SetRandomSeed((unsigned int)time(NULL));
        sementeAleatoriaConfigurada = true;
    }

    memset(jogo, 0, sizeof(*jogo));
    CarregarTexturaJogador();
    CarregarTexturasObstaculos();
    CarregarTexturaPista();
    InicializarListaObstaculos(&jogo->obstaculos);
    ReiniciarJogo(jogo);
}

/* Eu inicio o estado uma vez: semeio o random, carrego as textura e chamo reiniciar pra preparar. */

void ReiniciarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    LiberarObstaculos(&jogo->obstaculos);
    InicializarListaObstaculos(&jogo->obstaculos);
    InicializarJogador(&jogo->jogador);
    InicializarPista(jogo->pistaLogica);
    InicializarPontuacao(jogo);

    jogo->tempoGerarObstaculo = 0.0f;
    jogo->intervaloObstaculo = 1.0f;
    jogo->velocidadeBase = 190.0f;
    jogo->deslocamentoCenario = 0.0f;
    jogo->jogoAtivo = true;
}

/* Eu solto a lista de obstaculo e reseto jogador, pista, pontos e contadores. */

void AtualizarJogo(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->jogoAtivo) {
        return;
    }

    AtualizarJogador(&jogo->jogador, delta);
    AtualizarPontuacao(jogo, delta);
    AtualizarDeslocamentoCenario(jogo, delta);
    AtualizarGeracaoObstaculos(jogo, delta);

    AtualizarObstaculos(&jogo->obstaculos, delta * ObterMultiplicadorVelocidadeChuva(jogo));
    AtualizarPista(jogo->pistaLogica, &jogo->jogador, &jogo->obstaculos);

    if (VerificarColisaoJogadorObstaculos(&jogo->jogador, &jogo->obstaculos)) {
        jogo->jogoAtivo = false;
        SalvarPontuacaoSeRecorde(CAMINHO_RANKING, jogo->pontuacaoAtual);
    }
}

/* Eu atualizo nessa ordem: jogador, pontos, cenario, obstaculo, pista e por fim a colisao. */

void DesenharJogo(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    DesenharPista(jogo->pistaLogica, jogo->deslocamentoCenario);
    DesenharObstaculos(&jogo->obstaculos);
    DesenharJogador(&jogo->jogador);
    DesenharEventos(jogo);
    DesenharHud(jogo);
}

/* Eu desenho de baixo pra cima: pista, obstaculo, jogador, efeitos e o HUD. */

void FinalizarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    LiberarObstaculos(&jogo->obstaculos);
    LiberarTexturasObstaculos();
    LiberarTexturaPista();
    LiberarTexturaJogador();
}

/* Eu solto a lista de obstaculo e descarrego todas as textura antes de fechar o programa. */
