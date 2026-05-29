#include "jogador.h"
#include <stddef.h>
#include "assets.h"
#include "config.h"
#include "raylib.h"

#define LARGURA_SPRITE_JOGADOR 60.0f
#define ALTURA_SPRITE_JOGADOR 90.0f
#define VELOCIDADE_TROCA_FAIXA 993.6f
#define ESCALA_CAIXA_COLISAO_JOGADOR 0.64f

static Texture2D texturaJogador = { 0 };

static Rectangle ObterRetanguloVisualJogador(const Jogador *jogador)
{
    if (jogador == NULL) {
        return (Rectangle){ 0 };
    }

    return (Rectangle){
        jogador->posicaoX,
        jogador->posicaoY,
        jogador->largura,
        jogador->altura
    };
}

/* Eu monto o retangulo visual a partir da posicao e tamanho, pra passar pro Draw da raylib. */

static void AtualizarCaixaColisaoJogador(Jogador *jogador)
{
    float larguraColisao = 0.0f;
    float alturaColisao = 0.0f;

    if (jogador == NULL) {
        return;
    }

    larguraColisao = jogador->largura * ESCALA_CAIXA_COLISAO_JOGADOR;
    alturaColisao = jogador->altura * ESCALA_CAIXA_COLISAO_JOGADOR;
    jogador->caixaColisao = (Rectangle){
        jogador->posicaoX + ((jogador->largura - larguraColisao) / 2.0f),
        jogador->posicaoY + ((jogador->altura - alturaColisao) / 2.0f),
        larguraColisao,
        alturaColisao
    };
}

/* Eu encolho a caixa de colisao (escala < 1) e centralizo dentro do sprite pra batida nao parecer injusta. */

static float MoverValorAte(float valorAtual, float valorDestino, float passoMaximo)
{
    float diferenca = valorDestino - valorAtual;

    if (diferenca > -passoMaximo && diferenca < passoMaximo) {
        return valorDestino;
    }

    return valorAtual + (diferenca > 0.0f ? passoMaximo : -passoMaximo);
}

/* Eu ando ate o destino em passos limitados; se ja ta dentro do passo, grudo no destino. */

float CalcularCentroFaixa(int faixa)
{
    float larguraPista = LARGURA_PISTA_VISUAL;
    float margemEsquerda = (LARGURA_JANELA - larguraPista) / 2.0f;
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;

    return margemEsquerda + (larguraFaixa * faixa) + (larguraFaixa / 2.0f);
}

/* Eu divido a largura da pista pelas faixa e devolvo o centro X da faixa pedida. */

void CarregarTexturaJogador(void)
{
    if (IsTextureValid(texturaJogador)) {
        return;
    }

    texturaJogador = LoadTexture(ASSETS_CAMINHO_CARRO_JOGADOR);
}

/* Eu so carrego se ainda nao ta valida, pra nao recarregar imagem toda vez. */

void LiberarTexturaJogador(void)
{
    if (IsTextureValid(texturaJogador)) {
        UnloadTexture(texturaJogador);
        texturaJogador = (Texture2D){ 0 };
    }
}

/* Eu solto a textura quando ela ta valida e zero a variavel pra nao restar referencia antiga. */

void InicializarJogador(Jogador *jogador)
{
    if (jogador == NULL) {
        return;
    }

    jogador->faixaAtual = QUANTIDADE_FAIXAS / 2;
    jogador->largura = LARGURA_SPRITE_JOGADOR;
    jogador->altura = ALTURA_SPRITE_JOGADOR;
    jogador->velocidadeLateral = VELOCIDADE_TROCA_FAIXA;
    jogador->posicaoX = CalcularCentroFaixa(jogador->faixaAtual) - (jogador->largura / 2.0f);
    jogador->posicaoY = ALTURA_JANELA - 130.0f;
    AtualizarCaixaColisaoJogador(jogador);
}

/* Eu coloco o jogador na faixa do meio com tamanho e velocidade definido, e ja ajeito a colisao. */

void AtualizarJogador(Jogador *jogador, float delta)
{
    float destinoX = 0.0f;
    float passoMaximo = 0.0f;

    if (jogador == NULL) {
        return;
    }

    // As setas escolhem a faixa-alvo; a posicao horizontal chega nela aos poucos.
    if (IsKeyPressed(KEY_LEFT) && jogador->faixaAtual > 0) {
        jogador->faixaAtual--;
    }

    if (IsKeyPressed(KEY_RIGHT) && jogador->faixaAtual < QUANTIDADE_FAIXAS - 1) {
        jogador->faixaAtual++;
    }

    destinoX = CalcularCentroFaixa(jogador->faixaAtual) - (jogador->largura / 2.0f);
    passoMaximo = jogador->velocidadeLateral * delta;
    jogador->posicaoX = MoverValorAte(jogador->posicaoX, destinoX, passoMaximo);
    AtualizarCaixaColisaoJogador(jogador);
}

/* Eu uso as setas pra trocar de faixa alvo, e movo o X aos poucos pra dar deslize suave. */

void DesenharJogador(const Jogador *jogador)
{
    Rectangle retanguloVisual = { 0 };

    if (jogador == NULL) {
        return;
    }

    retanguloVisual = ObterRetanguloVisualJogador(jogador);

    if (IsTextureValid(texturaJogador)) {
        Rectangle origem = { 0.0f, 0.0f, (float)texturaJogador.width, (float)texturaJogador.height };
        DrawTexturePro(texturaJogador, origem, retanguloVisual, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        return;
    }

    DrawRectangleRounded(retanguloVisual, 0.18f, 6, (Color){ 40, 130, 220, 255 });
    DrawRectangleLinesEx(retanguloVisual, 2.0f, RAYWHITE);
    DrawText("CARRO", (int)jogador->posicaoX + 2, (int)jogador->posicaoY + 34, 14, RAYWHITE);
}

/* Eu uso o sprite quando ele esta valido; sem ele, desenho um retangulo de reserva escrito CARRO. */
