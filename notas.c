#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <time.h>

#define NUM_LINHAS 20
#define NUM_COLUNAS 4

// Função para inicializar a matriz com espaços
void inicializarMatriz(char a[NUM_LINHAS][NUM_COLUNAS]) {
    int i, j;
    for (i = 0; i < NUM_LINHAS; i++) {
        for (j = 0; j < NUM_COLUNAS; j++) {
            a[i][j] = ' '; // Inicializa com espaços
        }
    }
}

// Função para imprimir a matriz
void imprimirMatriz(char a[NUM_LINHAS][NUM_COLUNAS]) {
    int i, j;
    for (i = 0; i < NUM_LINHAS; i++) {
        for (j = 0; j < NUM_COLUNAS; j++) {
            printf("%c  ", a[i][j]); // Imprime cada caractere com espaçamento
        }
        printf("\n");
    }
}

// Função para gerar uma nota aleatória ou um espaço nas colunas fixas
void gerarNotas(char a[NUM_LINHAS][NUM_COLUNAS]) {
    // Definindo as notas estáticas para cada coluna
    a[0][0] = '<';  // Coluna 0 sempre começa com '<'
    a[0][1] = 'a';  // Coluna 1 sempre começa com 'a'
    a[0][2] = 'b';  // Coluna 2 sempre começa com 'b'
    a[0][3] = '>';  // Coluna 3 sempre começa com '>'

    // Podemos aleatoriamente gerar espaços, mantendo as notas nas suas colunas fixas
    for (int j = 0; j < NUM_COLUNAS; j++) {
        if (rand() % 3 == 0) {  // 1 de cada 3 vezes, coloca um espaço
            a[0][j] = ' ';
        }
    }
}

void matriz_notas () {
    char a[NUM_LINHAS][NUM_COLUNAS];
    int i, j;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa a matriz com espaços
    inicializarMatriz(a);

    // Gera notas estáticas para as 4 colunas, na linha 0
    gerarNotas(a);

    while (1) {
        // Limpa a tela (no Windows, substitua "clear" por "cls")
        system("cls");

        // Desloca as notas para baixo
        for (j = 0; j < NUM_COLUNAS; j++) {
            // Desce todas as notas da coluna, exceto a última linha
            for (i = NUM_LINHAS - 1; i > 0; i--) {
                a[i][j] = a[i - 1][j];  // Desloca a nota para a linha abaixo
            }
        }

        // Gera novas notas nas posições estáticas (linha 0)
        gerarNotas(a);

        // Imprime a matriz
        imprimirMatriz(a);

        // Atraso para o movimento das notas (0.3 segundos)
        Sleep(300); // 300 milissegundos = 0.3 segundos
    }
}
