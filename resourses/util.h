#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <windows.h>

// Função para exibir uma MessageBox genérica
void exibirMensagem(const char *mensagem);

// Funções para manipulação de arquivos
void lerPontuacao(char *buffer, int tamanho);
void salvarPontuacao(int pontuacao);
int lerArquivo(const char *caminho, char *buffer, int tamanho);
int salvarArquivo(const char *caminho, const char *dados);


#endif
