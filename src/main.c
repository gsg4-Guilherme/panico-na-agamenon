#include "engine.h"

int main(void)
{
    ConfiguracaoJanela configuracao = CriarConfiguracaoPadrao();
    return RodarEngine(configuracao);
}

/* Eu fiz o main bem seco: pego a configuracao padrao da janela e mando o engine roda o jogo. */
