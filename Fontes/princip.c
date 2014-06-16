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
            printf("Obrigado por jogar");
            exit(1);
        }
        jogo = JOGO_CriarJogo(nomeJogador1, nomeJogador2);
    }

    printf("Jogo vai começar.\n"
           "Digite 0q 0q a qualquer momento para sair do programa\n"
           "Digite 0d 0d a qualquer momento para desistir\n"
           "Aperte qualquer tecla para continuar\n");
    getchar();
    while(ContinuarJogo(jogo));

    return 0;
}

int ContinuarJogo(JOGO_tppJogo jogo)
{

    short int linhaOrigem, linhaDestino;
    char colunaOrigem, colunaDestino;

    system("cls");
    JOGO_ImprimirTabuleiro(jogo);
    printf("%s, escolha seu movimento (ex: 3a 5b): ", jogo->jogadorDaVez->nome);
    scanf(" %hd%c %hd%c", &linhaOrigem, &colunaOrigem,
                          &linhaDestino, &colunaDestino);

    if (colunaOrigem == 'q' && colunaDestino == 'q')
    {
        printf("Obrigado por jogar");
        return FALSE;
    }

    if (colunaOrigem == 'd' && colunaDestino == 'd')
    {
        printf("%s venceu!", JOGO_ObterJogadorNaoDaVez(jogo)->nome);
        return FALSE;
    }

    /* convertendo as letras das colunas para coordenadas */
    colunaOrigem = (char) tolower(colunaOrigem);
    colunaDestino = (char) tolower(colunaDestino);
    colunaOrigem -= 'a';
    colunaDestino -= 'a';

    switch (JOGO_ExecutarJogada(jogo,
                                linhaOrigem, (short int) colunaOrigem,
                                linhaDestino, (short int) colunaDestino))
    {
        case JOGO_CondRetJogadaInvalida:
            printf("Esse movimento não é valido, tente novamente");
            return TRUE;
        case JOGO_CondRetJogoNaoInicializado:
            printf("Ocorreu algum erro grave com jogo, comece novamente");
            exit(1);
        case JOGO_CondRetJogador1Ganhou:
            printf("%s venceu!", jogo->jogador1->nome);
            return FALSE;
        case JOGO_CondRetJogador2Ganhou:
            printf("%s venceu!", jogo->jogador2->nome);
            return FALSE;
        case JOGO_CondRetEmpate:
            printf("O jogo terminou num empate!");
            return FALSE;
    }
    return TRUE;
}
