#include "ranking.h"

#include <stdio.h>

int CarregarMelhorPontuacao(const char *caminho)
{
    int melhorPontuacao = 0;
    FILE *arquivo = fopen(caminho, "r");

    if (arquivo == NULL) {
        return 0;
    }

    if (fscanf(arquivo, "%d", &melhorPontuacao) != 1) {
        melhorPontuacao = 0;
    }

    fclose(arquivo);
    return melhorPontuacao;
}

/* Eu abro o arquivo, leio um numero e fecho; se faltar o arquivo ou der ruim eu volto zero. */

void SalvarPontuacaoSeRecorde(const char *caminho, int pontuacao)
{
    int melhorPontuacao = CarregarMelhorPontuacao(caminho);

    if (pontuacao <= melhorPontuacao) {
        return;
    }

    // Escrita simples em arquivo texto para atender ao requisito de persistência.
    FILE *arquivo = fopen(caminho, "w");

    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "%d\n", pontuacao);
    fclose(arquivo);
}

/* Eu so sobrescrevo o arquivo quando a pontuacao nova passa do recorde antigo. */
