/***************************************************************************
*  $MCI M�dulo de implementa��o: PRNC M�dulo principal
*
*  Arquivo gerado:              princip.c
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs, Hugo Pedrotti Grochau (hpg), Gustavo Marques Martins (gmm)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data         Observa��es
*     1       hpg      16/jun/2014  Desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Este m�dulo cont�m o programa principal do jogo de damas
*
***************************************************************************/

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

/***********************************************************************
*
*  $FC Fun��o: PRNC  - Continuar Jogo
*
*  $ED Descri��o da fun��o
*     Executa o pr�ximo passo do jogo
*
*  $EP Par�metros
*     jogo - o jogo que est� sendo jogado.
*
*  $FV Valor retornado
*     se o jogo � para continuar: TRUE, se n�o: FALSE.
*
***********************************************************************/

int ContinuarJogo(JOGO_tppJogo jogo);

/***********************************************************************
*
*  $FC Fun��o: PRNC &Programa principal
*
*  $ED Descri��o da fun��o
*
*     O programa principal do jogo de damas que lida com o input do
*     usu�rio e administra o jogo
*
*
***********************************************************************/

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

/***********************************************************************
*
*  $FC Fun��o: PRNC  -Continuar Jogo
*
***********************************************************************/

int ContinuarJogo(JOGO_tppJogo jogo)
{
    char linhaOrigem, linhaDestino;
    short int colunaOrigem, colunaDestino;
    JOGO_tpCondRet condRetJogada;

    JOGO_ImprimirTabuleiro(jogo);

    JOGO_ImprimirComCor(jogo->jogadorDaVez->nome, jogo->jogadorDaVez->cor, TRUE);
    printf(", escolha seu movimento (ex: 1f 2e): ");
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

    condRetJogada = JOGO_ExecutarJogada(jogo,
                                        linhaOrigem, (short int) colunaOrigem,
                                        linhaDestino, (short int) colunaDestino);
    switch (condRetJogada)
    {
        case JOGO_CondRetJogadaInvalida:
            printf("Jogada inv�lida, tente novamente\n");
            return TRUE;
        case JOGO_CondRetJogadaValida:
            return TRUE;
        case JOGO_CondRetJogoNaoInicializado:
            printf("Ocorreu algum erro grave com jogo, comece novamente\n");
            return FALSE;
        case JOGO_CondRetJogador1Ganhou:
            JOGO_ImprimirComCor(jogo->jogador1->nome, jogo->jogador1->cor, TRUE);
            printf(" venceu!\n");
            return FALSE;
        case JOGO_CondRetJogador2Ganhou:
            JOGO_ImprimirComCor(jogo->jogador2->nome, jogo->jogador2->cor, TRUE);
            printf(" venceu!\n");
            return FALSE;
        case JOGO_CondRetEmpate:
            printf("O jogo terminou num empate!\n");
            return FALSE;
    }
    printf("Ocorreu algum erro grave com jogo, comece novamente\n");
    return FALSE;
}