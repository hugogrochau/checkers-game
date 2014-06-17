#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "jogo.h"

#define TRUE 1
#define FALSE 0
#define TAM_MAX_NOME 128
#define VERMELHO 0
#define AZUL 1

int ContinuarJogo(JOGO_tppJogo jogo);

int main(void)
{
    JOGO_tppJogo jogo = NULL;
    char nomeJogador1[TAM_MAX_NOME], nomeJogador2[TAM_MAX_NOME];
    char opcao;

    system("cls");
    printf("~*~JOGO DE DAMAS~*~\n");

    printf("\nJogador 1, digite seu nome: ");

    fgets (nomeJogador1, TAM_MAX_NOME, stdin);
    if ((strlen(nomeJogador1)>0) && (nomeJogador1[strlen (nomeJogador1) - 1] == '\n'))
        nomeJogador1[strlen(nomeJogador1) - 1] = '\0';

    printf("\nJogador 2, digite seu nome: ");

    fgets (nomeJogador2, TAM_MAX_NOME, stdin);
    if ((strlen(nomeJogador2)>0) && (nomeJogador2[strlen (nomeJogador2) - 1] == '\n'))
        nomeJogador2[strlen(nomeJogador2) - 1] = '\0';

    jogo = JOGO_CriarJogo(nomeJogador1, nomeJogador2);
    while (jogo == NULL)
    {
        printf("Ocorreu algum erro a criar o jogo.\n"
               "Digite q para sair do programa\n"
               "Aperte qualquer outra tecla para tentar de novo\n");
        scanf("%c", &opcao);
        if (opcao == 'q')
        {
            printf("Obrigado por jogar\n");
            exit(1);
        }
        jogo = JOGO_CriarJogo(nomeJogador1, nomeJogador2);
    }

    printf("Jogo vai comecar.\n"
           "Digite 0q 0q a qualquer momento para sair do programa\n"
           "Digite 0d 0d a qualquer momento para desistir\n\n\n");
    while(ContinuarJogo(jogo));
    printf("Aperte qualquer tecla para sair :\n");
    _getch();
    return 0;
}

int ContinuarJogo(JOGO_tppJogo jogo)
{
    char linhaOrigem, linhaDestino;
    short int colunaOrigem, colunaDestino;
    JOGO_tpCondRet condRetJogada;

    JOGO_ImprimirTabuleiro(jogo);

    JOGO_ImprimirComCor(jogo->jogadorDaVez->nome, jogo->jogadorDaVez->cor, TRUE);
    printf(", escolha seu movimento (ex: 3a 5b): ");
    scanf(" %hd%c %hd%c", &colunaOrigem, &linhaOrigem,
                          &colunaDestino, &linhaDestino);
    system("cls");
    if (linhaOrigem == 'q' && linhaDestino == 'q')
    {
        printf("Obrigado por jogar");
        return FALSE;
    }

    if (linhaOrigem == 'd' && linhaDestino == 'd')
    {
        JOGO_ImprimirComCor(JOGO_ObterJogadorNaoDaVez(jogo)->nome, JOGO_ObterJogadorNaoDaVez(jogo)->cor, TRUE);
        printf(" venceu!\n");
        return FALSE;
    }

    if (linhaOrigem < 'a' || linhaOrigem > 'h' ||
        linhaDestino < 'a' || linhaDestino > 'h' ||
        colunaOrigem < 0 || colunaOrigem > 8 ||
        colunaDestino < 0 || colunaDestino > 8)
    {
        JOGO_ImprimirComCor("Esse movimento nao e valido, tente novamente\n", VERMELHO, TRUE);
        return TRUE;
    }

    /* convertendo as letras das colunas para coordenadas */
    linhaOrigem = (char) tolower(linhaOrigem);
    linhaOrigem -= 'a';
    colunaOrigem -= 1;
    linhaDestino = (char) tolower(linhaDestino);
    linhaDestino -= 'a';
    colunaDestino -= 1;

    printf("destino.linha = %d\ndestino.coluna = %d\n",linhaDestino,colunaDestino);
    printf("origem.linha = %d\norigem.coluna = %d\n",linhaOrigem,colunaOrigem);
    condRetJogada = JOGO_ExecutarJogada(jogo,
                                        linhaOrigem, (short int) colunaOrigem,
                                        linhaDestino, (short int) colunaDestino);
    switch (condRetJogada)
    {
        case JOGO_CondRetJogadaInvalida:
            printf("Jogada inválida, tente novamente\n");
            return TRUE;
        case JOGO_CondRetJogadaValida:
            return TRUE;
        case JOGO_CondRetJogoNaoInicializado:
            printf("Ocorreu algum erro grave com jogo, comece novamente\n");
            return FALSE;
        case JOGO_CondRetJogador1Ganhou:
            printf("%s venceu!\n", jogo->jogador1->nome);
            return FALSE;
        case JOGO_CondRetJogador2Ganhou:
            printf("%s venceu!\n", jogo->jogador2->nome);
            return FALSE;
        case JOGO_CondRetEmpate:
            printf("O jogo terminou num empate!\n");
            return FALSE;
    }
    printf("Ocorreu algum erro grave com jogo, comece novamente\n");
    return FALSE;
}