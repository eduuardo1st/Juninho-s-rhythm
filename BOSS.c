#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <stdlib.h>
#include <time.h>
#include "resourses/cores.h"
#include "resourses/cabecalho.h"
#include <mmsystem.h>
#include "resourses\util.h"
#include "resourses\configuracao.h"

#define WINDOW_TITLE "Jogo de Ritmo em C"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define HITBOX_TOLERANCIA 30
#define HITBOX_INVISIVEL_Y (WINDOW_HEIGHT - 70) // Linha abaixo dos bot�es
#define VIDA_MAXIMA 50 // Vida m�xima da barra

int vida_atual = VIDA_MAXIMA; // Inicia com vida cheia
int erros_consecutivos = 0;   // Conta os erros consecutivos

ULONG_PTR gdiplusToken;
GpImage* backgroundImage = NULL; // Ponteiro para a imagem de fundo
HBITMAP hBackgroundBitmap = NULL; // Bitmap do fundo em mem�ria

HWND hwnd;
int numNotas = 0;
int pontuacao = 0;
int erros;
BarraTecla barrasTeclas[4];
Nota *listaNotas; // Lista encadeada para as notas geradas aleatoriamente.

void CriarImagemDeFundo(HWND hwnd) {
    HDC hdc = GetDC(hwnd);

    // Criar um contexto de mem�ria compat�vel
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    SelectObject(memDC, hBitmap);

    // Usar GDI+ para desenhar no contexto de mem�ria
    GpGraphics* graphics;
    GdipCreateFromHDC(memDC, &graphics);
    GdipDrawImageRectI(graphics, backgroundImage, 323, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Limpar recursos
    GdipDeleteGraphics(graphics);
    ReleaseDC(hwnd, hdc);

    // Armazenar o bitmap gerado
    hBackgroundBitmap = hBitmap;
    DeleteDC(memDC);
}

void DesenharImagemDeFundo(HDC hdc) {
    if (hBackgroundBitmap) {
        HDC memDC = CreateCompatibleDC(hdc);
        SelectObject(memDC, hBackgroundBitmap);

        // Copiar o fundo para o contexto da janela
        BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);

        DeleteDC(memDC);
    }
}


void DesenharBarraDeVida(HDC hdc) {
    int largura_total = 200;  // Largura m�xima da barra
    int altura_barra = 20;    // Altura da barra
    int x_posicao = WINDOW_WIDTH - 300; // Posi��o X da barra
    int y_posicao = 100;                // Posi��o Y da barra

    // Calcula a largura proporcional � vida atual
    int largura_atual = (vida_atual * largura_total) / VIDA_MAXIMA;

    // Cor de fundo (vida perdida)
    HBRUSH fundo = CreateSolidBrush(RGB(255, 0, 0)); // Vermelho
    RECT fundo_rect = {x_posicao, y_posicao, x_posicao + largura_total, y_posicao + altura_barra};
    FillRect(hdc, &fundo_rect, fundo);
    DeleteObject(fundo);

    // Cor da vida restante
    HBRUSH vida = CreateSolidBrush(RGB(0, 255, 0)); // Verde
    RECT vida_rect = {x_posicao, y_posicao, x_posicao + largura_atual, y_posicao + altura_barra};
    FillRect(hdc, &vida_rect, vida);
    DeleteObject(vida);

    // Texto da vida
    char vidaTexto[20];
    sprintf(vidaTexto, "Vida: %d/%d", vida_atual, VIDA_MAXIMA);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x_posicao + 50, y_posicao - 20, vidaTexto, strlen(vidaTexto));
}


void RegistrarErro(HWND hwnd) {
    erros++; // Incrementa o contador de erros
    erros_consecutivos++;

    if (erros_consecutivos >= 1 && !(vida_atual <= 0)) { // A cada 3 erros consecutivos
        vida_atual--;             // Reduz a vida
        erros_consecutivos = 0;   // Reseta os erros consecutivos

        if (vida_atual <= 0) {
            DestroyWindow(hwnd); // Verifica se a vida acabou
            finalizarFase(hwnd); // Encerrar o jogo
        }
    }
}

void salvarPontuacao(int novaPontuacao) {
    const char *arquivoPontuacao = "resourses/pontuacaoB.txt";
    int pontuacaoAnterior = 0;

    FILE *arquivo = fopen(arquivoPontuacao, "r");
    if (arquivo != NULL) {
        fscanf(arquivo, "O FIM:     %d\n", &pontuacaoAnterior);
        fclose(arquivo);
    }

    int pontuacaoFinal = (novaPontuacao > pontuacaoAnterior) ? novaPontuacao : pontuacaoAnterior;

    arquivo = fopen(arquivoPontuacao, "w");
    if (arquivo != NULL) {
        fprintf(arquivo, "O FIM:    %d\n", pontuacaoFinal);
        fclose(arquivo);
    } else {
        printf("Erro ao salvar a pontua��o.\n");
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AdicionarNota(int tecla);
void GerarNotaAleatoria(int tempoAtual);
void MoverNotas();
void DesenharNotas(HDC hdc);
void DesenharBarrasTeclas(HDC hdc);
void DesenharPontuacao(HDC hdc);
COLORREF GerarCorPorColuna(int coluna);
void VerificarAcerto(WPARAM wParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "RythmGameWindowClass";
    // Inicializa o GDI+
    GdiplusStartupInput gdiplusStartupInput = { 0 };
    gdiplusStartupInput.GdiplusVersion = 1;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    srand((unsigned)time(NULL));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    SetTimer(hwnd, 1, 30, NULL); // A cada 30ms

   // Carrega a imagem de fundo
    GdipLoadImageFromFile(L"assets/sans_FIM1.png", &backgroundImage);
    if (!backgroundImage) {
        MessageBox(NULL, "Erro ao carregar a imagem!", "Erro", MB_ICONERROR);
        return 1;
    }

    // Criar o bitmap de fundo na mem�ria
    CriarImagemDeFundo(hwnd);


    ShowWindow(hwnd, SW_SHOW);

    PlaySound("assets/fim.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // Limpeza de recursos
    if (backgroundImage) {
        GdipDisposeImage(backgroundImage);
    }
    if (hBackgroundBitmap) {
        DeleteObject(hBackgroundBitmap);
    }
    GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}


bool jogoFinalizado = false; // Vari�vel de controle para verificar se o jogo j� foi finalizado

void finalizarFase(HWND hwnd) {
    if (jogoFinalizado) return; // Se o jogo j� foi finalizado, n�o faz nada

    jogoFinalizado = true; // Marca que o jogo foi finalizado

    char mensagem[100];
    DestroyWindow(hwnd);
    sprintf(mensagem, "Game Over!\nPontua��o final: %d\nErros: %d", pontuacao, erros);
    MessageBox(NULL, mensagem, "Fim do Jogo", MB_OK | MB_ICONERROR);
    
    salvarPontuacao(pontuacao);
    
    InvalidateRect(hwnd, NULL, TRUE);
    PostQuitMessage(0); // Encerra o execut�vel
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int tempoAtual = 0;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_ERASEBKGND:
            // Retorna 1 para evitar a limpeza do fundo autom�tica
            return 1;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, CreateSolidBrush(COR_FUNDO_BOSS));

            // Declara��es para Double Buffering
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
            HBITMAP oldBitmap = SelectObject(memDC, memBitmap);

           // Desenhar fundo e elementos do jogo no contexto de mem�ria
            DesenharImagemDeFundo(memDC);
            DesenharBarrasTeclas(memDC);
            DesenharNotas(memDC);
            DesenharPontuacao(memDC);
            DesenharBarraDeVida(memDC);

             // Copiar do contexto de mem�ria para o contexto da janela
            BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);

            // Limpar recursos do contexto de mem�ria
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);


            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_KEYDOWN:
            VerificarAcerto(wParam);
            break;

        case WM_TIMER:
            tempoAtual += 30;
            GerarNotaAleatoria(tempoAtual); // Gera notas aleat�rias conforme o tempo
            MoverNotas(); // Move as notas existentes
            RECT areaNotas = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            InvalidateRect(hwnd, &areaNotas, FALSE);
            break;

        case WM_CLOSE:
            finalizarFase(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AdicionarNota(int tecla) {
    Nota *novaNota = (Nota *)malloc(sizeof(Nota));
    if (novaNota == NULL) return;

    int x = 0;
    switch (tecla) {
        case 1: x = 150; break;
        case 2: x = 300; break;
        case 3: x = 450; break;
        case 4: x = 600; break;
    }

    novaNota->x = x;
    novaNota->y = 0;
    novaNota->tecla = tecla;
    novaNota->cor = GerarCorPorColuna(tecla - 1);
    novaNota->proxima = NULL;

    if (listaNotas == NULL) {
        listaNotas = novaNota;
    } else {
        Nota *temp = listaNotas;
        while (temp->proxima != NULL) {
            temp = temp->proxima;
        }
        temp->proxima = novaNota;
    }
}

void GerarNotaAleatoria(int tempoAtual) {
    static int proximoTempoGeracao = 0;

    if (tempoAtual >= proximoTempoGeracao) {
        int tecla = rand() % 4 + 1;
        AdicionarNota(tecla);
        proximoTempoGeracao = tempoAtual + (rand() % 150 + 150);
    }
}

void MoverNotas() {
    Nota *atual = listaNotas;
    Nota *anterior = NULL;

    while (atual != NULL) {
        atual->y += 13; // Move a nota para baixo

        // Verificar se a nota ultrapassou a hitbox invis�vel
        if (atual->y > HITBOX_INVISIVEL_Y) { 
            // Contabilizar erro
            RegistrarErro(hwnd);
            // Remover a nota da lista encadeada
            if (anterior == NULL) {
                listaNotas = atual->proxima; // Ajustar a cabe�a da lista
            } else {
                anterior->proxima = atual->proxima; // Remover nota no meio da lista
            }

            Nota *remover = atual;
            atual = atual->proxima;
            free(remover); // Liberar mem�ria da nota
        } else {
            anterior = atual;
            atual = atual->proxima; // Avan�ar para a pr�xima nota
        }
    }
}

void DesenharNotas(HDC hdc) {
    Nota *atual = listaNotas;
    while (atual != NULL) {
        // Definir os v�rtices do tri�ngulo equil�tero invertido
        POINT vertices[3];
        int lado = 60;  // Lado do tri�ngulo equil�tero (maior que antes)
        int altura = (int)((lado * sqrt(3)) / 2); // Altura do tri�ngulo equil�tero

        // Calcular os v�rtices
        vertices[0].x = atual->x;               // V�rtice inferior (ponta para baixo)
        vertices[0].y = atual->y + altura;

        vertices[1].x = atual->x - (lado / 2);  // V�rtice superior esquerdo
        vertices[1].y = atual->y;

        vertices[2].x = atual->x + (lado / 2);  // V�rtice superior direito
        vertices[2].y = atual->y;

        // Desenhar o tri�ngulo invertido
        HBRUSH brush = CreateSolidBrush(atual->cor);
        SelectObject(hdc, brush);
        Polygon(hdc, vertices, 3);  // Desenha o tri�ngulo com os v�rtices definidos
        DeleteObject(brush);

        atual = atual->proxima;
    }
}


void DesenharBarrasTeclas(HDC hdc) {
    int barraHeight = 40;
    int yPosition = WINDOW_HEIGHT - barraHeight - 100;
    int totalBarWidth = 176 * 4;
    int startX = (WINDOW_WIDTH - totalBarWidth) / 2;
    int i;

    for (i = 0; i < 4; i++) {
        int x = startX + i * 150;
        int raio = 20 * 1.5;
        barrasTeclas[i].x = x;
        barrasTeclas[i].y = yPosition;
        barrasTeclas[i].raio = raio;
        barrasTeclas[i].hitbox.left = x - raio;
        barrasTeclas[i].hitbox.top = yPosition - raio;
        barrasTeclas[i].hitbox.right = x + raio;
        barrasTeclas[i].hitbox.bottom = yPosition + raio;

        HBRUSH brush = CreateSolidBrush(COR_BOTOES_BOSS);
        SelectObject(hdc, brush);
        Ellipse(hdc, x - raio, yPosition - raio, x + raio, yPosition + raio);
        DeleteObject(brush);
    }
}



void DesenharPontuacao(HDC hdc) {
    char pontuacaoTexto[50];
    sprintf(pontuacaoTexto, "Pontos: %d | Erros: %d", pontuacao, erros);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, WINDOW_WIDTH - 250, 90, pontuacaoTexto, strlen(pontuacaoTexto));
}

COLORREF GerarCorPorColuna(int coluna) {
    switch (coluna) {
        case 0: return COR_NOTAS_1_BOSS;
        case 1: return COR_NOTAS_2_BOSS;
        case 2: return COR_NOTAS_3_BOSS;
        case 3: return COR_NOTAS_4_BOSS;
    }
    return RGB(0, 0, 0);
}

void VerificarAcerto(WPARAM wParam)
{

    unsigned char tecla1, tecla2, tecla3, tecla4;

    FILE *arquivo = fopen("resourses/teclas.txt", "r");
    fgets(buffer, 128, arquivo);
    fclose(arquivo);
    sscanf(buffer, "%c %c %c %c", &tecla1, &tecla2, &tecla3, &tecla4);

    int teclaPressionada = 0;
    int tecla = 0;
   
    if(wParam == tecla1) teclaPressionada = 1;
    if(wParam == tecla2) teclaPressionada = 2;
    if(wParam == tecla3) teclaPressionada = 3;
    if(wParam == tecla4) teclaPressionada = 4;

    Nota *atual = listaNotas;
    Nota *anterior = NULL;

    while (atual != NULL)
    {
        if (atual->tecla == teclaPressionada)
        {
            RECT hitbox = barrasTeclas[teclaPressionada - 1].hitbox;
            if (atual->y >= hitbox.top - HITBOX_TOLERANCIA &&
                atual->y <= hitbox.bottom + HITBOX_TOLERANCIA)
            {
                pontuacao++;

                if (anterior == NULL)
                {
                    listaNotas = atual->proxima;
                }
                else
                {
                    anterior->proxima = atual->proxima;
                }

                Nota *remover = atual;
                atual = atual->proxima;
                free(remover);
                return;
            }
        }

        anterior = atual;
        atual = atual->proxima;
    }
}




