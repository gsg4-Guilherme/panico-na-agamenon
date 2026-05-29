#ifndef OBSTACULOS_H
#define OBSTACULOS_H

#include "tipos.h"

// Prepara a lista encadeada que guardará os obstáculos ativos.
void InicializarListaObstaculos(ListaObstaculos *lista);

// Carrega os sprites usados pelos obstáculos.
void CarregarTexturasObstaculos(void);

// Libera os sprites usados pelos obstáculos.
void LiberarTexturasObstaculos(void);

// Aloca dinamicamente um novo obstáculo e insere no início da lista.
bool AdicionarObstaculo(ListaObstaculos *lista, int faixa, float velocidade, TipoObstaculo tipo);

// Atualiza posição dos obstáculos e remove os que saíram da tela.
void AtualizarObstaculos(ListaObstaculos *lista, float delta);

// Desenha todos os obstáculos ativos.
void DesenharObstaculos(const ListaObstaculos *lista);

// Verifica colisão entre o carro do jogador e a lista de obstáculos.
bool VerificarColisaoJogadorObstaculos(const Jogador *jogador, const ListaObstaculos *lista);

// Libera todos os nós da lista encadeada.
void LiberarObstaculos(ListaObstaculos *lista);

#endif
