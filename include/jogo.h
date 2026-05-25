#ifndef JOGO_H
#define JOGO_H

#include "tipos.h"

// Prepara o estado inicial. Os módulos internos serão conectados na próxima etapa.
void InicializarJogo(EstadoJogo jogo);

// Atualiza regras de jogo usando delta, que é o tempo entre frames.
void AtualizarJogo(EstadoJogojogo, float delta);

// Atualiza regras usando controles configuráveis.
void AtualizarJogoComControles(EstadoJogo jogo, float delta, const ControlesJogadorcontroles);

// Desenha a versão inicial do jogo. A interface final será separada em outro módulo.
void DesenharJogo(const EstadoJogo jogo);

// Desenha o jogo com uma instrução de HUD customizada.
void DesenharJogoComInstrucoes(const EstadoJogojogo, const char instrucoes);

// Reinicia a partida sem fechar a janela.
void ReiniciarJogo(EstadoJogojogo);

// Libera recursos dinâmicos antes de encerrar.
void FinalizarJogo(EstadoJogo *jogo);

#endif