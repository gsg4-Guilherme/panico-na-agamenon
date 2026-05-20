#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

#define ASSETS_QUANTIDADE_CARROS 3
#define ASSETS_QUANTIDADE_ONIBUS 2
#define ASSETS_QUANTIDADE_POWERUPS 5
#define ASSETS_POWERUP_ESCUDO 0
#define ASSETS_POWERUP_BAIXA_VELOCIDADE 1
#define ASSETS_POWERUP_BUZINA 2
#define ASSETS_POWERUP_DOBRO_PONTOS 3
#define ASSETS_POWERUP_LIMPA_FAIXA 4

extern const char *ASSETS_CAMINHO_CARRO_JOGADOR;
extern const char *ASSETS_CAMINHO_PISTA;
extern const char *ASSETS_CAMINHOS_CARROS[ASSETS_QUANTIDADE_CARROS];
extern const char *ASSETS_CAMINHOS_ONIBUS[ASSETS_QUANTIDADE_ONIBUS];
extern const char *ASSETS_CAMINHOS_POWERUPS[ASSETS_QUANTIDADE_POWERUPS];

typedef struct AssetsJogo {
    Texture2D carroJogador;
    Texture2D pista;
    Texture2D carros[ASSETS_QUANTIDADE_CARROS];
    Texture2D onibus[ASSETS_QUANTIDADE_ONIBUS];
    Texture2D powerups[ASSETS_QUANTIDADE_POWERUPS];
} AssetsJogo;

void InicializarAssets(AssetsJogo *assets);
void CarregarAssets(AssetsJogo *assets);
void LiberarAssets(AssetsJogo *assets);

#endif
