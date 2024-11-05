#include <stdio.h>
#include <conio.h> 

void exibirTelaInicial() 
{
    printf("=== Bem-vindo ao Jogo do Juninho! ===\n\n");
    printf("        1. Jogar\n\n");
    printf("        2. Ver Pontuacao\n\n");
    printf("        3. Sair\n\n");
}

void jogar() 
{
    printf("Iniciando o jogo...\n");
}

void verPontuacao() 
{
    printf("Exibindo a pontuacao...\n");
}

void telaInicial() 
{
    char escolha;

    while (1) 
    {  
        exibirTelaInicial();
        escolha = getch();  // Lê o caractere sem precisar de Enter

        switch (escolha) 
        {
            case '1':
                jogar();
                break;
            case '2':
                verPontuacao();
                break;
            case '3':
                printf("Saindo do jogo...\n");
                return 0; 
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        printf("\n");  
    }
    return 0;
}
