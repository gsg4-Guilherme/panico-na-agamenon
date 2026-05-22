#ifndef CONFIG_H
#define CONFIG_H

// Configurações principais da janela e do ritmo do jogo.
#define LARGURA_JANELA 900
#define ALTURA_JANELA 700
#define LARGURA_DIVISORIA_DOIS_JOGADORES 16
#define LARGURA_JANELA_DOIS_JOGADORES ((LARGURA_JANELA * 2) + LARGURA_DIVISORIA_DOIS_JOGADORES)
#define FPS_ALVO 60
#define TITULO_JANELA "Pânico na Agamenon"

// Configurações da pista usadas pelos módulos de jogabilidade.
#define QUANTIDADE_FAIXAS 5
#define LARGURA_PISTA_VISUAL 650.0f
#define LINHAS_PISTA 12
#define COLUNAS_PISTA QUANTIDADE_FAIXAS

// Caminho do arquivo simples que será usado para guardar o melhor recorde.
#define CAMINHO_RANKING "data/ranking.txt"

#endif //tem que botar pra fechar o codigo uskara

//arquvio de definir valores que iremos usar em outras partes do codigo.
