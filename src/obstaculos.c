#include "obstaculos.h"

#include <stddef.h>
#include <stdlib.h>

#include "assets.h"
#include "config.h"
#include "jogador.h"
#include "raylib.h"

static const Vector2 TAMANHOS_SPRITES_CARRO[ASSETS_QUANTIDADE_CARROS] = {
    { 56.0f, 84.0f },
    { 56.0f, 84.0f },
    { 56.0f, 84.0f }
};

static const Vector2 TAMANHOS_SPRITES_ONIBUS[ASSETS_QUANTIDADE_ONIBUS] = {
    { 72.0f, 136.0f },
    { 72.0f, 136.0f }
};

static Texture2D texturasCarro[ASSETS_QUANTIDADE_CARROS] = { 0 };
static Texture2D texturasOnibus[ASSETS_QUANTIDADE_ONIBUS] = { 0 };

static bool TipoObstaculoValido(TipoObstaculo tipo)
{
    return tipo == OBSTACULO_CARRO || tipo == OBSTACULO_ONIBUS;
}

static int ObterQuantidadeSprites(TipoObstaculo tipo)
{
    if (tipo == OBSTACULO_ONIBUS) {
        return ASSETS_QUANTIDADE_ONIBUS;
    }

    return ASSETS_QUANTIDADE_CARROS;
}

static int SortearIndiceSprite(TipoObstaculo tipo)
{
    return GetRandomValue(0, ObterQuantidadeSprites(tipo) - 1);
}

static Vector2 ObterTamanhoSprite(TipoObstaculo tipo, int indiceSprite)
{
    if (tipo == OBSTACULO_ONIBUS && indiceSprite >= 0 && indiceSprite < ASSETS_QUANTIDADE_ONIBUS) {
        return TAMANHOS_SPRITES_ONIBUS[indiceSprite];
    }

    if (indiceSprite >= 0 && indiceSprite < ASSETS_QUANTIDADE_CARROS) {
        return TAMANHOS_SPRITES_CARRO[indiceSprite];
    }

    return TAMANHOS_SPRITES_CARRO[0];
}

static Texture2D ObterTexturaObstaculo(const Obstaculo *obstaculo)
{
    if (obstaculo == NULL) {
        return (Texture2D){ 0 };
    }

    if (obstaculo->tipo == OBSTACULO_ONIBUS &&
        obstaculo->indiceSprite >= 0 &&
        obstaculo->indiceSprite < ASSETS_QUANTIDADE_ONIBUS) {
        return texturasOnibus[obstaculo->indiceSprite];
    }

    if (obstaculo->indiceSprite >= 0 && obstaculo->indiceSprite < ASSETS_QUANTIDADE_CARROS) {
        return texturasCarro[obstaculo->indiceSprite];
    }

    return (Texture2D){ 0 };
}

static void AtualizarCaixaObstaculo(Obstaculo *obstaculo)
{
    if (obstaculo == NULL) {
        return;
    }

    obstaculo->caixaColisao = (Rectangle){
        CalcularCentroFaixa(obstaculo->faixa) - (obstaculo->largura / 2.0f),
        obstaculo->posicaoY,
        obstaculo->largura,
        obstaculo->altura
    };
}

static bool ObstaculoAindaPodeSerLimpo(const Obstaculo *obstaculo)
{
    return obstaculo != NULL && obstaculo->posicaoY <= ALTURA_JANELA;
}

static void DesenharFallbackObstaculo(const Obstaculo *obstaculo)
{
    Color cor = (obstaculo->tipo == OBSTACULO_ONIBUS)
        ? (Color){ 220, 130, 35, 255 }
        : (Color){ 220, 80, 70, 255 };
    const char *texto = (obstaculo->tipo == OBSTACULO_ONIBUS) ? "BUS" : "CAR";

    DrawRectangleRounded(obstaculo->caixaColisao, 0.15f, 6, cor);
    DrawRectangleLinesEx(obstaculo->caixaColisao, 2.0f, RAYWHITE);
    DrawText(texto, (int)obstaculo->caixaColisao.x + 10, (int)obstaculo->caixaColisao.y + 34, 16, RAYWHITE);
}

static Obstaculo *CriarObstaculo(int faixa, float velocidade, TipoObstaculo tipo)
{
    Obstaculo *novoObstaculo = malloc(sizeof(*novoObstaculo));

    if (novoObstaculo == NULL) {
        return NULL;
    }

    if (!TipoObstaculoValido(tipo)) {
        tipo = OBSTACULO_CARRO;
    }

    int indiceSprite = SortearIndiceSprite(tipo);
    Vector2 tamanhoSprite = ObterTamanhoSprite(tipo, indiceSprite);

    novoObstaculo->faixa = faixa;
    novoObstaculo->tipo = tipo;
    novoObstaculo->indiceSprite = indiceSprite;
    novoObstaculo->posicaoY = -tamanhoSprite.y;
    novoObstaculo->velocidade = velocidade;
    novoObstaculo->largura = tamanhoSprite.x;
    novoObstaculo->altura = tamanhoSprite.y;
    novoObstaculo->proximo = NULL;
    AtualizarCaixaObstaculo(novoObstaculo);

    return novoObstaculo;
}

void InicializarListaObstaculos(ListaObstaculos *lista)
{
    if (lista == NULL) {
        return;
    }

    lista->inicio = NULL;
    lista->quantidade = 0;
}

void CarregarTexturasObstaculos(void)
{
    for (int i = 0; i < ASSETS_QUANTIDADE_CARROS; i++) {
        if (!IsTextureValid(texturasCarro[i])) {
            texturasCarro[i] = LoadTexture(ASSETS_CAMINHOS_CARROS[i]);
        }
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_ONIBUS; i++) {
        if (!IsTextureValid(texturasOnibus[i])) {
            texturasOnibus[i] = LoadTexture(ASSETS_CAMINHOS_ONIBUS[i]);
        }
    }
}

void LiberarTexturasObstaculos(void)
{
    for (int i = 0; i < ASSETS_QUANTIDADE_CARROS; i++) {
        if (IsTextureValid(texturasCarro[i])) {
            UnloadTexture(texturasCarro[i]);
            texturasCarro[i] = (Texture2D){ 0 };
        }
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_ONIBUS; i++) {
        if (IsTextureValid(texturasOnibus[i])) {
            UnloadTexture(texturasOnibus[i]);
            texturasOnibus[i] = (Texture2D){ 0 };
        }
    }
}

bool AdicionarObstaculo(ListaObstaculos *lista, int faixa, float velocidade, TipoObstaculo tipo)
{
    if (lista == NULL || faixa < 0 || faixa >= QUANTIDADE_FAIXAS) {
        return false;
    }

    Obstaculo *novoObstaculo = CriarObstaculo(faixa, velocidade, tipo);

    if (novoObstaculo == NULL) {
        return false;
    }

    // Inserção no início: simples, rápida e suficiente para uma lista encadeada de obstáculos.
    novoObstaculo->proximo = lista->inicio;
    lista->inicio = novoObstaculo;
    lista->quantidade++;
    return true;
}

void AtualizarObstaculos(ListaObstaculos *lista, float delta)
{
    if (lista == NULL) {
        return;
    }

    Obstaculo **ponteiroAtual = &lista->inicio;

    while (*ponteiroAtual != NULL) {
        Obstaculo *obstaculoAtual = *ponteiroAtual;
        obstaculoAtual->posicaoY += obstaculoAtual->velocidade * delta;
        AtualizarCaixaObstaculo(obstaculoAtual);

        if (obstaculoAtual->posicaoY > ALTURA_JANELA + 120.0f ||
            obstaculoAtual->posicaoY + obstaculoAtual->altura < -120.0f) {
            *ponteiroAtual = obstaculoAtual->proximo;
            free(obstaculoAtual);
            lista->quantidade--;
        } else {
            ponteiroAtual = &obstaculoAtual->proximo;
        }
    }
}

void DesenharObstaculos(const ListaObstaculos *lista)
{
    if (lista == NULL) {
        return;
    }

    for (const Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        Texture2D textura = ObterTexturaObstaculo(obstaculo);

        if (IsTextureValid(textura)) {
            Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
            DrawTexturePro(textura, origem, obstaculo->caixaColisao, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        } else {
            DesenharFallbackObstaculo(obstaculo);
        }
    }
}

bool VerificarColisaoJogadorObstaculos(const Jogador *jogador, const ListaObstaculos *lista)
{
    if (jogador == NULL || lista == NULL) {
        return false;
    }

    for (const Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        if (CheckCollisionRecs(jogador->caixaColisao, obstaculo->caixaColisao)) {
            return true;
        }
    }

    return false;
}

bool RemoverPrimeiroObstaculoColidindo(ListaObstaculos *lista, const Jogador *jogador)
{
    if (jogador == NULL || lista == NULL) {
        return false;
    }

    Obstaculo **ponteiroAtual = &lista->inicio;

    while (*ponteiroAtual != NULL) {
        Obstaculo *obstaculoAtual = *ponteiroAtual;

        if (CheckCollisionRecs(jogador->caixaColisao, obstaculoAtual->caixaColisao)) {
            *ponteiroAtual = obstaculoAtual->proximo;
            free(obstaculoAtual);
            lista->quantidade--;
            return true;
        }

        ponteiroAtual = &obstaculoAtual->proximo;
    }

    return false;
}

int RemoverObstaculosFaixa(ListaObstaculos *lista, int faixa)
{
    int removidos = 0;

    if (lista == NULL || faixa < 0 || faixa >= QUANTIDADE_FAIXAS) {
        return 0;
    }

    Obstaculo **ponteiroAtual = &lista->inicio;

    while (*ponteiroAtual != NULL) {
        Obstaculo *obstaculoAtual = *ponteiroAtual;

        if (obstaculoAtual->faixa == faixa) {
            *ponteiroAtual = obstaculoAtual->proximo;
            free(obstaculoAtual);
            lista->quantidade--;
            removidos++;
        } else {
            ponteiroAtual = &obstaculoAtual->proximo;
        }
    }

    return removidos;
}

void AplicarVelocidadeObstaculosFaixa(ListaObstaculos *lista, int faixa, float velocidade)
{
    if (lista == NULL || faixa < 0 || faixa >= QUANTIDADE_FAIXAS) {
        return;
    }

    for (Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        if (obstaculo->faixa == faixa && ObstaculoAindaPodeSerLimpo(obstaculo)) {
            obstaculo->velocidade = velocidade;
        }
    }
}

void LiberarObstaculos(ListaObstaculos *lista)
{
    if (lista == NULL) {
        return;
    }

    Obstaculo *obstaculo = lista->inicio;

    while (obstaculo != NULL) {
        Obstaculo *proximo = obstaculo->proximo;
        free(obstaculo);
        obstaculo = proximo;
    }

    InicializarListaObstaculos(lista);
}
