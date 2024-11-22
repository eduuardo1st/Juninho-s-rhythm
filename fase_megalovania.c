#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cabecalho.h"
#include <mmsystem.h>


#define WINDOW_TITLE "Jogo de Ritmo em C"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define HITBOX_TOLERANCIA 20

int numNotas1 = 0;
int pontuacao1 = 0;
int erros1;
BarraTecla barrasTeclas1[4]; // Declaração do array de barras
Nota *listaNotas1; // Cabeça da lista encadeada de notas
Nota notas1[10];

LRESULT CALLBACK WindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GerarNota1(int tecla);
void MoverNotas1(int tempoAtual);
void DesenharNotas1(HDC hdc);
void DesenharBarrasTeclas1(HDC hdc);
void DesenharPontuacao1(HDC hdc);
COLORREF GerarCorPorColuna1(int coluna);
void VerificarAcerto1(WPARAM wParam);

void Fase5() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc1;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "RythmGameWindowClass";

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
        return;
    }

    SetTimer(hwnd, 1, 1, NULL); // A cada 1ms

    ShowWindow(hwnd, SW_SHOW);

    // mÃºsica
    PlaySound("megalovania_novo.wav", NULL, SND_FILENAME | SND_ASYNC | 1);

    MSG msg = {0};
    int tempoAtual = 0;  // VariÃ¡vel para o tempo do jogo em ms
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Incrementa o tempo do jogo
        tempoAtual += 1;
    }
}

LRESULT CALLBACK WindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

                DesenharBarrasTeclas1(hdc);
                DesenharNotas1(hdc);
                DesenharPontuacao1(hdc);

                EndPaint(hwnd, &ps);
            }
            return 0;

        case WM_KEYDOWN:
            VerificarAcerto1(wParam); 
            break;

        case WM_TIMER:
            tempoAtual += 30; 
            MoverNotas1(tempoAtual); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AdicionarNota1(int tecla) {
    Nota *novaNota = (Nota *)malloc(sizeof(Nota));
    if (novaNota == NULL) return; // Verificaï¿½ï¿½o de erro de alocaï¿½ï¿½o

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
    if (listaNotas1 == NULL) {
        listaNotas1 = novaNota;
    } else {
        Nota *temp = listaNotas1;
        while (temp->proxima != NULL) {
            temp = temp->proxima;
        }
        temp->proxima = novaNota;
    }
}

void GerarNota1(int tecla) {
    if (numNotas1 < 10) {
        int x = 0;
        switch (tecla) {
            case 1: x = 150; break;
            case 2: x = 300; break;
            case 3: x = 450; break;
            case 4: x = 600; break;
        }
        notas1[numNotas1].x = x;
        notas1[numNotas1].y = 0;
        notas1[numNotas1].tecla = tecla;
        notas1[numNotas1].cor = GerarCorPorColuna(tecla - 1);
        numNotas1++;
    }
}

erros1 = 0; // Adicione uma variï¿½vel global para erros

bool jogoEncerrado = false; // Variï¿½vel de controle para o estado do jogo

void MoverNotas1(int tempoAtual) {
    // Gerar notas de acordo com o tempo predefinido
    for (int i = 0; i < numeroEventos1; i++) {
        if (eventos1[i].tempo <= tempoAtual && eventos1[i].jaCriada == 0) {
            AdicionarNota1(eventos1[i].tecla);
            eventos1[i].jaCriada = 1;
        }
    }

    // Mover e remover notas
    Nota *atual1 = listaNotas1;
    Nota *anterior1 = NULL;

    while (atual1 != NULL) {
        atual1->y += 5; // Move a nota para baixo

        if (atual1->y > WINDOW_HEIGHT) {
            // Contabiliza um erro se a nota passou da tela
            erros1++;

            // Remover a nota da lista
            if (anterior1 == NULL) {
                listaNotas1 = atual1->proxima;
            } else {
                anterior1->proxima = atual1->proxima;
            }

            // Libera a memï¿½ria da nota removida
            Nota *remover1 = atual1;
            atual1 = atual1->proxima;
            free(remover1);
        } else {
            anterior1 = atual1;
            atual1 = atual1->proxima;
        }
    }

    // Verifica se a soma de erros e pontuaï¿½ï¿½o ï¿½ 539
    if (!jogoEncerrado && pontuacao1 + erros1 == 539) {
        jogoEncerrado = true; // Marca o jogo como encerrado
        char mensagem[100];
        sprintf(mensagem, "Fim da fase!\nPontuaco final: %d\nErros1: %d", pontuacao1, 1);
        MessageBox(NULL, mensagem, "Fim da Fase", MB_OK | MB_ICONINFORMATION);
        PostQuitMessage(0); // Encerra o jogo
    }
}


void DesenharNotas1(HDC hdc) {
    Nota *atual1 = listaNotas1;
    while (atual1 != NULL) {
        HBRUSH brush = CreateSolidBrush(atual1->cor);
        SelectObject(hdc, brush);
        Ellipse(hdc, atual1->x - 20, atual1->y - 20, atual1->x + 20, atual1->y + 20);
        DeleteObject(brush);

        atual1 = atual1->proxima;
    }
}


void DesenharBarrasTeclas1(HDC hdc) {
    int barraHeight = 40;
    int yPosition = WINDOW_HEIGHT - barraHeight - 100;
    int totalBarWidth = 176 * 4;
    int startX = (WINDOW_WIDTH - totalBarWidth) / 2;

    for (int i = 0; i < 4; i++) {
        int x = startX + i * 150;
        int raio = 20 * 1.5;
        barrasTeclas1[i].x = x;
        barrasTeclas1[i].y = yPosition;
        barrasTeclas1[i].raio = raio;
        barrasTeclas1[i].hitbox.left = x - raio;
        barrasTeclas1[i].hitbox.top = yPosition - raio;
        barrasTeclas1[i].hitbox.right = x + raio;
        barrasTeclas1[i].hitbox.bottom = yPosition + raio;

        HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, brush);
        Ellipse(hdc, x - raio, yPosition - raio, x + raio, yPosition + raio);
        DeleteObject(brush);
    }
}

void DesenharPontuacao1(HDC hdc) {
    char pontuacaoTexto[50];
    sprintf(pontuacaoTexto, "Pontos: %d | Erros: %d", pontuacao1, erros1);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, WINDOW_WIDTH - 250, 50, pontuacaoTexto, strlen(pontuacaoTexto));
}

COLORREF GerarCorPorColuna1(int coluna) {
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
            return RGB(255, 255, 255); // Branco (ou outra cor padrï¿½o)
    }
}

void VerificarAcerto1(WPARAM wParam) {
    int tecla = 0;
    if (wParam == 0x44) tecla = 1;  // 'D'
    if (wParam == 0x46) tecla = 2;  // 'F'
    if (wParam == 0x4A) tecla = 3;  // 'J'
    if (wParam == 0x4B) tecla = 4;  // 'K'

    Nota *atual = listaNotas1;
    Nota *anterior = NULL;

    while (atual != NULL) {
        if (atual->tecla == tecla) {
            RECT hitbox = barrasTeclas1[tecla - 1].hitbox;
            if (atual->y >= hitbox.top - HITBOX_TOLERANCIA &&
                atual->y <= hitbox.bottom + HITBOX_TOLERANCIA) {
                pontuacao1++;

                if (anterior == NULL) {
                    listaNotas1 = atual->proxima;
                } else {
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

