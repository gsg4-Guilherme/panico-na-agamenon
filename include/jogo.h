#ifndef JOGO_H
#define JOGO_H

#include "tipos.h"

// Calcula o aumento de velocidade que vem do tempo de sobrevivência, com teto.
float CalcularAumentoVelocidadePorTempo(float tempoSobrevivencia);

// Prepara o estado inicial. Os módulos internos serão conectados na próxima etapa.
void InicializarJogo(EstadoJogo *jogo);

// Atualiza as regras da partida usando delta (tempo entre frames).
void AtualizarJogo(EstadoJogo *jogo, float delta);

// Desenha a partida: pista, obstáculos, jogador, eventos e HUD.
void DesenharJogo(const EstadoJogo *jogo);

// Reinicia a partida sem fechar a janela.
void ReiniciarJogo(EstadoJogo *jogo);

// Libera recursos dinâmicos antes de encerrar.
void FinalizarJogo(EstadoJogo *jogo);

#endif
