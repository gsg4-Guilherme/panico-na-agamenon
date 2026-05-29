#ifndef INTERFACE_H
#define INTERFACE_H

#include "tipos.h"

// Desenha a tela inicial do projeto.
void DesenharTelaMenu(void);

// Desenha pontuação, recorde e instruções de controles.
void DesenharHud(const EstadoJogo *jogo);

// Desenha a camada de fim de jogo por cima da partida.
void DesenharTelaGameOver(const EstadoJogo *jogo);

// Desenha indicadores simples para eventos como chuva e engarrafamento.
void DesenharEventos(const EstadoJogo *jogo);

#endif
