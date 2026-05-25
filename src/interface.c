#include "interface.h"

#include <stddef.h>

#include "config.h"
#include "powerups.h"
#include "raylib.h"

static void DesenharFiltroChuva(void)
{
    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 0, 0, 0, 68 });
}

static void DesenharGotasChuva(void)
{
    const int quantidadeGotas = 76;
    const int cicloVertical = ALTURA_JANELA + 160;
    const float larguraGota = 3.0f;
    const float alturaGota = 34.0f;
    const float velocidadeChuva = 520.0f;
    const Color corGota = (Color){ 150, 205, 230, 120 };
    int deslocamento = (int)((long long)(GetTime() * velocidadeChuva) % cicloVertical);

    for (int indice = 0; indice < quantidadeGotas; indice++) {
        int x = (indice * 97) % (LARGURA_JANELA + 180) - 90;
        int y = (indice * 53 + deslocamento) % cicloVertical - 120;
        Rectangle gota = {
            (float)x,
            (float)y,
            larguraGota,
            alturaGota
        };

        DrawRectanglePro(gota, (Vector2){ larguraGota / 2.0f, 0.0f }, -18.0f, corGota);
    }
}

static void DesenharEfeitoChuva(void)
{
    DesenharFiltroChuva();
    DesenharGotasChuva();
}

static void DesenharTextoCentralizado(const char *texto, int centroX, int y, int tamanhoFonte, Color cor)
{
    int larguraTexto = MeasureText(texto, tamanhoFonte);

    DrawText(texto, centroX - (larguraTexto / 2), y, tamanhoFonte, cor);
}

void DesenharTelaMenu(void)
{
    DrawText("Pânico na Agamenon", 250, 220, 38, RAYWHITE);
    DrawText("ENTER - 1 jogador", 335, 290, 22, LIGHTGRAY);
    DrawText("ESPAÇO - 2 jogadores", 315, 325, 22, LIGHTGRAY);
    DrawText("ESC - sair", 390, 360, 18, GRAY);
    DrawText("Objetivo: desviar dos veículos e sobreviver mais tempo.", 175, 420, 20, RAYWHITE);
}

void DesenharHudComInstrucoes(const EstadoJogo *jogo, const char *instrucoes)
{
    const char *textoInstrucoes = instrucoes != NULL
        ? instrucoes
        : "Setas mudam de faixa | Seta cima ativa power-up";

    if (jogo == NULL) {
        return;
    }

    DrawText(TextFormat("Pontos: %d", jogo->pontuacaoAtual), 30, 30, 24, RAYWHITE);
    DrawText(TextFormat("Recorde: %d", jogo->melhorPontuacao), 30, 62, 20, LIGHTGRAY);
    DrawText(TextFormat("Obstáculos: %d", jogo->obstaculos.quantidade), 30, 90, 18, LIGHTGRAY);
    DesenharPowerUpGuardado(jogo);
    DrawText(textoInstrucoes, 245, 650, 18, LIGHTGRAY);
}

void DesenharHud(const EstadoJogo *jogo)
{
    DesenharHudComInstrucoes(jogo, NULL);
}

void DesenharTelaGameOver(const EstadoJogo *jogo)
{
    int pontuacao = jogo != NULL ? jogo->pontuacaoAtual : 0;

    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 0, 0, 0, 150 });
    DrawText("Fim de jogo", 335, 250, 40, RAYWHITE);
    DrawText(TextFormat("Pontuação: %d", pontuacao), 365, 305, 22, LIGHTGRAY);
    DrawText("Pressione ENTER para tentar de novo", 255, 355, 22, LIGHTGRAY);
}

void DesenharTelaGameOverDoisJogadores(const EstadoJogo *jogador1, const EstadoJogo *jogador2)
{
    int pontuacao1 = jogador1 != NULL ? jogador1->pontuacaoAtual : 0;
    int pontuacao2 = jogador2 != NULL ? jogador2->pontuacaoAtual : 0;
    int centroX = GetScreenWidth() / 2;
    const char *resultado = "Empate";

    if (pontuacao1 > pontuacao2) {
        resultado = "Jogador 1 venceu";
    } else if (pontuacao2 > pontuacao1) {
        resultado = "Jogador 2 venceu";
    }

    DrawRectangle(0, 0, GetScreenWidth(), ALTURA_JANELA, (Color){ 0, 0, 0, 165 });
    DesenharTextoCentralizado("Fim de jogo", centroX, 210, 42, RAYWHITE);
    DesenharTextoCentralizado(resultado, centroX, 270, 30, GOLD);
    DesenharTextoCentralizado(TextFormat("Jogador 1: %d pontos", pontuacao1), centroX, 325, 22, LIGHTGRAY);
    DesenharTextoCentralizado(TextFormat("Jogador 2: %d pontos", pontuacao2), centroX, 360, 22, LIGHTGRAY);
    DesenharTextoCentralizado("Pressione ENTER para jogar de novo", centroX, 420, 22, LIGHTGRAY);
}

void DesenharAvisoJogadorBatido(void)
{
    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 0, 0, 0, 110 });
    DesenharTextoCentralizado("BATIDO", LARGURA_JANELA / 2, 310, 44, RAYWHITE);
}

void DesenharEventos(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    if (jogo->chuvaAtiva) {
        DesenharEfeitoChuva();
        DrawText("CHUVA", 680, 30, 20, SKYBLUE);
    }

    if (jogo->engarrafamentoAtivo) {
        DrawText("ENGARRAFAMENTO", 680, 60, 20, ORANGE);
    }

    DesenharIndicadoresPowerUps(jogo);
}