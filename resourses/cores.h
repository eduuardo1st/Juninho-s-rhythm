#ifndef CORES_H
#define CORES_H

#include <windows.h>

// Definições de cores
#define COR_BOTOES_BORDA RGB(0, 0, 0)  // Cor preta para a borda dos botões
#define COR_FUNDO_JANELA       RGB(94, 84, 142)  // Cor de fundo da janela
#define COR_BOTOES_FUNDO       RGB(35, 25, 66)  // Cor de fundo dos botões
#define COR_BOTOES_TEXTO       RGB(224, 177, 203)        // Cor do texto dos botões
#define COR_BOTOES_FUNDO_PASSIVO RGB(35, 25, 66) // Cor de fundo dos botões passivos
#define COR_BOTOES_TEXTO_PASSIVO RGB(224, 177, 203) // Cor do texto dos botões passivos
#define COR_FUNDO_CONFIGURACAO  RGB(94, 84, 142)  // Cor de fundo da janela de configuração
#define COR_BOTOES_SALVAR      RGB(190, 149, 196)       // Cor de fundo do botão Salvar
#define COR_BOTOES_VOLTAR      RGB(190, 149, 196)       // Cor de fundo do botão Voltar

// Função para mudar as cores globais do projeto
void mudarCores(COLORREF corFundo, COLORREF corTextoBotao, COLORREF corFundoBotao);

#endif // CORES_H
