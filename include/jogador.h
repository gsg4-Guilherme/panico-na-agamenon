#ifndef JOGADOR_H
#define JOGADOR_H

#include "tipos.h"

// Calcula o centro horizontal de uma faixa da pista.
float CalcularCentroFaixa(int faixa);

// Carrega o sprite do carro do jogador.
void CarregarTexturaJogador(void);

// Libera o sprite do carro do jogador.
void LiberarTexturaJogador(void);

// Prepara o carro do jogador no centro da pista.
void InicializarJogador(Jogador jogador);

// Lê o teclado e muda a faixa do carro.
void AtualizarJogador(Jogadorjogador, float delta);

// le os controles configuráveis e muda a faixa do carro
void AtualizarJogadorComControles(Jogador jogador, float delta, const ControlesJogadorcontroles);

// Desenha o carro do jogador.
void DesenharJogador(const Jogador *jogador);

#endif