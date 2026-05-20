#include "pista.h"

#include <stddef.h>

#include "assets.h"
#include "config.h"
#include "raylib.h"

#define LARGURA_ACOSTAMENTO 24.0f
#define CICLO_ARVORES 820.0f
#define CICLO_MARCAS_ASFALTO 760.0f

static Texture2D texturaPista = { 0 };

typedef struct ArvoreCenario {
    float deslocamentoX;
    float faseY;
    float escala;
} ArvoreCenario;

typedef struct MarcaAsfalto {
    float x;
    float faseY;
    float largura;
    float altura;
    Color cor;
} MarcaAsfalto;

static float CalcularMargemEsquerdaPista(void)
{
    return (LARGURA_JANELA - LARGURA_PISTA_VISUAL) / 2.0f;
}

static float NormalizarDeslocamento(float deslocamento, float intervalo)
{
    if (intervalo <= 0.0f) {
        return 0.0f;
    }

    if (deslocamento >= intervalo || deslocamento <= -intervalo) {
        int repeticoes = (int)(deslocamento / intervalo);
        deslocamento -= intervalo * repeticoes;
    }

    while (deslocamento >= intervalo) {
        deslocamento -= intervalo;
    }

    while (deslocamento < 0.0f) {
        deslocamento += intervalo;
    }

    return deslocamento;
}

static void DesenharSpritePista(float deslocamentoCenario)
{
    Rectangle origem = { 0.0f, 0.0f, (float)texturaPista.width, (float)texturaPista.height };
    float alturaTela = (float)ALTURA_JANELA;
    float deslocamento = NormalizarDeslocamento(deslocamentoCenario, alturaTela);
    Rectangle destinoSuperior = { 0.0f, deslocamento - alturaTela, (float)LARGURA_JANELA, alturaTela };
    Rectangle destinoInferior = { 0.0f, deslocamento, (float)LARGURA_JANELA, alturaTela };

    DrawTexturePro(texturaPista, origem, destinoSuperior, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    DrawTexturePro(texturaPista, origem, destinoInferior, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

static void DesenharArvore(float centroX, float baseY, float escala)
{
    float troncoLargura = 10.0f * escala;
    float troncoAltura = 28.0f * escala;
    Rectangle tronco = {
        centroX - (troncoLargura / 2.0f),
        baseY - troncoAltura,
        troncoLargura,
        troncoAltura
    };

    DrawEllipse((int)centroX + 6, (int)baseY + 1, 24.0f * escala, 7.0f * escala, (Color){ 11, 32, 25, 120 });
    DrawRectangleRec(tronco, (Color){ 104, 68, 38, 255 });
    DrawRectangleRec((Rectangle){ tronco.x + 2.0f, tronco.y, 2.0f, tronco.height }, (Color){ 145, 94, 50, 255 });

    DrawCircleV((Vector2){ centroX, baseY - (42.0f * escala) }, 25.0f * escala, (Color){ 28, 112, 56, 255 });
    DrawCircleV((Vector2){ centroX - (18.0f * escala), baseY - (32.0f * escala) }, 20.0f * escala, (Color){ 34, 134, 65, 255 });
    DrawCircleV((Vector2){ centroX + (18.0f * escala), baseY - (31.0f * escala) }, 20.0f * escala, (Color){ 23, 96, 50, 255 });
    DrawCircleV((Vector2){ centroX + (2.0f * escala), baseY - (22.0f * escala) }, 18.0f * escala, (Color){ 42, 145, 70, 255 });
}

static void DesenharGramado(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    static const int posicoesGrama[] = { 18, 47, 91, 136, 180, 711, 756, 804, 846, 879 };
    float deslocamentoGrama = NormalizarDeslocamento(deslocamentoCenario, 42.0f);

    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 25, 82, 52, 255 });
    DrawRectangle(0, 0, (int)margemEsquerda, ALTURA_JANELA, (Color){ 30, 92, 57, 255 });
    DrawRectangle((int)(margemEsquerda + larguraPista), 0, (int)margemEsquerda, ALTURA_JANELA, (Color){ 30, 92, 57, 255 });

    for (int linha = -1; linha < 18; linha++) {
        float y = deslocamentoGrama + (linha * 42.0f);

        for (int i = 0; i < (int)(sizeof(posicoesGrama) / sizeof(posicoesGrama[0])); i++) {
            float altura = 7.0f + (float)((linha + i + 8) % 4);
            DrawLineV(
                (Vector2){ (float)posicoesGrama[i], y },
                (Vector2){ (float)posicoesGrama[i] + 4.0f, y - altura },
                (Color){ 55, 132, 73, 180 }
            );
        }
    }
}

static void DesenharArvoresLaterais(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    static const ArvoreCenario arvoresEsquerda[] = {
        { -152.0f, 20.0f, 0.92f },
        { -96.0f, 146.0f, 1.05f },
        { -171.0f, 298.0f, 0.98f },
        { -115.0f, 448.0f, 0.88f },
        { -145.0f, 612.0f, 1.08f },
        { -82.0f, 760.0f, 0.94f }
    };
    static const ArvoreCenario arvoresDireita[] = {
        { 95.0f, 76.0f, 1.04f },
        { 148.0f, 212.0f, 0.91f },
        { 108.0f, 372.0f, 1.08f },
        { 162.0f, 530.0f, 0.96f },
        { 122.0f, 688.0f, 1.0f }
    };
    float deslocamentoArvore = NormalizarDeslocamento(deslocamentoCenario, CICLO_ARVORES);

    for (int i = 0; i < (int)(sizeof(arvoresEsquerda) / sizeof(arvoresEsquerda[0])); i++) {
        float y = NormalizarDeslocamento(deslocamentoArvore + arvoresEsquerda[i].faseY, CICLO_ARVORES) - 70.0f;
        DesenharArvore(margemEsquerda + arvoresEsquerda[i].deslocamentoX, y, arvoresEsquerda[i].escala);
    }

    for (int i = 0; i < (int)(sizeof(arvoresDireita) / sizeof(arvoresDireita[0])); i++) {
        float y = NormalizarDeslocamento(deslocamentoArvore + arvoresDireita[i].faseY, CICLO_ARVORES) - 70.0f;
        DesenharArvore(margemEsquerda + larguraPista + arvoresDireita[i].deslocamentoX, y, arvoresDireita[i].escala);
    }
}

static void DesenharAsfalto(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    static const MarcaAsfalto marcas[] = {
        { 34.0f, 24.0f, 3.0f, 12.0f, { 55, 58, 65, 140 } },
        { 107.0f, 96.0f, 4.0f, 9.0f, { 30, 33, 39, 110 } },
        { 181.0f, 184.0f, 2.0f, 14.0f, { 57, 60, 67, 130 } },
        { 246.0f, 268.0f, 5.0f, 8.0f, { 32, 35, 41, 115 } },
        { 330.0f, 352.0f, 3.0f, 13.0f, { 55, 58, 65, 120 } },
        { 405.0f, 432.0f, 4.0f, 10.0f, { 31, 34, 40, 110 } },
        { 73.0f, 520.0f, 2.0f, 12.0f, { 58, 61, 68, 120 } },
        { 215.0f, 604.0f, 4.0f, 9.0f, { 33, 36, 42, 115 } },
        { 365.0f, 690.0f, 3.0f, 13.0f, { 56, 59, 66, 125 } }
    };
    float deslocamentoMarcas = NormalizarDeslocamento(deslocamentoCenario, CICLO_MARCAS_ASFALTO);

    DrawRectangle((int)margemEsquerda, 0, (int)larguraPista, ALTURA_JANELA, (Color){ 42, 45, 52, 255 });
    DrawRectangle((int)margemEsquerda, 0, 10, ALTURA_JANELA, (Color){ 35, 38, 44, 255 });
    DrawRectangle((int)(margemEsquerda + larguraPista - 10.0f), 0, 10, ALTURA_JANELA, (Color){ 35, 38, 44, 255 });

    for (int i = 0; i < (int)(sizeof(marcas) / sizeof(marcas[0])); i++) {
        float y = NormalizarDeslocamento(deslocamentoMarcas + marcas[i].faseY, CICLO_MARCAS_ASFALTO) - 20.0f;
        DrawRectangleRec(
            (Rectangle){ margemEsquerda + marcas[i].x, y, marcas[i].largura, marcas[i].altura },
            marcas[i].cor
        );
    }
}

static void DesenharAcostamento(float margemEsquerda, float larguraPista)
{
    int xEsquerda = (int)(margemEsquerda - LARGURA_ACOSTAMENTO);
    int xDireita = (int)(margemEsquerda + larguraPista);

    DrawRectangle(xEsquerda, 0, (int)LARGURA_ACOSTAMENTO, ALTURA_JANELA, (Color){ 86, 90, 84, 255 });
    DrawRectangle(xDireita, 0, (int)LARGURA_ACOSTAMENTO, ALTURA_JANELA, (Color){ 86, 90, 84, 255 });
    DrawLine((int)margemEsquerda, 0, (int)margemEsquerda, ALTURA_JANELA, (Color){ 210, 210, 190, 255 });
    DrawLine((int)(margemEsquerda + larguraPista), 0, (int)(margemEsquerda + larguraPista), ALTURA_JANELA, (Color){ 210, 210, 190, 255 });
    DrawLine(xEsquerda, 0, xEsquerda, ALTURA_JANELA, (Color){ 65, 69, 63, 255 });
    DrawLine(xDireita + (int)LARGURA_ACOSTAMENTO, 0, xDireita + (int)LARGURA_ACOSTAMENTO, ALTURA_JANELA, (Color){ 65, 69, 63, 255 });
}

static void DesenharFaixasTracejadas(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;
    float deslocamentoFaixa = NormalizarDeslocamento(deslocamentoCenario, 92.0f);

    for (int faixa = 1; faixa < QUANTIDADE_FAIXAS; faixa++) {
        float x = margemEsquerda + (larguraFaixa * faixa);

        for (int i = -1; i < 9; i++) {
            float y = deslocamentoFaixa + (i * 92.0f);
            DrawRectangleRec((Rectangle){ x - 3.0f, y, 6.0f, 48.0f }, (Color){ 235, 206, 62, 255 });
            DrawRectangleRec((Rectangle){ x - 1.0f, y + 4.0f, 2.0f, 40.0f }, (Color){ 250, 232, 118, 255 });
        }
    }
}

static void DesenharMarcadoresLogicos(const int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA], float margemEsquerda, float larguraPista)
{
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;
    float alturaLinha = (float)ALTURA_JANELA / LINHAS_PISTA;

    for (int linha = 0; linha < LINHAS_PISTA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_PISTA; coluna++) {
            if (pistaLogica[linha][coluna] != 0) {
                int x = (int)(margemEsquerda + (larguraFaixa * coluna) + (larguraFaixa / 2.0f));
                int y = (int)((alturaLinha * linha) + 10.0f);
                Color cor = pistaLogica[linha][coluna] == 1
                    ? (Color){ 80, 160, 255, 90 }
                    : (Color){ 255, 80, 70, 90 };

                DrawCircle(x, y, 4.0f, cor);
            }
        }
    }
}

void InicializarPista(int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA])
{
    for (int linha = 0; linha < LINHAS_PISTA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_PISTA; coluna++) {
            pistaLogica[linha][coluna] = 0;
        }
    }
}

void AtualizarPista(
    int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA],
    const Jogador *jogador,
    const ListaObstaculos *obstaculos
)
{
    InicializarPista(pistaLogica);

    if (jogador != NULL && jogador->faixaAtual >= 0 && jogador->faixaAtual < COLUNAS_PISTA) {
        pistaLogica[LINHAS_PISTA - 2][jogador->faixaAtual] = 1;
    }

    if (obstaculos == NULL) {
        return;
    }

    for (const Obstaculo *obstaculo = obstaculos->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        int linha = (int)((obstaculo->posicaoY / ALTURA_JANELA) * LINHAS_PISTA);

        if (linha >= 0 && linha < LINHAS_PISTA && obstaculo->faixa >= 0 && obstaculo->faixa < COLUNAS_PISTA) {
            pistaLogica[linha][obstaculo->faixa] = 2;
        }
    }
}

void CarregarTexturaPista(void)
{
    if (IsTextureValid(texturaPista) || !FileExists(ASSETS_CAMINHO_PISTA)) {
        return;
    }

    texturaPista = LoadTexture(ASSETS_CAMINHO_PISTA);
}

void LiberarTexturaPista(void)
{
    if (IsTextureValid(texturaPista)) {
        UnloadTexture(texturaPista);
        texturaPista = (Texture2D){ 0 };
    }
}

void DesenharPista(const int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA], float deslocamentoCenario)
{
    float larguraPista = LARGURA_PISTA_VISUAL;
    float margemEsquerda = CalcularMargemEsquerdaPista();

    if (IsTextureValid(texturaPista)) {
        DesenharSpritePista(deslocamentoCenario);
        return;
    }

    DesenharGramado(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharArvoresLaterais(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharAcostamento(margemEsquerda, larguraPista);
    DesenharAsfalto(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharFaixasTracejadas(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharMarcadoresLogicos(pistaLogica, margemEsquerda, larguraPista);
}
