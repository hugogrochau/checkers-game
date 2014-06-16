#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "jogo.h"

#define TRUE 1
#define FALSE 0
#define TAM_MAX_NOME 128

int ContinuarJogo(JOGO_tppJogo jogo);

int main(void)
{
    JOGO_tppJogo jogo = NULL;
    char nomeJogador1[TAM_MAX_NOME], nomeJogador2[TAM_MAX_NOME];
    char opcao;

    system("cls");
    printf("~*~JOGO DE DAMAS~*~\n");

    do
    {
        printf("\nJogador 1, digite seu nome: ");
    }
    while(scanf("%s", nomeJogador1) != 1);

    do
    {
        printf("\nJogador 2, digite seu nome: ");
    }
    while(scanf("%s", nomeJogador2) != 1);

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
    scanf("%c",&opcao);
    return 0;
}

int ContinuarJogo(JOGO_tppJogo jogo)
{

    char linhaOrigem, linhaDestino, opcao;
    short int colunaOrigem, colunaDestino;
    JOGO_tpCondRet condRetJogada;

    JOGO_ImprimirTabuleiro(jogo);
    printf("%s, escolha seu movimento (ex: 3a 5b): ", jogo->jogadorDaVez->nome);
    scanf(" %hd%c %hd%c", &colunaOrigem, &linhaOrigem,
                        &colunaDestino, &linhaDestino);
    //system("cls");
    if (linhaOrigem == 'q' && linhaDestino == 'q')
    {
        printf("Obrigado por jogar");
        return FALSE;
    }

    if (linhaOrigem == 'd' && linhaDestino == 'd')
    {
        printf("%s venceu!", JOGO_ObterJogadorNaoDaVez(jogo)->nome);
        return FALSE;
    }

    if (linhaOrigem < 'a' || linhaOrigem > 'h' ||
        linhaDestino < 'a' || linhaDestino > 'h' ||
        colunaOrigem < 0 || colunaOrigem > 8 ||
        colunaDestino < 0 || colunaDestino > 8)
    {
        printf("Esse movimento nao e valido, tente novamente\n");
        return TRUE;
    }

    /* convertendo as letras das colunas para coordenadas */
    linhaOrigem = (char) tolower(linhaOrigem);
    linhaDestino = (char) tolower(linhaDestino);
    linhaOrigem -= 'a';
    linhaDestino -= 'a';
    colunaOrigem -= 1;
    colunaDestino -= 1;
    printf("destino.linha = %d\ndestino.coluna = %d\n",linhaDestino,colunaDestino);
    printf("origem.linha = %d\norigem.coluna = %d\n",linhaOrigem,colunaOrigem);
    condRetJogada = JOGO_ExecutarJogada(jogo,
                                        linhaOrigem, (short int) colunaOrigem,
                                        linhaDestino, (short int) colunaDestino);
    switch (condRetJogada)
    {
        case JOGO_CondRetJogadaInvalida:
            printf("Esse movimento nao e valido, tente novamente\n");
            return TRUE;
        case JOGO_CondRetJogadaValida:
            printf("Passou por aqui\n");
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
        default : printf("Nao executou nada\n");        
    }
    printf("Aperte qualquer tecla para sair :\n");
    scanf("%c",&opcao);
    printf("Ocorreu algum erro grave com jogo, comece novamente\n");
    return FALSE;
}
