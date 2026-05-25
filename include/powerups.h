#ifndef POWERUPS_H
#define POWERUPS_H

#include "tipos.h"

// Prepara todos os campos de power-up para uma nova partida.
void InicializarPowerUps(EstadoJogo* jogo);

// Carrega os sprites usados pelos power-ups.
void CarregarTexturasPowerUps(void);

// Libera os sprites usados pelos power-ups.
void LiberarTexturasPowerUps(void);

// Atualiza coleta, ativacao e duracao dos efeitos.
void AtualizarPowerUps(EstadoJogo* jogo, float delta);

// Atualiza power-ups usando uma tecla configurável para ativação.
void AtualizarPowerUpsComTecla(EstadoJogo* jogo, float delta, KeyboardKey teclaPowerUp);

// Desenha o item coletavel que esta na pista.
void DesenharPowerUpColetavel(const EstadoJogo* jogo);

// Desenha a zona da faixa interditada pelo power-up limpa_faixa.
void DesenharZonaInterditada(const EstadoJogo* jogo);

// Desenha o escudo visual ao redor do jogador quando ele esta ativo.
void DesenharEscudoJogador(const EstadoJogo* jogo);

// Desenha o quadrado 40x40 do power-up guardado no canto superior direito.
void DesenharPowerUpGuardado(const EstadoJogo* jogo);

// Desenha textos curtos dos efeitos temporarios ativos.
void DesenharIndicadoresPowerUps(const EstadoJogo* jogo);

#endif