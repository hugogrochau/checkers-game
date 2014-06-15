#include <jogo.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

int main(int n, char **args)
{
    JOGO_tppJogo jogo = NULL;
    char nomeJogador1[LINE_MAX], nomeJogador2[LINE_MAX];
    char opcao;

    printf("~*~JOGO DE DAMAS~*~\n");

    do
    {
        printf("\nJogador2, digite seu nome: \n");
    }
    while(fgets(nomeJogador1, sizeof(nomeBuffer), stdin) == NULL);

    do
    {
        printf("\nJogador2, digite seu nome: \n");
    }
    while(fgets(nomeJogador2, sizeof(nomeBuffer), stdin) == NULL);

    jogo = JOGO_CriarJogo(nomeJogador1, nomeJogador2);
    while (jogo == NULL)
    {
        printf("Ocorreu algum erro a criar o jogo.\n"
               "Digite q para sair do programa\n");
               "Aperte qualquer outra tecla para tentar de novo\n"
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
           "Digite 0d 0d a qualquer momento para desistir\n");

    while(ContinuarJogo(jogo));

    return 0;
}

int ContinuarJogo(JOGO_tppJogo jogo)
{
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
        printf("%s venceu!", ObterJogadorNaoDaVez(jogo)->nome);
        return FALSE;
    }

    /* convertendo as letras das colunas para coordenadas */
    tolower(colunaOrigem);
    tolower(colunaDestino);
    colunaOrigem -= 'a';
    colunaDestino -= 'a';

    switch (JOGO_ExecutarJogada(jogo,
                                linhaOrigem, (short int) colunaOrigem,
                                linhaDestino, (short int) colunaDestino))
    {
        case TAB_CondRetPosicaoInvalida:
            printf("Esse movimento não é valido, tente novamente");
            return TRUE;
        case TAB_CondRetTabuleiroVazio:
            printf("Ocorreu algum erro grave com jogo, comece novamente");
            exit(1);
        case TAB_CondRetJogador1Ganhou:
            printf("%s venceu!", jogo->jogador1->nome);
            return FALSE;
        case TAB_CondRetJogador2Ganhou:
            printf("%s venceu!", jogo->jogador2->nome);
            return FALSE;
        case TAB_CondRetEmpate:
            prinf("O jogo terminou num empate!");
            return FALSE;
    }
    return TRUE;
}
