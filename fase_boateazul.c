#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "resourses\cabecalho.h"
#include <mmsystem.h>

#define WINDOW_TITLE "Jogo de Ritmo em C"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define HITBOX_TOLERANCIA 20

int numNotas = 0;
int pontuacao = 0;
int erros;
BarraTecla barrasTeclas[4]; // Declara��o do array de barras
Nota *listaNotas; // Cabe�a da lista encadeada de notas
Nota notas[10];

void salvarPontuacao(int novaPontuacao) {
    const char *arquivoPontuacao = "resourses/pontuacao.txt"; // Caminho do arquivo
    int pontuacaoAnterior = 0;

    // Tenta abrir o arquivo para leitura
    FILE *arquivo = fopen(arquivoPontuacao, "r");
    if (arquivo != NULL) {
        fscanf(arquivo, "%d", &pontuacaoAnterior); // L� a pontua��o anterior
        fclose(arquivo);
    }

    // Soma a nova pontua��o � pontua��o anterior
    int pontuacaoFinal = pontuacaoAnterior + novaPontuacao;

    // Salva a pontua��o final no arquivo
    arquivo = fopen(arquivoPontuacao, "w");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d", pontuacaoFinal);
        fclose(arquivo);
    } else {
        printf("Erro ao salvar a pontua��o.\n"); // Tratamento de erro
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GerarNota(int tecla);
void MoverNotas(int tempoAtual);
void DesenharNotas(HDC hdc);
void DesenharBarrasTeclas(HDC hdc);
void DesenharPontuacao(HDC hdc);
COLORREF GerarCorPorColuna(int coluna);
void VerificarAcerto(WPARAM wParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "RythmGameWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    if (hwnd == NULL) {
        return;
    }

    SetTimer(hwnd, 1, 1, NULL); // A cada 1ms

    ShowWindow(hwnd, SW_SHOW);

    // música
    PlaySound("assets/boateazul.wav", NULL, SND_FILENAME | SND_ASYNC | 1);

    MSG msg = {0};
    int tempoAtual = 0;  // Variável para o tempo do jogo em ms
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Incrementa o tempo do jogo
        tempoAtual += 1;
    }
}

void finalizarFase1(HWND hwnd) {
    char mensagem[100];
    sprintf(mensagem, "Fim da fase!\nPontuacao final: %d\nErros: %d", pontuacao, erros);
    MessageBox(NULL, mensagem, "Fim da Fase", MB_OK | MB_ICONINFORMATION);

    // Aqui voc� pode chamar a fun��o que cria o menu principal novamente
    salvarPontuacao(pontuacao);
    // Fecha a janela da Fase 1
    DestroyWindow(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);


}
void ReiniciarFase() {
    pontuacao = 0; // Reinicia a pontua��o
    erros = 0; // Reinicia os erros
    listaNotas = NULL; // Limpa a lista de notas
    numNotas = 0; // Reseta o n�mero de notas

    // Reinicia os eventos
    for (int i = 0; i < numeroEventos2; i++) {
 eventos2[i].jaCriada = 0; // Reseta o estado de cria��o dos eventos
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int tempoAtual = 0;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(169, 169, 169))); 

                DesenharBarrasTeclas(hdc);
                DesenharNotas(hdc);
                DesenharPontuacao(hdc);

                EndPaint(hwnd, &ps);
            }
            return 0;

        case WM_KEYDOWN:
            VerificarAcerto(wParam); 
            break;

        case WM_TIMER:
            tempoAtual += 30; 
            MoverNotas(tempoAtual); 
            InvalidateRect(hwnd, NULL, TRUE); 
            break;

        // Adicione um caso para finalizar a fase
        case WM_CLOSE:
            finalizarFase1(hwnd); // Chama a fun��o de finaliza��o ao fechar
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AdicionarNota(int tecla) {
    Nota *novaNota = (Nota *)malloc(sizeof(Nota));
    if (novaNota == NULL) return; // Verifica��o de erro de aloca��o

    int x = 0;
    switch (tecla) {
        case 1: x = 150; break;
        case 2: x = 300; break;
        case 3: x = 450; break;
        case 4: x = 600; break;
    }
    novaNota->x = x;
    novaNota->y = 0;
    novaNota->tecla = tecla;
    novaNota->cor = GerarCorPorColuna(tecla - 1);
    novaNota->proxima = NULL;

    // Adiciona ao final da lista
    if (listaNotas == NULL) {
        listaNotas = novaNota;
    } else {
        Nota *temp = listaNotas;
        while (temp->proxima != NULL) {
            temp = temp->proxima;
        }
        temp->proxima = novaNota;
    }
}

void GerarNota(int tecla) {
    if (numNotas < 10) {
        int x = 0;
        switch (tecla) {
            case 1: x = 150; break;
            case 2: x = 300; break;
            case 3: x = 450; break;
            case 4: x = 600; break;
        }
        notas[numNotas].x = x;
        notas[numNotas].y = 0;
        notas[numNotas].tecla = tecla;
        notas[numNotas].cor = GerarCorPorColuna(tecla - 1);
        numNotas++;
    }
}

erros = 0; // Adicione uma vari�vel global para erros

bool jogoEncerrado1 = false; // Vari�vel de controle para o estado do jogo

void MoverNotas(int tempoAtual) {
    // Gerar notas de acordo com o tempo predefinido
    for (int i = 0; i < numeroEventos2; i++) {
        if (eventos2[i].tempo <= tempoAtual && eventos2[i].jaCriada == 0) {
            AdicionarNota(eventos2[i].tecla);
            eventos2[i].jaCriada = 1;
        }
    }

    // Mover e remover notas
    Nota *atual = listaNotas;
    Nota *anterior = NULL;

    while (atual != NULL) {
        atual->y += 5; // Move a nota para baixo

        if (atual->y > WINDOW_HEIGHT) {
            // Contabiliza um erro se a nota passou da tela
            erros++;

            // Remover a nota da lista
            if (anterior == NULL) {
                listaNotas = atual->proxima;
            } else {
                anterior->proxima = atual->proxima;
            }

            // Libera a mem�ria da nota removida
            Nota *remover = atual;
            atual = atual->proxima;
            free(remover);
        } else {
            anterior = atual;
            atual = atual->proxima;
        }
    }

    // Verifica se a soma de erros e pontua��o � 539
    if (!jogoEncerrado1 && pontuacao + erros == 305) {
        jogoEncerrado1 = true; // Marca o jogo como encerrado
        finalizarFase1(GetActiveWindow());
    }
}


void DesenharNotas(HDC hdc) {
    Nota *atual = listaNotas;
    while (atual != NULL) {
        HBRUSH brush = CreateSolidBrush(atual->cor);
        SelectObject(hdc, brush);
        Ellipse(hdc, atual->x - 20, atual->y - 20, atual->x + 20, atual->y + 20);
        DeleteObject(brush);

        atual = atual->proxima;
    }
}


void DesenharBarrasTeclas(HDC hdc) {
    int barraHeight = 40;
    int yPosition = WINDOW_HEIGHT - barraHeight - 100;
    int totalBarWidth = 176 * 4;
    int startX = (WINDOW_WIDTH - totalBarWidth) / 2;

    for (int i = 0; i < 4; i++) {
        int x = startX + i * 150;
        int raio = 20 * 1.5;
        barrasTeclas[i].x = x;
        barrasTeclas[i].y = yPosition;
        barrasTeclas[i].raio = raio;
        barrasTeclas[i].hitbox.left = x - raio;
        barrasTeclas[i].hitbox.top = yPosition - raio;
        barrasTeclas[i].hitbox.right = x + raio;
        barrasTeclas[i].hitbox.bottom = yPosition + raio;

        HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, brush);
        Ellipse(hdc, x - raio, yPosition - raio, x + raio, yPosition + raio);
        DeleteObject(brush);
    }
}

void DesenharPontuacao(HDC hdc) {
    char pontuacaoTexto[50];
    sprintf(pontuacaoTexto, "Pontos: %d | Erros: %d", pontuacao, erros);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, WINDOW_WIDTH - 250, 50, pontuacaoTexto, strlen(pontuacaoTexto));
}

COLORREF GerarCorPorColuna(int coluna) {
    switch (coluna) {
        case 0: // Primeira coluna
            return RGB(255, 255, 0); // Amarelo
        case 1: // Segunda coluna
            return RGB(128, 0, 128); // Roxo
        case 2: // Terceira coluna
            return RGB(0, 0, 0);     // Preto
        case 3: // Quarta coluna
            return RGB(0, 0, 255);   // Azul
        default:
            return RGB(255, 255, 255); // Branco (ou outra cor padr�o)
    }
}

void VerificarAcerto(WPARAM wParam) {
    int tecla = 0;
    if (wParam == 0x44) tecla = 1;  // 'D'
    if (wParam == 0x46) tecla = 2;  // 'F'
    if (wParam == 0x4A) tecla = 3;  // 'J'
    if (wParam == 0x4B) tecla = 4;  // 'K'

    Nota *atual = listaNotas;
    Nota *anterior = NULL;

    while (atual != NULL) {
        if (atual->tecla == tecla) {
            RECT hitbox = barrasTeclas[tecla - 1].hitbox;
            if (atual->y >= hitbox.top - HITBOX_TOLERANCIA &&
                atual->y <= hitbox.bottom + HITBOX_TOLERANCIA) {
                pontuacao++;

                if (anterior == NULL) {
                    listaNotas = atual->proxima;
                } else {
                    anterior->proxima = atual->proxima;
                }

                Nota *remover = atual;
                atual = atual->proxima;
                free(remover);
                return;
            }
        }

        anterior = atual;
        atual = atual->proxima;
    }
}
