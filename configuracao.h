#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <windows.h>

// IDs dos elementos na janela de configuração
#define TAMANHO_BUFFER 128

#define ID_SALVAR_CONFIG 201
#define ID_VOLTAR_CONFIG 202

// Função para abrir a janela de configurações
void abrirConfiguracao(HWND hwnd);
LRESULT CALLBACK ConfiguracaoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Função para salvar a configuração de teclas em um arquivo
void salvarConfiguracao(char tecla1, char tecla2, char tecla3, char tecla4);

#endif
