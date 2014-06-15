#include <jogo.h>
#include <stdio.h>
#include <string.h>

#define TAM_MAX_NOME 256
#define TRUE 1
#define FALSE 0

int main(int n, char **args)
{
    JOGO_tppJogo jogo = NULL;
    char nomeJogador1[TAM_MAX_NOME], nomeJogador2[TAM_MAX_NOME];
    char opcao;

    printf("~*~JOGO DE DAMAS~*~\n");

    ReceberNome(nomeJogador1, 1);
    ReceberNome(nomeJogador2, 2);

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

    printf("Jogo vai começar, digite 0q 0q a qualquer momento para sair do programa\n");

    while(Jogada(jogo));

    return 0;
}

int Jogada(JOGO_tppJogo jogo)
{
    printf(" Escolha sua jogada (ex: 3a 5b): ");
    scanf(" %hd%c %hd%c", &linhaOrigem, &colunaOrigem,
                          &linhaDestino, &colunaDestino);

    if (colunaOrigem == 'q' && colunaDestino == 'q')
    {
        printf("Obrigado por jogar");
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
            printf("Ocorreu algum erro a criar o jogo, comece novamente");
            return FALSE;
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
}

void ReceberNome(char *nomeBuffer, int jogador)
{
    size_t tam = 0;
    while(!ValidaNome(nomeBuffer, tam))
    {
        if (strlen(nomeBuffer) > 0)
        {
            printf("Nome inválido, ");
        }
        printf("\nJogador%d, digite seu nome: ", jogador);
        tam = scanf("%s", nomeBuffer);
    }
}

int ValidaNome(char *nome, size_t tam)
{
    if (tam > TAM_MAX_NOME)
    {
        return FALSE;
    }
}
