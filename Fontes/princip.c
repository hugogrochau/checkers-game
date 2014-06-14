#include <jogo.h>
#include <stdio.h>
#include <string.h>
#define PATH_TAB_INICIAL "Objetos/TabuleiroInicial.tab"

int main(int n, char **args)
{
	JOGO_tppJogo jogo = JOGO_CriarJogo();
	int linhaOrigem, linhaDestino;
	char colunaOrigem, colunaDestino;
	
	if (jogo == NULL)
	{
		printf("Ocorreu algum erro a criar o jogo, comece novamente");
		exit(1);
	}

	while(jogada());
	
	return 0;
}

int jogada()
{
	printf(" Escolha sua jogada (ex: 3a 5b): ");
    scanf(" %hd%c %hd%c", &linhaOrigem, &colunaOrigem,
                        &linhaDestino,  &colunaDestino);

    /* convertendo as letras das colunas coordenadas */
    tolower(colunaOrigem);
    tolower(colunaDestino);
    colunaOrigem -= 'a'; 
    colunaDestino -= 'a';
	switch (JOGO_ExecutarJogada(jogo, linhaOrigem, (short int) colunaOrigem, linhaDestino, (short int) colunaDestino))
	{
		case TAB_CondRetPosicaoInvalida:
			printf("Esse movimento não é valido, tente novamente");
			return TRUE;
		case TAB_CondRetTabuleiroVazio:
			printf("Ocorreu algum erro a criar o jogo, comece novamente");
			return FALSE;
		case TAB_CondRetJogoAcabou:
			printf("Esse movimento não é valido, tente novamente");
			return FALSE;
	}
}

