#include <stdio.h>
#include "resourses/util.h"

// Função para ler o conteúdo de um arquivo e armazená-lo em um buffer
int lerArquivo(const char *caminho, char *buffer, int tamanho) {
    FILE *file = fopen(caminho, "r");
    if (file == NULL) {
        return 0;  // Retorna 0 se não conseguiu abrir o arquivo
    }
    fgets(buffer, tamanho, file);  // Lê o conteúdo do arquivo
    fclose(file);
    return 1;  // Sucesso
}

// Função para salvar dados em um arquivo
int salvarArquivo(const char *nomeArquivo, const char *conteudo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        return 0;  // Retorna 0 se houver um erro ao abrir o arquivo
    }

    fprintf(arquivo, "%s", conteudo);  // Escreve o conteúdo no arquivo
    fclose(arquivo);  // Fecha o arquivo
    return 1;  // Retorna 1 se a operação for bem-sucedida
}
