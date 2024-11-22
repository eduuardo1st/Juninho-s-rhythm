#ifndef MENU_H
#define MENU_H

#include <windows.h>

// IDs dos botões no menu principal
#define ID_JOGAR 1
#define ID_CONFIGURACAO 2
#define ID_PONTUACAO 3
#define ID_SAIR 4

// Função para criar o menu principal
void criarMenuPrincipal(HINSTANCE hInstance, int nCmdShow);

// Procedimento de janela para o menu principal
LRESULT CALLBACK MenuProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
