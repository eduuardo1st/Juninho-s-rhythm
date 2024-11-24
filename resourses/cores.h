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
#define COR_FUNDO_FASE3       RGB(0, 114, 0)
#define COR_FUNDO_FASE4       RGB(36, 0, 70)
#define COR_FUNDO_FASE5       RGB(5, 5, 15)
#define COR_FUNDO_BOSS       RGB(0, 0, 0)

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

#define COR_NOTAS_1_FASE3   RGB(253, 197, 0)
#define COR_NOTAS_2_FASE3   RGB(248, 249, 250)
#define COR_NOTAS_3_FASE3   RGB(3, 4, 94)  
#define COR_NOTAS_4_FASE3   RGB(253, 197, 0)

#define COR_BOTOES_FASE4   RGB(92, 103, 125)

#define COR_NOTAS_1_FASE4   RGB(255, 255, 0)
#define COR_NOTAS_2_FASE4   RGB(128, 0, 128)
#define COR_NOTAS_3_FASE4   RGB(199, 125, 255)  
#define COR_NOTAS_4_FASE4   RGB(0, 0, 255)

#define COR_BOTOES_FASE5   RGB(92, 103, 125)

#define COR_NOTAS_1_FASE5   RGB(0, 166, 251)
#define COR_NOTAS_2_FASE5   RGB(0, 100, 148)
#define COR_NOTAS_3_FASE5   RGB(237, 242, 251)  
#define COR_NOTAS_4_FASE5   RGB(5, 130, 202)

#define COR_BOTOES_BOSS   RGB(0, 40, 85)

#define COR_NOTAS_1_BOSS   RGB(250,250 ,250 )
#define COR_NOTAS_2_BOSS   RGB(0, 180, 216 )
#define COR_NOTAS_3_BOSS   RGB(250,250 ,250 )  
#define COR_NOTAS_4_BOSS   RGB(0, 180, 216 )





// Função para mudar as cores globais do projeto
void mudarCores(COLORREF corFundo, COLORREF corTextoBotao, COLORREF corFundoBotao);

#endif // CORES_H
