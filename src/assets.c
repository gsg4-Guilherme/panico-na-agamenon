#include "assets.h"

#include <stddef.h>

const char *ASSETS_CAMINHO_CARRO_JOGADOR = "assets/veiculos/carro_jogador.png";
const char *ASSETS_CAMINHO_PISTA = "assets/cenario/tela.png";

const char *ASSETS_CAMINHOS_CARROS[ASSETS_QUANTIDADE_CARROS] = {
    "assets/veiculos/carro_verde.png",
    "assets/veiculos/carro_branco.png",
    "assets/veiculos/carro_vermelho.png"
};

const char *ASSETS_CAMINHOS_ONIBUS[ASSETS_QUANTIDADE_ONIBUS] = {
    "assets/veiculos/onibus_vermelho.png",
    "assets/veiculos/onibus_amarelo.png"
};

const char *ASSETS_CAMINHOS_POWERUPS[ASSETS_QUANTIDADE_POWERUPS] = {
    "assets/powerups/escudo.png",
    "assets/powerups/baixa_velocidade.png",
    "assets/powerups/buzina.png",
    "assets/powerups/dobro_de_pontos.png",
    "assets/powerups/limpa_faixa.png"
};

static Texture2D CarregarTexturaSeExistir(const char *caminho)
{
    if (caminho == NULL || !FileExists(caminho)) {
        return (Texture2D){ 0 };
    }

    return LoadTexture(caminho);
}

static void LiberarTextura(Texture2D *textura)
{
    if (textura == NULL || !IsTextureValid(*textura)) {
        return;
    }

    UnloadTexture(*textura);
    *textura = (Texture2D){ 0 };
}

void InicializarAssets(AssetsJogo *assets)
{
    if (assets == NULL) {
        return;
    }

    *assets = (AssetsJogo){ 0 };
}

void CarregarAssets(AssetsJogo *assets)
{
    if (assets == NULL) {
        return;
    }

    if (!IsTextureValid(assets->carroJogador)) {
        assets->carroJogador = CarregarTexturaSeExistir(ASSETS_CAMINHO_CARRO_JOGADOR);
    }

    if (!IsTextureValid(assets->pista)) {
        assets->pista = CarregarTexturaSeExistir(ASSETS_CAMINHO_PISTA);
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_CARROS; i++) {
        if (!IsTextureValid(assets->carros[i])) {
            assets->carros[i] = CarregarTexturaSeExistir(ASSETS_CAMINHOS_CARROS[i]);
        }
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_ONIBUS; i++) {
        if (!IsTextureValid(assets->onibus[i])) {
            assets->onibus[i] = CarregarTexturaSeExistir(ASSETS_CAMINHOS_ONIBUS[i]);
        }
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_POWERUPS; i++) {
        if (!IsTextureValid(assets->powerups[i])) {
            assets->powerups[i] = CarregarTexturaSeExistir(ASSETS_CAMINHOS_POWERUPS[i]);
        }
    }
}

void LiberarAssets(AssetsJogo *assets)
{
    if (assets == NULL) {
        return;
    }

    LiberarTextura(&assets->carroJogador);
    LiberarTextura(&assets->pista);

    for (int i = 0; i < ASSETS_QUANTIDADE_CARROS; i++) {
        LiberarTextura(&assets->carros[i]);
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_ONIBUS; i++) {
        LiberarTextura(&assets->onibus[i]);
    }

    for (int i = 0; i < ASSETS_QUANTIDADE_POWERUPS; i++) {
        LiberarTextura(&assets->powerups[i]);
    }
}
