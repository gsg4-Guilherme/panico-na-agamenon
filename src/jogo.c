#include "jogo.h"

#include <string.h>
#include <time.h>

#include "config.h"
#include "interface.h"
#include "jogador.h"
#include "obstaculos.h"
#include "pista.h"
#include "pontuacao.h"
#include "powerups.h"
#include "raylib.h"
#include "ranking.h"

static const float MULTIPLICADOR_INTERVALO_ENGARRAFAMENTO = 0.65f;
static const float MULTIPLICADOR_VELOCIDADE_CHUVA = 1.25f;
static const float MULTIPLICADOR_VELOCIDADE_FREIO = 0.45f;
static const float INTERVALO_MINIMO_NORMAL = 0.32f;
static const float INTERVALO_MINIMO_ENGARRAFAMENTO = 0.30f;
static const float DISTANCIA_MINIMA_OBSTACULO_MESMA_FAIXA = 240.0f;
static const float LIMITE_SUPERIOR_BANDA_SPAWN = -140.0f;
static const float LIMITE_INFERIOR_BANDA_SPAWN = 220.0f;

static float ObterMultiplicadorVelocidadeChuva(const EstadoJogo *jogo)
{
    return (jogo != NULL && jogo->chuvaAtiva) ? MULTIPLICADOR_VELOCIDADE_CHUVA : 1.0f;
}

static float ObterMultiplicadorVelocidadeFreio(const EstadoJogo *jogo)
{
    return (jogo != NULL && jogo->tempoFreio > 0.0f) ? MULTIPLICADOR_VELOCIDADE_FREIO : 1.0f;
}

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

static TipoObstaculo SortearTipoObstaculo(const EstadoJogo *jogo)
{
    int chanceOnibus = (jogo != NULL && jogo->engarrafamentoAtivo) ? 50 : 30;

    if (GetRandomValue(1, 100) <= chanceOnibus) {
        return OBSTACULO_ONIBUS;
    }

    return OBSTACULO_CARRO;
}

static bool DeveGerarObstaculoExtra(const EstadoJogo *jogo)
{
    int chanceObstaculoExtra = (jogo != NULL && jogo->engarrafamentoAtivo) ? 30 : 20;

    return GetRandomValue(1, 100) <= chanceObstaculoExtra;
}

static bool ObstaculoEstaNaBandaSpawn(const Obstaculo *obstaculo)
{
    return obstaculo != NULL &&
        obstaculo->posicaoY >= LIMITE_SUPERIOR_BANDA_SPAWN &&
        obstaculo->posicaoY <= LIMITE_INFERIOR_BANDA_SPAWN;
}

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

static bool GerarObstaculoNaFaixa(EstadoJogo *jogo, int faixa)
{
    TipoObstaculo tipo = SortearTipoObstaculo(jogo);
    float aumentoPorTempo = jogo->tempoSobrevivencia * 5.0f;
    float variacaoVelocidade = (float)GetRandomValue(-20, 35);

    if (aumentoPorTempo > 220.0f) {
        aumentoPorTempo = 220.0f;
    }

    return AdicionarObstaculo(&jogo->obstaculos, faixa, jogo->velocidadeBase + aumentoPorTempo + variacaoVelocidade, tipo);
}

static void GerarObstaculosAleatorios(EstadoJogo *jogo)
{
    bool faixasReservadas[QUANTIDADE_FAIXAS] = { false };
    int faixaPrincipal = 0;
    int faixaExtra = 0;

    if (jogo->tempoInterdicaoFaixa > 0.0f &&
        jogo->faixaInterditada >= 0 &&
        jogo->faixaInterditada < QUANTIDADE_FAIXAS) {
        faixasReservadas[jogo->faixaInterditada] = true;
    }

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

static void AtualizarGeracaoObstaculos(EstadoJogo *jogo, float delta)
{
    jogo->tempoGerarObstaculo += delta;

    if (jogo->tempoGerarObstaculo >= jogo->intervaloObstaculo) {
        GerarObstaculosAleatorios(jogo);
        jogo->tempoGerarObstaculo = 0.0f;
        jogo->intervaloObstaculo = CalcularProximoIntervaloObstaculo(jogo);
    }
}

static void AtualizarDeslocamentoCenario(EstadoJogo *jogo, float delta)
{
    float aumentoPorTempo = jogo->tempoSobrevivencia * 5.0f;

    if (aumentoPorTempo > 220.0f) {
        aumentoPorTempo = 220.0f;
    }

    jogo->deslocamentoCenario += (jogo->velocidadeBase + aumentoPorTempo) *
        ObterMultiplicadorVelocidadeChuva(jogo) * delta;
}

void InicializarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    SetRandomSeed((unsigned int)time(NULL));
    memset(jogo, 0, sizeof(*jogo));
    CarregarTexturaJogador();
    CarregarTexturasObstaculos();
    CarregarTexturasPowerUps();
    CarregarTexturaPista();
    InicializarListaObstaculos(&jogo->obstaculos);
    ReiniciarJogo(jogo);
}

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
    InicializarPowerUps(jogo);

    jogo->tempoGerarObstaculo = 0.0f;
    jogo->intervaloObstaculo = 1.0f;
    jogo->velocidadeBase = 190.0f;
    jogo->deslocamentoCenario = 0.0f;
    jogo->jogoAtivo = true;
}

void AtualizarJogo(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->jogoAtivo) {
        return;
    }

    AtualizarJogador(&jogo->jogador, delta);
    AtualizarPowerUps(jogo, delta);
    AtualizarPontuacao(jogo, delta);
    AtualizarDeslocamentoCenario(jogo, delta);
    AtualizarGeracaoObstaculos(jogo, delta);

    AtualizarObstaculos(
        &jogo->obstaculos,
        delta * ObterMultiplicadorVelocidadeChuva(jogo) * ObterMultiplicadorVelocidadeFreio(jogo)
    );
    AtualizarPista(jogo->pistaLogica, &jogo->jogador, &jogo->obstaculos);

    if (VerificarColisaoJogadorObstaculos(&jogo->jogador, &jogo->obstaculos)) {
        if (jogo->escudoAtivo) {
            RemoverPrimeiroObstaculoColidindo(&jogo->obstaculos, &jogo->jogador);
            jogo->escudoAtivo = false;
            AtualizarPista(jogo->pistaLogica, &jogo->jogador, &jogo->obstaculos);
            return;
        }

        jogo->jogoAtivo = false;
        SalvarPontuacaoSeRecorde(CAMINHO_RANKING, jogo->pontuacaoAtual);
    }
}

void DesenharJogo(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    DesenharPista(jogo->pistaLogica, jogo->deslocamentoCenario);
    DesenharZonaInterditada(jogo);
    DesenharObstaculos(&jogo->obstaculos);
    DesenharPowerUpColetavel(jogo);
    DesenharJogador(&jogo->jogador);
    DesenharEscudoJogador(jogo);
    DesenharEventos(jogo);
    DesenharHud(jogo);
}

void FinalizarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    LiberarObstaculos(&jogo->obstaculos);
    LiberarTexturasObstaculos();
    LiberarTexturasPowerUps();
    LiberarTexturaPista();
    LiberarTexturaJogador();
}
