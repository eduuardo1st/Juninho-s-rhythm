# Explicação do Jogo de Ritmo *Juninho Tiles*

Este projeto foi desenvolvido utilizando **linguagem C** e bibliotecas nativas para Windows.

---

## **Comandos Padrão**
Os comandos padrão para jogar são as teclas:  
`D`, `F`, `J`, `K`.

No entanto, é possível personalizar essas teclas. As configurações permitem alterar os comandos para **qualquer tecla em maiúsculo ou número**.  
**Não são permitidos caracteres especiais ou letras minúsculas.**

### Para modificar as teclas:
1. Inicie o jogo.  
2. Vá para a seção **Configurações**.  
3. Ajuste os comandos conforme preferir.

---

## **Recompilação de Fases**
Se você alterar algo em alguma fase, será necessário recompilá-la individualmente para que as mudanças entrem em vigor.  

### Comandos para recompilar fases:
```
gcc -o fase_boateazul.exe resourses/script_boateazul.c fase_boateazul.c -mwindows -lgdi32 -lwinmm
gcc -o fase_aquipensando.exe resourses/script_aquipensando.c fase_aquipensando.c -mwindows -lgdi32 -lwinmm
gcc -o fase_giorno.exe resourses/script_giorno.c fase_giorno.c -mwindows -lgdi32 -lwinmm
gcc -o fase_megalovania.exe resourses/script_megalovania.c fase_megalovania.c -mwindows -lgdi32 -lwinmm
gcc -o fase_piratas.exe resourses/script_piratas.c fase_piratas.c -mwindows -lgdi32 -lwinmm
gcc -o FIM.exe BOSS.c -mwindows -lgdi32 -lwinmm -lgdiplus
```
## **Compilação do Menu Principal**
O menu principal também precisa ser compilado antes de executar o jogo.

### Comando para compilar o menu principal:
```bash
gcc configuracao.c jogar.c main.c menu.c pontuacao.c util.c -o Menu.exe -mwindows -lgdi32 -lwinmm
```
## **Execução do Jogo**
Após a compilação, para iniciar o jogo:

1. Abra o terminal.  
2. Digite o nome do arquivo correspondente ao menu principal ou à fase que você compilou.

### Exemplo para executar o menu principal:
```bash
Menu.exe
