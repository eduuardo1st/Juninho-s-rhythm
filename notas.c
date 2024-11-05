#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <time.h>

#define NUM_LINHAS 20
#define NUM_COLUNAS 4

// Fun��o para inicializar a matriz com espa�os
void inicializarMatriz(char a[NUM_LINHAS][NUM_COLUNAS]) {
    int i, j;
    for (i = 0; i < NUM_LINHAS; i++) {
        for (j = 0; j < NUM_COLUNAS; j++) {
            a[i][j] = ' '; // Inicializa com espa�os
        }
    }
}

// Fun��o para imprimir a matriz
void imprimirMatriz(char a[NUM_LINHAS][NUM_COLUNAS]) {
    int i, j;
    for (i = 0; i < NUM_LINHAS; i++) {
        for (j = 0; j < NUM_COLUNAS; j++) {
            printf("%c  ", a[i][j]); // Imprime cada caractere com espa�amento
        }
        printf("\n");
    }
}

// Fun��o para gerar uma nota aleat�ria ou um espa�o nas colunas fixas
void gerarNotas(char a[NUM_LINHAS][NUM_COLUNAS]) {
    // Definindo as notas est�ticas para cada coluna
    a[0][0] = '<';  // Coluna 0 sempre come�a com '<'
    a[0][1] = 'a';  // Coluna 1 sempre come�a com 'a'
    a[0][2] = 'b';  // Coluna 2 sempre come�a com 'b'
    a[0][3] = '>';  // Coluna 3 sempre come�a com '>'

    // Podemos aleatoriamente gerar espa�os, mantendo as notas nas suas colunas fixas
    for (int j = 0; j < NUM_COLUNAS; j++) {
        if (rand() % 3 == 0) {  // 1 de cada 3 vezes, coloca um espa�o
            a[0][j] = ' ';
        }
    }
}

void matriz_notas () {
    char a[NUM_LINHAS][NUM_COLUNAS];
    int i, j;

    // Inicializa o gerador de n�meros aleat�rios
    srand(time(NULL));

    // Inicializa a matriz com espa�os
    inicializarMatriz(a);

    // Gera notas est�ticas para as 4 colunas, na linha 0
    gerarNotas(a);

    while (1) {
        // Limpa a tela (no Windows, substitua "clear" por "cls")
        system("cls");

        // Desloca as notas para baixo
        for (j = 0; j < NUM_COLUNAS; j++) {
            // Desce todas as notas da coluna, exceto a �ltima linha
            for (i = NUM_LINHAS - 1; i > 0; i--) {
                a[i][j] = a[i - 1][j];  // Desloca a nota para a linha abaixo
            }
        }

        // Gera novas notas nas posi��es est�ticas (linha 0)
        gerarNotas(a);

        // Imprime a matriz
        imprimirMatriz(a);

        // Atraso para o movimento das notas (0.3 segundos)
        Sleep(300); // 300 milissegundos = 0.3 segundos
    }
}
