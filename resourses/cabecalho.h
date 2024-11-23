#ifndef CABECALHO_H
#define CABECALHO_H
#include <stdbool.h>
#include <windows.h>

typedef struct {
    long tempo;    // Tempo em ms para a nota surgir
    int tecla;    // Qual tecla a nota representa
    int jaCriada; // Flag para indicar se a nota j� foi gerada
} EventoNota;

extern EventoNota eventos[];
extern EventoNota eventos1[];
extern EventoNota eventos2[];
extern EventoNota eventos3[];
extern EventoNota eventos4[];
extern int numeroEventos;
extern int numeroEventos1;
extern int numeroEventos2;
extern int numeroEventos3;
extern int numeroEventos4;

typedef struct Nota {
    int x, y;
    int tecla;
    COLORREF cor;
    struct Nota *proxima; // Usar struct Nota aqui
} Nota;

typedef struct {
    int x, y;
    int raio;
    RECT hitbox;
} BarraTecla;
void DesenharNotas(HDC hdc);
void DesenharBarrasTeclas(HDC hdc);
void DesenharPontuacao(HDC hdc);
COLORREF GerarCorPorColuna(int coluna);
void VerificarAcerto(WPARAM wParam);

void DesenharNotas1(HDC hdc);
void DesenharBarrasTeclas1(HDC hdc);
void DesenharPontuacao1(HDC hdc);
COLORREF GerarCorPorColuna1(int coluna);
void VerificarAcerto1(WPARAM wParam);

extern int tecla[4]; // Declare como um array de inteiros

void salvarPontuacao(int novaPontuacao);

#endif
