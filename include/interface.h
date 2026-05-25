#ifndef INTERFACE_H
#define INTERFACE_H

#include "tipos.h"

// Desenha a tela inicial do projeto.
void DesenharTelaMenu(void);

// Desenha pontuação, recorde e pequenas instruções temporárias.
void DesenharHud(const EstadoJogo* jogo);

// desenha a pontuação, recorde e instruções de controles customizadas
void DesenharHudComInstrucoes(const EstadoJogo* jogo, const char* instrucoes);

// Desenha a camada de fim de jogo por cima da partida.
void DesenharTelaGameOver(const EstadoJogo *jogo);

// desenha a camada final comparando as duas pontuações
void DesenharTelaGameOverDoisJogadores(const EstadoJogo* jogador1, const EstadoJogo* jogador2);

// desenha um aviso sobre uma pista que já perdeu no modo dois jogadores
void DesenharAvisoJogadorBatido(void);

// desenha os indicadores simples pra chuva e engarrafamento.
void DesenharEventos(const EstadoJogo *jogo);

#endif
