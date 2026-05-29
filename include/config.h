#ifndef CONFIG_H
#define CONFIG_H

// Configurações principais da janela e do ritmo do jogo.
#define LARGURA_JANELA 900
#define ALTURA_JANELA 700
#define FPS_ALVO 60
#define TITULO_JANELA "Pânico na Agamenon"

// Configurações da pista usadas pelos módulos de jogabilidade.
#define QUANTIDADE_FAIXAS 5
#define LARGURA_PISTA_VISUAL 650.0f
#define LINHAS_PISTA 12
#define COLUNAS_PISTA QUANTIDADE_FAIXAS

// Caminho do arquivo simples que será usado para guardar o melhor recorde.
#define CAMINHO_RANKING "data/ranking.txt"

#endif
