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

// Remove o primeiro obstáculo que estiver colidindo com o jogador.
bool RemoverPrimeiroObstaculoColidindo(ListaObstaculos *lista, const Jogador *jogador);

// Remove todos os obstáculos de uma faixa específica.
int RemoverObstaculosFaixa(ListaObstaculos *lista, int faixa);

// Altera a velocidade dos obstáculos de uma faixa específica que ainda não saíram por baixo.
void AplicarVelocidadeObstaculosFaixa(ListaObstaculos *lista, int faixa, float velocidade);

// Libera todos os nós da lista encadeada.
void LiberarObstaculos(ListaObstaculos *lista);

#endif
