#ifndef TIPOS_H
#define TIPOS_H

#include <stdbool.h>

#include "config.h"
#include "raylib.h" // colisao

// Telas principais do jogo. A engine usa este enum para decidir o que atualizar e desenhar.
typedef enum TelaAtual {
    TELA_MENU = 0,
    TELA_JOGANDO,
    TELA_GAME_OVER
} TelaAtual;

// Configuração da janela passada do main para a engine.
typedef struct ConfiguracaoJanela {
    int largura;
    int altura;
    int fps;
    const char *titulo;
} ConfiguracaoJanela;

typedef enum TipoObstaculo {
    OBSTACULO_CARRO = 0,
    OBSTACULO_ONIBUS
} TipoObstaculo;

typedef enum TipoPowerUp {
    POWERUP_NENHUM = 0,
    POWERUP_ESCUDO,
    POWERUP_FREIO,
    POWERUP_BUZINA,
    POWERUP_DOBRO_PONTOS,
    POWERUP_LIMPA_FAIXA
} TipoPowerUp;

typedef enum ModoJogo {
    MODO_UM_JOGADOR = 0,
    MODO_DOIS_JOGADORES
} ModoJogo;

typedef struct ControlesJogador {
    KeyboardKey esquerda;
    KeyboardKey direita;
    KeyboardKey powerUp;
} ControlesJogador;

// Representa o carro do jogador dentro da pista.
typedef struct Jogador {
    int faixaAtual;
    float posicaoX;
    float posicaoY;
    float largura;
    float altura;
    float velocidadeLateral;
    Rectangle caixaColisao;
} Jogador;

// Cada obstáculo será um nó da lista encadeada.
typedef struct Obstaculo {
    int faixa;
    float posicaoY;
    float velocidade;
    float largura;
    float altura;
    Rectangle caixaColisao;
    struct Obstaculo *proximo;
    TipoObstaculo tipo;
    int indiceSprite;
} Obstaculo;

// Lista encadeada de obstáculos ativos na tela.
typedef struct ListaObstaculos {
    Obstaculo *inicio;
    int quantidade;
} ListaObstaculos;

// Item coletavel de power-up que aparece na pista.
typedef struct PowerUpColetavel {
    bool ativo;
    TipoPowerUp tipo;
    int faixa;
    float posicaoY;
    float velocidade;
    Rectangle caixaColisao;
} PowerUpColetavel;

// Registro simples para leitura e escrita de pontuação.
typedef struct RegistroPontuacao {
    char nome[32];
    int pontos;
} RegistroPontuacao;

// Estado completo que circula entre engine, jogabilidade, interface e dados.
typedef struct EstadoJogo {
    Jogador jogador;
    ListaObstaculos obstaculos;
    int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA];
    int pontuacaoAtual;
    int melhorPontuacao;
    float tempoSobrevivencia;
    float tempoGerarObstaculo;
    float intervaloObstaculo;
    float velocidadeBase;
    float deslocamentoCenario;
    float tempoGerarPowerUp;
    float intervaloPowerUp;
    float tempoFreio;
    float tempoDobroPontos;
    float tempoInterdicaoFaixa;
    float pontosBonusPowerUp;
    int faixaInterditada;
    TipoPowerUp powerUpGuardado;
    PowerUpColetavel powerUpColetavel;
    bool jogoAtivo;
    bool chuvaAtiva;
    bool engarrafamentoAtivo;
    bool escudoAtivo;
} EstadoJogo;

#endif

//aqui tem as structs e enum ,