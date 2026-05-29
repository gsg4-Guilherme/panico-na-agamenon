#include "engine.h"

#include "interface.h"
#include "jogo.h"
#include "raylib.h"

static int MaximoInteiro(int a, int b)
{
    return a > b ? a : b;
}

/* Eu devolvo o maior entre dois numeros, usado pra impedir posicao negativa da janela. */

static void CentralizarJanela(int largura, int altura)
{
    int monitor = GetCurrentMonitor();
    Vector2 posicaoMonitor = GetMonitorPosition(monitor);
    int larguraMonitor = GetMonitorWidth(monitor);
    int alturaMonitor = GetMonitorHeight(monitor);
    int x = (int)posicaoMonitor.x + MaximoInteiro((larguraMonitor - largura) / 2, 0);
    int y = (int)posicaoMonitor.y + MaximoInteiro((alturaMonitor - altura) / 2, 0);

    SetWindowPosition(x, y);
}

/* Eu pego as medidas do monitor atual e movo a janela pro meio dele. */

ConfiguracaoJanela CriarConfiguracaoPadrao(void)
{
    ConfiguracaoJanela configuracao = {
        .largura = LARGURA_JANELA,
        .altura = ALTURA_JANELA,
        .fps = FPS_ALVO,
        .titulo = TITULO_JANELA
    };

    return configuracao;
}

/* Eu devolvo uma configuracao pronta com largura, altura, fps e titulo dos defines de config.h. */

int RodarEngine(ConfiguracaoJanela configuracao)
{
    EstadoJogo jogo;
    TelaAtual telaAtual = TELA_MENU;

    InitWindow(configuracao.largura, configuracao.altura, configuracao.titulo);
    SetTargetFPS(configuracao.fps);
    CentralizarJanela(configuracao.largura, configuracao.altura);

    InicializarJogo(&jogo);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // ESC volta para o menu durante a partida; no menu, fecha a janela.
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (telaAtual == TELA_MENU) {
                break;
            }

            telaAtual = TELA_MENU;
        }

        if (telaAtual == TELA_MENU && IsKeyPressed(KEY_ENTER)) {
            ReiniciarJogo(&jogo);
            telaAtual = TELA_JOGANDO;
        } else if (telaAtual == TELA_JOGANDO) {
            AtualizarJogo(&jogo, delta);

            if (!jogo.jogoAtivo) {
                telaAtual = TELA_GAME_OVER;
            }
        } else if (telaAtual == TELA_GAME_OVER && IsKeyPressed(KEY_ENTER)) {
            ReiniciarJogo(&jogo);
            telaAtual = TELA_JOGANDO;
        }

        BeginDrawing();
        ClearBackground((Color){ 22, 25, 35, 255 });

        if (telaAtual == TELA_MENU) {
            DesenharTelaMenu();
        } else {
            DesenharJogo(&jogo);

            if (telaAtual == TELA_GAME_OVER) {
                DesenharTelaGameOver(&jogo);
            }
        }

        EndDrawing();
    }

    FinalizarJogo(&jogo);
    CloseWindow();
    return 0;
}

/* Eu abro a janela e fico alternando entre menu, jogo e game-over conforme as teclas, ate fechar. */
