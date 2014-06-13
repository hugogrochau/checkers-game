#include <jogo.h>
#include <stdio.h>
#define PATH_TAB_INICIAL "Objetos/TabuleiroInicial.tab"

int main(int n, char **args)
{
	JOGO_tppJogo jogo = JOGO_CriarJogo("Hugo", "Gustavo");
	
	if (jogo == NULL)
	{
		printf("Ouve algum erro a criar o jogo, por favor, tente de novo");
		exit(1);
	}
}