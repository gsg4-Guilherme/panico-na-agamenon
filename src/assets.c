#include "assets.h"

/* Eu deixei so os caminho das imagem nesse arquivo, os outro modulo carrega elas la na frente. */

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
