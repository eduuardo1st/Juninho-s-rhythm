#ifndef CABECALHO_H
#define CABECALHO_H

typedef struct {
    long tempo;    // Tempo em ms para a nota surgir
    int tecla;    // Qual tecla a nota representa
    int jaCriada; // Flag para indicar se a nota já foi gerada
} EventoNota;

extern EventoNota eventos[];
extern int numeroEventos;

#endif
