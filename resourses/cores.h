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
#define COR_BOTOES_VOLTAR      RGB(190, 149, 196)      // Cor de fundo do botão Voltar

#define COR_FUNDO_FASE1       RGB(0, 18, 51)
#define COR_FUNDO_FASE2       RGB(2, 62, 138)
#define COR_FUNDO_FASE3       RGB(94, 84, 142)
#define COR_FUNDO_FASE4       RGB(94, 84, 142)
#define COR_FUNDO_FASE5       RGB(94, 84, 142)

#define COR_BOTOES_FASE1   RGB(92, 103, 125)

#define COR_NOTAS_1_FASE1   RGB(116, 0, 184)
#define COR_NOTAS_2_FASE1   RGB(8, 124, 167)
#define COR_NOTAS_3_FASE1   RGB(97, 165, 194)  
#define COR_NOTAS_4_FASE1   RGB(4, 102, 200)

#define COR_BOTOES_FASE2   RGB(92, 103, 125)

#define COR_NOTAS_1_FASE2   RGB(237, 224, 212)
#define COR_NOTAS_2_FASE2   RGB(47, 14, 7)
#define COR_NOTAS_3_FASE2   RGB(133, 24, 42)  
#define COR_NOTAS_4_FASE2   RGB(237, 224, 212)

#define COR_BOTOES_FASE3   RGB(92, 103, 125)

#define COR_NOTAS_1_FASE3   RGB(237, 224, 212)
#define COR_NOTAS_2_FASE3   RGB(47, 14, 7)
#define COR_NOTAS_3_FASE3   RGB(133, 24, 42)  
#define COR_NOTAS_4_FASE3   RGB(237, 224, 212)






// Função para mudar as cores globais do projeto
void mudarCores(COLORREF corFundo, COLORREF corTextoBotao, COLORREF corFundoBotao);

#endif // CORES_H
