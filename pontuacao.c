#include <windows.h>
#include <stdio.h>
#include "resourses/pontuacao.h"
#include "resourses/util.h"
#include "resourses/cores.h"

#define TAMANHO_BUFFER 100

HWND hwndPontuacao = NULL;
HBRUSH hbrBackgroundPontuacao;

// Declaração de PontuacaoProc antes de usá-lo
LRESULT CALLBACK PontuacaoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Função para abrir a janela de pontuação
void abrirPontuacao() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = PontuacaoProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "JanelaPontuacao";
    wc.hbrBackground = CreateSolidBrush(COR_FUNDO_JANELA); // Fundo personalizado
    hbrBackgroundPontuacao = wc.hbrBackground;

    RegisterClass(&wc);

    hwndPontuacao = CreateWindow(
        "JanelaPontuacao", "Pontuacoes Gerais",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        NULL, NULL, wc.hInstance, NULL
    );

    // Leitura das pontuações de vários arquivos
    char pontuacoes[6][TAMANHO_BUFFER];
    const char *arquivos[6] = {
        "resourses/pontuacao1.txt",
        "resourses/pontuacao2.txt",
        "resourses/pontuacao3.txt",
        "resourses/pontuacao4.txt",
        "resourses/pontuacao5.txt",
        "resourses/pontuacaoB.txt"
    };

    for (int i = 0; i < 6; i++) {
        if (!lerArquivo(arquivos[i], pontuacoes[i], TAMANHO_BUFFER)) {
            snprintf(pontuacoes[i], TAMANHO_BUFFER, "Pontuacao %d: 0", i + 1);
        }
    }

    // Criação de elementos da interface para exibir cada pontuação
    for (int i = 0; i < 6; i++) {
        CreateWindow("STATIC", pontuacoes[i], WS_VISIBLE | WS_CHILD | SS_CENTER,
                     50, 50 + (i * 50), 400, 40, hwndPontuacao, NULL, wc.hInstance, NULL);
    }

    CreateWindow("BUTTON", "Voltar", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                 200, 430, 100, 40, hwndPontuacao, (HMENU)ID_VOLTAR_PONTUACAO, wc.hInstance, NULL);

    ShowWindow(hwndPontuacao, SW_SHOW);
}


// Função de procedimento de janela para a janela de pontuação
LRESULT CALLBACK PontuacaoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pDraw = (LPDRAWITEMSTRUCT)lParam;

            // Botão estilizado com bordas arredondadas e fundo colorido
            HRGN hRgn = CreateRoundRectRgn(
                pDraw->rcItem.left, pDraw->rcItem.top,
                pDraw->rcItem.right, pDraw->rcItem.bottom,
                6, 6
            );
            SelectClipRgn(pDraw->hDC, hRgn);
            HBRUSH hBrush = CreateSolidBrush(COR_BOTOES_FUNDO);
            FillRect(pDraw->hDC, &pDraw->rcItem, hBrush);
            DeleteObject(hBrush);

            char textoBotao[256];
            GetWindowText(pDraw->hwndItem, textoBotao, sizeof(textoBotao));
            SetBkMode(pDraw->hDC, TRANSPARENT);
            SetTextColor(pDraw->hDC, COR_BOTOES_TEXTO);
            DrawText(pDraw->hDC, textoBotao, -1, &pDraw->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectClipRgn(pDraw->hDC, NULL);
            DeleteObject(hRgn);
            return TRUE;
        }

        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hbrBackgroundPontuacao);
            return 1;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_VOLTAR_PONTUACAO) {
                DestroyWindow(hwnd);
            }
            break;

        case WM_DESTROY:
            hwndPontuacao = NULL;
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
