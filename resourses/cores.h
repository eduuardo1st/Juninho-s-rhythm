#ifndef CORES_H
#define CORES_H

#include <windows.h>

// Definições de cores
#define COR_BOTOES_BORDA RGB(0, 0, 0)  // Cor preta para a borda dos botões
#define COR_FUNDO_JANELA       RGB(255, 194, 209)  // Cor de fundo da janela
#define COR_BOTOES_FUNDO       RGB(251, 111, 146)  // Cor de fundo dos botões
#define COR_BOTOES_TEXTO       RGB(0, 0, 0)        // Cor do texto dos botões
#define COR_BOTOES_FUNDO_PASSIVO RGB(228, 86, 107) // Cor de fundo dos botões passivos
#define COR_BOTOES_TEXTO_PASSIVO RGB(255, 255, 255) // Cor do texto dos botões passivos
#define COR_FUNDO_CONFIGURACAO  RGB(255, 255, 255)  // Cor de fundo da janela de configuração
#define COR_BOTOES_SALVAR      RGB(255, 0, 0)       // Cor de fundo do botão Salvar
#define COR_BOTOES_VOLTAR      RGB(0, 255, 0)       // Cor de fundo do botão Voltar

// Função para mudar as cores globais do projeto
void mudarCores(COLORREF corFundo, COLORREF corTextoBotao, COLORREF corFundoBotao);

#endif // CORES_H
