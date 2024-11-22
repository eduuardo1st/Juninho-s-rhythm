#include <windows.h>
#include <stdio.h>
#include "resourses/menu.h"
#include "resourses/cores.h"  // Incluir o arquivo de cores

HWND hwndMenuPrincipal;
HWND botaoEmHover = NULL;
HBRUSH hbrBackground;  // Brush para o fundo

// Função para alterar as cores
void mudarCores(COLORREF corFundo, COLORREF corTextoBotao, COLORREF corFundoBotao) {
    if (hbrBackground) {
        DeleteObject(hbrBackground);
    }
    hbrBackground = CreateSolidBrush(corFundo);
}

// Função para criar um título (label)
void criarLabel(HWND parent, LPCSTR texto, int x, int y, int largura, int altura, HINSTANCE hInstance) {
    HWND label = CreateWindow(
        "STATIC", texto, WS_VISIBLE | WS_CHILD | SS_CENTER,
        x, y, largura, altura, parent, NULL, hInstance, NULL
    );

    HFONT hFont = CreateFont(
        24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_QUALITY, NULL
    );

    // Definindo a cor do fundo da label para a mesma cor da janela
    HDC hdc = GetDC(label);
    SetBkMode(hdc, TRANSPARENT); // Fundo transparente para o texto
    ReleaseDC(label, hdc);

    SendMessage(label, WM_SETFONT, (WPARAM)hFont, TRUE);
}

// Função para criar botões
HWND criarBotao(HWND parent, LPCSTR texto, int id, int x, int y, int buttonWidth, int buttonHeight, HINSTANCE hInstance) {
    HWND button = CreateWindow(
        "BUTTON", texto, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_FLAT,  // Adicionando BS_FLAT para remover borda
        x, y, buttonWidth, buttonHeight, parent, (HMENU)(uintptr_t)id, hInstance, NULL
    );

    HFONT hFont = CreateFont(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_QUALITY, NULL
    );

    SendMessage(button, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Definindo o fundo do botão como transparente
    SetWindowLongPtr(button, GWL_EXSTYLE, GetWindowLongPtr(button, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(button, RGB(255, 255, 255), 0, LWA_COLORKEY);

    return button;
}

// Função para criar o menu principal
void criarMenuPrincipal(HINSTANCE hInstance, int nCmdShow) {
    // Alterando as cores usando as definições de cores centralizadas
    mudarCores(COR_FUNDO_JANELA, COR_BOTOES_TEXTO, COR_BOTOES_FUNDO);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = MenuProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MenuPrincipal";
    wc.hbrBackground = hbrBackground; // Usando o brush definido

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "MenuPrincipal", "Menu Principal",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        NULL, NULL, hInstance, NULL
    );

    criarLabel(hwnd, "Juninho Tiles", 0, 10, 600, 50, hInstance);

    int buttonWidth = 180, buttonHeight = 40, startY = 80;
    int posX = (600 - buttonWidth) / 2;

    criarBotao(hwnd, "Jogar", ID_JOGAR, posX, startY, buttonWidth, buttonHeight, hInstance);
    criarBotao(hwnd, "Configuração", ID_CONFIGURACAO, posX, startY + 60, buttonWidth, buttonHeight, hInstance);
    criarBotao(hwnd, "Pontuação", ID_PONTUACAO, posX, startY + 120, buttonWidth, buttonHeight, hInstance);
    criarBotao(hwnd, "Sair", ID_SAIR, posX, startY + 180, buttonWidth, buttonHeight, hInstance);

    ShowWindow(hwnd, nCmdShow);
}

// Procedimento de janela para o menu principal
LRESULT CALLBACK MenuProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
        case WM_ERASEBKGND: {
            // Captura da mensagem de fundo
            HDC hdc = (HDC)wParam;
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hbrBackground); // Preenche o fundo com a cor definida
            return 1; // Indica que tratamos a mensagem
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pDraw = (LPDRAWITEMSTRUCT)lParam;

            // Criar uma região arredondada para o botão
            HRGN hRgn = CreateRoundRectRgn(
                pDraw->rcItem.left, pDraw->rcItem.top,
                pDraw->rcItem.right, pDraw->rcItem.bottom,
                20, 20 // Raio horizontal e vertical das bordas arredondadas
            );

            // Definir a região arredondada no contexto de dispositivo
            SelectClipRgn(pDraw->hDC, hRgn);

            // Preencher o fundo do botão com a cor especificada
            HBRUSH hBrush = CreateSolidBrush(COR_BOTOES_FUNDO);
            FillRect(pDraw->hDC, &pDraw->rcItem, hBrush);
            DeleteObject(hBrush);

            // Obter o texto do botão
            char textoBotao[256];
            GetWindowText(pDraw->hwndItem, textoBotao, sizeof(textoBotao));

            // Definir a cor do texto e desenhá-lo
            SetBkMode(pDraw->hDC, TRANSPARENT); // Fundo transparente para o texto
            SetTextColor(pDraw->hDC, COR_BOTOES_TEXTO);
            DrawText(pDraw->hDC, textoBotao, -1, &pDraw->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Limpar a região arredondada
            SelectClipRgn(pDraw->hDC, NULL);
            DeleteObject(hRgn);

            return TRUE;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_JOGAR: abrirJogar(hwnd); break;
                case ID_CONFIGURACAO: abrirConfiguracao(hwnd); break;
                case ID_PONTUACAO: abrirPontuacao(hwnd); break;
                case ID_SAIR: PostQuitMessage(0); break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
