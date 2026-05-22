#include "engine.h"

#include <stddef.h>

#include "interface.h"
#include "jogo.h"
#include "raylib.h"

static const ControlesJogador CONTROLES_UM_JOGADOR = {
    .esquerda = KEY_LEFT,
    .direita = KEY_RIGHT,
    .powerUp = KEY_UP
};

static const ControlesJogador CONTROLES_JOGADOR_ESQUERDA = {
    .esquerda = KEY_A,
    .direita = KEY_D,
    .powerUp = KEY_W
};

static const ControlesJogador CONTROLES_JOGADOR_DIREITA = {
    .esquerda = KEY_LEFT,
    .direita = KEY_RIGHT,
    .powerUp = KEY_UP
};

static void AjustarTamanhoJanela(ModoJogo modo)
{
    int larguraDesejada = modo == MODO_DOIS_JOGADORES
        ? LARGURA_JANELA_DOIS_JOGADORES
        : LARGURA_JANELA;

    if (GetScreenWidth() != larguraDesejada || GetScreenHeight() != ALTURA_JANELA) {
        SetWindowSize(larguraDesejada, ALTURA_JANELA);
    }
}

static void ReiniciarPartida(EstadoJogo *jogo1, EstadoJogo *jogo2, ModoJogo modo)
{
    ReiniciarJogo(jogo1);

    if (modo == MODO_DOIS_JOGADORES) {
        ReiniciarJogo(jogo2);
    }
}

static void DesenharDivisoriaDoisJogadores(void)
{
    int x = LARGURA_JANELA;

    DrawRectangle(x, 0, LARGURA_DIVISORIA_DOIS_JOGADORES, ALTURA_JANELA, (Color){ 14, 16, 22, 255 });
    DrawLine(x + 2, 0, x + 2, ALTURA_JANELA, (Color){ 80, 84, 96, 150 });
    DrawLine(
        x + LARGURA_DIVISORIA_DOIS_JOGADORES - 3,
        0,
        x + LARGURA_DIVISORIA_DOIS_JOGADORES - 3,
        ALTURA_JANELA,
        (Color){ 80, 84, 96, 150 }
    );
}

static void DesenharJogoNoViewport(
    const EstadoJogo *jogo,
    int x,
    const char *instrucoes,
    bool mostrarAvisoBatido
)
{
    Camera2D camera = {
        .offset = { (float)x, 0.0f },
        .target = { 0.0f, 0.0f },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    BeginScissorMode(x, 0, LARGURA_JANELA, ALTURA_JANELA);
    BeginMode2D(camera);
    DesenharJogoComInstrucoes(jogo, instrucoes);

    if (mostrarAvisoBatido) {
        DesenharAvisoJogadorBatido();
    }

    EndMode2D();
    EndScissorMode();
}

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

int RodarEngine(ConfiguracaoJanela configuracao)
{
    EstadoJogo jogo1;
    EstadoJogo jogo2;
    TelaAtual telaAtual = TELA_MENU;
    ModoJogo modoAtual = MODO_UM_JOGADOR;

    InitWindow(configuracao.largura, configuracao.altura, configuracao.titulo);
    SetTargetFPS(configuracao.fps);

    InicializarJogo(&jogo1);
    InicializarJogo(&jogo2);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // ESC volta para o menu durante a partida; no menu, fecha a janela.
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (telaAtual == TELA_MENU) {
                break;
            }

            modoAtual = MODO_UM_JOGADOR;
            AjustarTamanhoJanela(modoAtual);
            telaAtual = TELA_MENU;
        }

        if (telaAtual == TELA_MENU && IsKeyPressed(KEY_ENTER)) {
            modoAtual = MODO_UM_JOGADOR;
            AjustarTamanhoJanela(modoAtual);
            ReiniciarPartida(&jogo1, &jogo2, modoAtual);
            telaAtual = TELA_JOGANDO;
        } else if (telaAtual == TELA_MENU && IsKeyPressed(KEY_SPACE)) {
            modoAtual = MODO_DOIS_JOGADORES;
            AjustarTamanhoJanela(modoAtual);
            ReiniciarPartida(&jogo1, &jogo2, modoAtual);
            telaAtual = TELA_JOGANDO;
        } else if (telaAtual == TELA_JOGANDO) {
            if (modoAtual == MODO_DOIS_JOGADORES) {
                AtualizarJogoComControles(&jogo1, delta, &CONTROLES_JOGADOR_ESQUERDA);
                AtualizarJogoComControles(&jogo2, delta, &CONTROLES_JOGADOR_DIREITA);

                if (!jogo1.jogoAtivo && !jogo2.jogoAtivo) {
                    telaAtual = TELA_GAME_OVER;
                }
            } else {
                AtualizarJogoComControles(&jogo1, delta, &CONTROLES_UM_JOGADOR);

                if (!jogo1.jogoAtivo) {
                    telaAtual = TELA_GAME_OVER;
                }
            }
        } else if (telaAtual == TELA_GAME_OVER && IsKeyPressed(KEY_ENTER)) {
            AjustarTamanhoJanela(modoAtual);
            ReiniciarPartida(&jogo1, &jogo2, modoAtual);
            telaAtual = TELA_JOGANDO;
        }

        BeginDrawing();
        ClearBackground((Color){ 22, 25, 35, 255 });

        if (telaAtual == TELA_MENU) {
            DesenharTelaMenu();
        } else if (modoAtual == MODO_DOIS_JOGADORES) {
            DesenharJogoNoViewport(
                &jogo1,
                0,
                "J1: A/D mudam de faixa | W ativa power-up",
                telaAtual == TELA_JOGANDO && !jogo1.jogoAtivo
            );
            DesenharDivisoriaDoisJogadores();
            DesenharJogoNoViewport(
                &jogo2,
                LARGURA_JANELA + LARGURA_DIVISORIA_DOIS_JOGADORES,
                "J2: Setas mudam de faixa | Seta cima ativa power-up",
                telaAtual == TELA_JOGANDO && !jogo2.jogoAtivo
            );

            if (telaAtual == TELA_GAME_OVER) {
                DesenharTelaGameOverDoisJogadores(&jogo1, &jogo2);
            }
        } else {
            DesenharJogoComInstrucoes(&jogo1, NULL);

            if (telaAtual == TELA_GAME_OVER) {
                DesenharTelaGameOver(&jogo1);
            }
        }

        EndDrawing();
    }

    FinalizarJogo(&jogo1);
    FinalizarJogo(&jogo2);
    CloseWindow();
    return 0;
}