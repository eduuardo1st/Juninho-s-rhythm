#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cabecalho.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define WINDOW_TITLE "Jogo de Ritmo em C"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define HITBOX_TOLERANCIA 20

typedef struct Nota Nota;

struct Nota {
    int x, y;
    int tecla;
    COLORREF cor;
    Nota *proxima; // Now properly defined
};

Nota *listaNotas = NULL; // Cabeça da lista encadeada de notas
Nota *atual;
Nota *anterior;

typedef struct {
    int x, y;
    int raio;
    RECT hitbox;
} BarraTecla;

Nota notas[10];
int numNotas = 0;
int pontuacao = 0;
BarraTecla barrasTeclas[4];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GerarNota(int tecla);
void MoverNotas(int tempoAtual);
void DesenharNotas(HDC hdc);
void DesenharBarrasTeclas(HDC hdc);
void DesenharPontuacao(HDC hdc);
COLORREF GerarCorPorColuna(int coluna);
void VerificarAcerto(WPARAM wParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RythmGameWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    SetTimer(hwnd, 1, 1, NULL); // A cada 1ms

    ShowWindow(hwnd, nCmdShow);

    //musica
    PlaySound("giorno_epic.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    MSG msg = {0};
    int tempoAtual = 0;  // Variável para o tempo do jogo em ms
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Incrementa o tempo do jogo
        tempoAtual += 1;
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int tempoAtual = 0;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(169, 169, 169))); 

                DesenharBarrasTeclas(hdc);
                DesenharNotas(hdc);
                DesenharPontuacao(hdc);

                EndPaint(hwnd, &ps);
            }
            return 0;

        case WM_KEYDOWN:
            VerificarAcerto(wParam); 
            break;

        case WM_TIMER:
            tempoAtual += 30; 
            MoverNotas(tempoAtual); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AdicionarNota(int tecla) {
    Nota *novaNota = (Nota *)malloc(sizeof(Nota));
    if (novaNota == NULL) return; // Verificação de erro de alocação

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

    // Adiciona ao final da lista
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

void GerarNota(int tecla) {
    if (numNotas < 10) {
        int x = 0;
        switch (tecla) {
            case 1: x = 150; break;
            case 2: x = 300; break;
            case 3: x = 450; break;
            case 4: x = 600; break;
        }
        notas[numNotas].x = x;
        notas[numNotas].y = 0;
        notas[numNotas].tecla = tecla;
        notas[numNotas].cor = GerarCorPorColuna(tecla - 1);
        numNotas++;
    }
}

int erros = 0; // Adicione uma variável global para erros

void MoverNotas(int tempoAtual) {
    // Gerar notas de acordo com o tempo predefinido
    for (int i = 0; i < numeroEventos; i++) {
        if (eventos[i].tempo <= tempoAtual && eventos[i].jaCriada == 0) {
            AdicionarNota(eventos[i].tecla);
            eventos[i].jaCriada = 1;
        }
    }

    // Mover e remover notas
    Nota *atual = listaNotas;
    Nota *anterior = NULL;

    while (atual != NULL) {
        atual->y += 5; // Move a nota para baixo

        if (atual->y > WINDOW_HEIGHT) {
            // Contabiliza um erro se a nota passou da tela
            erros++;

            // Remover a nota da lista
            if (anterior == NULL) {
                listaNotas = atual->proxima;
            } else {
                anterior->proxima = atual->proxima;
            }

            // Libera a memória da nota removida
            Nota *remover = atual;
            atual = atual->proxima;
            free(remover);
        } else {
            anterior = atual;
            atual = atual->proxima;
        }
    }
}


void DesenharNotas(HDC hdc) {
    Nota *atual = listaNotas;
    while (atual != NULL) {
        HBRUSH brush = CreateSolidBrush(atual->cor);
        SelectObject(hdc, brush);
        Ellipse(hdc, atual->x - 20, atual->y - 20, atual->x + 20, atual->y + 20);
        DeleteObject(brush);

        atual = atual->proxima;
    }
}


void DesenharBarrasTeclas(HDC hdc) {
    int barraHeight = 40;
    int yPosition = WINDOW_HEIGHT - barraHeight - 100;
    int totalBarWidth = 176 * 4;
    int startX = (WINDOW_WIDTH - totalBarWidth) / 2;

    for (int i = 0; i < 4; i++) {
        int x = startX + i * 150;
        int raio = 20 * 1.5;
        barrasTeclas[i].x = x;
        barrasTeclas[i].y = yPosition;
        barrasTeclas[i].raio = raio;
        barrasTeclas[i].hitbox.left = x - raio;
        barrasTeclas[i].hitbox.top = yPosition - raio;
        barrasTeclas[i].hitbox.right = x + raio;
        barrasTeclas[i].hitbox.bottom = yPosition + raio;

        HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
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
    TextOut(hdc, WINDOW_WIDTH - 250, 50, pontuacaoTexto, strlen(pontuacaoTexto));
}

COLORREF GerarCorPorColuna(int coluna) {
    switch (coluna) {
        case 0: // Primeira coluna
            return RGB(255, 255, 0); // Amarelo
        case 1: // Segunda coluna
            return RGB(128, 0, 128); // Roxo
        case 2: // Terceira coluna
            return RGB(0, 0, 0);     // Preto
        case 3: // Quarta coluna
            return RGB(0, 0, 255);   // Azul
        default:
            return RGB(255, 255, 255); // Branco (ou outra cor padrão)
    }
}

void VerificarAcerto(WPARAM wParam) {
    bool tecla1 = false, tecla2 = false, tecla3 = false, tecla4 = false;

    if (wParam == 0x44) tecla1 = true; // tecla 1
    if (wParam == 0x46) tecla2 = true; // tecla 2
    if (wParam == 0x4A) tecla3 = true; // tecla 3
    if (wParam == 0x4B) tecla4 = true; // tecla 4

    Nota *atual = listaNotas;
    Nota *anterior = NULL;

    while (atual != NULL) {
        bool acerto = false;

        if ((atual->tecla == 1 && tecla1) ||
            (atual->tecla == 2 && tecla2) ||
            (atual->tecla == 3 && tecla3) ||
            (atual->tecla == 4 && tecla4)) {

            RECT hitbox = barrasTeclas[atual->tecla - 1].hitbox;

            if (atual->y >= hitbox.top - HITBOX_TOLERANCIA &&
                atual->y <= hitbox.bottom + HITBOX_TOLERANCIA) {

                pontuacao++;
                acerto = true;
            }
        }

        if (acerto) {
            if (anterior == NULL) {
                listaNotas = atual->proxima;
            } else {
                anterior->proxima = atual->proxima;
            }

            Nota *remover = atual;
            atual = atual->proxima;
            free(remover);
        } else {
            anterior = atual;
            atual = atual->proxima;
        }
    }
}
