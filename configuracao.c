#include <windows.h>
#include <stdio.h>
#include "resourses/configuracao.h"
#include "resourses/util.h"
#include "resourses/cores.h"

LRESULT CALLBACK ConfiguracaoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND hwndConfiguracao, editTecla1, editTecla2, editTecla3, editTecla4;
HBRUSH hbrBackgroundConfig;

void abrirConfiguracao(HWND hwnd) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = ConfiguracaoProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "ConfiguracaoWindowClass";
    wc.hbrBackground = CreateSolidBrush(COR_FUNDO_JANELA); // Fundo com a cor definida
    hbrBackgroundConfig = wc.hbrBackground;

    RegisterClass(&wc);

    hwndConfiguracao = CreateWindow(
        "ConfiguracaoWindowClass", "Configuração", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        hwnd, NULL, GetModuleHandle(NULL), NULL
    );

    ShowWindow(hwndConfiguracao, SW_SHOW);
}

LRESULT CALLBACK ConfiguracaoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static char buffer[TAMANHO_BUFFER];

    switch (msg) {
        case WM_CREATE:
            // Cria os campos de edição para cada tecla
            CreateWindow("STATIC", "Tecla 1:", WS_VISIBLE | WS_CHILD, 50, 50, 60, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            editTecla1 = CreateWindow("EDIT", "D", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 50, 30, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            CreateWindow("STATIC", "Tecla 2:", WS_VISIBLE | WS_CHILD, 50, 80, 60, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            editTecla2 = CreateWindow("EDIT", "F", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 80, 30, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            CreateWindow("STATIC", "Tecla 3:", WS_VISIBLE | WS_CHILD, 50, 110, 60, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            editTecla3 = CreateWindow("EDIT", "J", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 110, 30, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            CreateWindow("STATIC", "Tecla 4:", WS_VISIBLE | WS_CHILD, 50, 140, 60, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            editTecla4 = CreateWindow("EDIT", "K", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 140, 30, 20, hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            // Cria os botões com BS_OWNERDRAW para estilização
            CreateWindow("BUTTON", "Salvar", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 50, 200, 80, 30, hwnd, (HMENU)ID_SALVAR_CONFIG, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            CreateWindow("BUTTON", "Voltar", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 150, 200, 80, 30, hwnd, (HMENU)ID_VOLTAR_CONFIG, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            // Carrega as configurações salvas ao abrir a janela
            if (lerArquivo("resourses/teclas.txt", buffer, TAMANHO_BUFFER)) {
                char tecla1, tecla2, tecla3, tecla4;
                sscanf(buffer, "%c %c %c %c", &tecla1, &tecla2, &tecla3, &tecla4);

                char temp[2] = {0};
                temp[0] = tecla1;
                SetWindowText(editTecla1, temp);

                temp[0] = tecla2;
                SetWindowText(editTecla2, temp);

                temp[0] = tecla3;
                SetWindowText(editTecla3, temp);

                temp[0] = tecla4;
                SetWindowText(editTecla4, temp);
            }
            break;

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pDraw = (LPDRAWITEMSTRUCT)lParam;

            // Botões arredondados com fundo colorido
            HRGN hRgn = CreateRoundRectRgn(
                pDraw->rcItem.left, pDraw->rcItem.top,
                pDraw->rcItem.right, pDraw->rcItem.bottom,
                20, 20
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
            FillRect(hdc, &rect, hbrBackgroundConfig);
            return 1;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_SALVAR_CONFIG:
                    GetWindowText(editTecla1, buffer, TAMANHO_BUFFER);
                    char tecla1 = buffer[0];
                    GetWindowText(editTecla2, buffer, TAMANHO_BUFFER);
                    char tecla2 = buffer[0];
                    GetWindowText(editTecla3, buffer, TAMANHO_BUFFER);
                    char tecla3 = buffer[0];
                    GetWindowText(editTecla4, buffer, TAMANHO_BUFFER);
                    char tecla4 = buffer[0];

                    snprintf(buffer, sizeof(buffer), "%c %c %c %c", tecla1, tecla2, tecla3, tecla4);
                    if (salvarArquivo("resourses/teclas.txt", buffer)) {
                        MessageBox(NULL, "Configurações salvas com sucesso!", "Sucesso", MB_OK);
                    } else {
                        MessageBox(NULL, "Erro ao salvar configurações.", "Erro", MB_OK);
                    }
                    break;

                case ID_VOLTAR_CONFIG:
                    DestroyWindow(hwnd);
                    break;
            }
            break;

        case WM_DESTROY:
            hwndConfiguracao = NULL;
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
