#include "pontuacao.h"

#include <stddef.h>

#include "config.h"
#include "ranking.h"

void InicializarPontuacao(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    jogo->pontuacaoAtual = 0;
    jogo->tempoSobrevivencia = 0.0f;
    jogo->melhorPontuacao = CarregarMelhorPontuacao(CAMINHO_RANKING);
    jogo->chuvaAtiva = false;
    jogo->engarrafamentoAtivo = false;
}

/* Eu zero os campo da partida, puxo o recorde do arquivo e desligo chuva/engarrafamento. */

void AtualizarPontuacao(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL) {
        return;
    }

    jogo->tempoSobrevivencia += delta;
    jogo->pontuacaoAtual = (int)(jogo->tempoSobrevivencia * 10.0f);

    jogo->chuvaAtiva = ((int)jogo->tempoSobrevivencia % 18) >= 12;
    jogo->engarrafamentoAtivo = ((int)jogo->tempoSobrevivencia % 25) >= 20;
}

/* Eu transformo o tempo vivo em pontos, e a chuva/engarrafamento ligam em ciclos pelo tempo de jogo. */
