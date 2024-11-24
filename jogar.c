#include <windows.h>
#include <process.h>
#include "resourses/jogar.h"
#include "resourses/cores.h"

HWND hwndJogar = NULL;
HBRUSH hbrBackgroundJogar;

// Declaração de JogarProc antes de usá-lo
LRESULT CALLBACK JogarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Função para abrir a janela de seleção de fases
void abrirJogar() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = JogarProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "JanelaJogar";
    wc.hbrBackground = CreateSolidBrush(COR_FUNDO_JANELA); // Fundo personalizado
    hbrBackgroundJogar = wc.hbrBackground;

    RegisterClass(&wc);

    // Criação da janela principal
    hwndJogar = CreateWindow(
        "JanelaJogar", "Seleção de Fases",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 500,
        NULL, NULL, wc.hInstance, NULL);

    // Criação dos botões centralizados
    CreateWindow("BUTTON", "Fase 1", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 175, 50, 100, 40, hwndJogar, (HMENU)ID_FASE1, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Fase 2", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 425, 50, 100, 40, hwndJogar, (HMENU)ID_FASE2, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Fase 3", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 175, 120, 100, 40, hwndJogar, (HMENU)ID_FASE3, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Fase 4", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 425, 120, 100, 40, hwndJogar, (HMENU)ID_FASE4, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Fase 5", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 175, 190, 100, 40, hwndJogar, (HMENU)ID_FASE5, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Fase 6", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 425, 190, 100, 40, hwndJogar, (HMENU)ID_FASE6, wc.hInstance, NULL);
    CreateWindow("BUTTON", "?????", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 300, 260, 100, 40, hwndJogar, (HMENU)ID_BOSS, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Voltar", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
                 300, 360, 100, 40, hwndJogar, (HMENU)ID_VOLTAR_FASES, wc.hInstance, NULL);

    ShowWindow(hwndJogar, SW_SHOW);
}


// Função de procedimento de janela para a janela de seleção de fases
LRESULT CALLBACK JogarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT pDraw = (LPDRAWITEMSTRUCT)lParam;

        // Botões estilizados com bordas arredondadas e fundo colorido
        HRGN hRgn = CreateRoundRectRgn(
            pDraw->rcItem.left, pDraw->rcItem.top,
            pDraw->rcItem.right, pDraw->rcItem.bottom,
            6, 6);
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

    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, hbrBackgroundJogar);
        return 1;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
         case ID_FASE1:
            system("fase_boateazul.exe");
        break;

        case ID_FASE2:
            system("fase_piratas.exe");
            break;
        case ID_FASE3:
            system("fase_aquipensando.exe");
            break;
        case ID_FASE4:
            system("fase_giorno.exe");
            break;
         case ID_FASE5:
            system("Bloodbath.exe");
            break;
        case ID_FASE6:
            system("fase_megalovania.exe");
            break;
        case ID_BOSS:
            system("FIM.exe");
        break;
        case ID_VOLTAR_FASES:
            DestroyWindow(hwnd);
            break;
        }
        break;

    case WM_DESTROY:
        hwndJogar = NULL;
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
