/***************************************************************************
*  $MCI M�dulo de implementa��o: JOGO  gerenciador do jogo de damas
*
*  Arquivo gerado:              jogo.c
*  Letras identificadoras:      JOGO
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor     Data        Observa��es
*     1       hpg/gmm   09/jun/2014 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa a l�gica e funcionamento geral do jogo de damas.
*
***************************************************************************/


#include <stdio.h>
#include "tabuleiro.h"
#include "peca.h"

#define jogo_OWN
#include "jogo.h"
#undef jogo_OWN

#define TRUE  1
#define FALSE 0
#define PATH_TAB_INICIAL "Objetos/TabuleiroInicial.tab"
#define LINHAS 8
#define COLUNAS 8


/* Tipo refer�ncia para o jogo */
typedef JOGO_tpJogo * JOGO_tppJogo;

/***********************************************************************
*
*  $TC Tipo de dados: JOGO jogo
*
*
*  $ED Descri��o do tipo
*     Dados sobre o jogo de damas
*
***********************************************************************/

typedef struct JOGO_tagJogo
{

    TAB_tppTabuleiro tabuleiro ;
    /* Tabuleiro do jogo */

    JOGO_tppJogador jogador1;
    /* Primeiro jogador */

    JOGO_tppJogador jogador2;
    /* Segundo jogador */

} JOGO_tpJogo ;

/* Tipo refer�ncia para um jogador */
typedef JOGO_tpJogador * JOGO_tppJogador;

/***********************************************************************
*
*  $TC Tipo de dados: JOGO jogador
*
*
*  $ED Descri��o do tipo
*     Dados de um dos jogadores
*
***********************************************************************/

typedef struct JOGO_tagJogador
{
    char *nome ;
    /* Nome do jogador */

    PECA_tpCor cor ;
    /* Cor do jogador */

    unsigned int jogadas ;
    /* N�mero de jogadas */

} JOGO_tpJogador

/***********************************************************************
*
*  $TC Tipo de dados: JOGO condi��es de retorno
*
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es do jogo
*
***********************************************************************/

typedef struct
{

    JOGO_CondRetOk,
    /* Concluiu corretamente */

    JOGO_CondRetArquivoInvalido,
    /* Ocorreu erro a ler o arquivo */

    JOGO_CondRetArquivoCorrompido,
    /* Ocorreu erro a interpretar o arquivo */

    JOGO_CondRetJogoNaoInicializado
    /* O jogo n�o foi inicializado */

} JOGO_tpCondRet

/***** Prot�tipo das fun��es encapsuladas no m�dulo *****/

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Inicializar jogo
*
*  $ED Descri��o da fun��o
*      Destroi por completo uma lista. Destroi os elementos e dps a cabe�a da lista
*
***********************************************************************/

static JOGO_tppJogo JOGO_InicializarJogo (char *NomeJogador1, char *NomeJogador2);

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Destruir jogo
*
*  $ED Descri��o da fun��o
*      Deleta o jogo, limpado o espa�o de mem�ria alocado para ele 
*      o tabuleiro, e os jogadores
*
*  $EP Par�metros
*     jogo - refer�ncia para o tipo jogo a ser deletado.
*
***********************************************************************/

static void JOGO_DestruirJogo (JOGO_tppJogo jogo);

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Criar jogador
*
*  $ED Descri��o da fun��o
*      Cria um jogador, dando um nome e uma cor a ele(a).
*
*  $EP Par�metros
*     Nome - String contendo o nome do jogador.
*     cor - A cor das pe�as do jogador (pretas ou brancas).
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o jogador.
*
*     Se ocorreu algum erro, por exemplo falta de mem�ria ou dados errados,
*     a fun��o retornar� NULL.
*     N�o ser� dada mais informa��o quanto ao problema ocorrido.
*
***********************************************************************/

static JOGO_tppJogador JOGO_CriarJogador (char *Nome, PECA_tpCor cor);

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Destruir jogador
*
*  $ED Descri��o da fun��o
*      Deleta um jogador, limpado o espa�o de mem�ria alocado para ele 
*      e seu nome.
*
*  $EP Par�metros
*     jogador - refer�ncia para o tipo jogador a ser deletado.
*
***********************************************************************/

static void JOGO_DestruirJogador (JOGO_tppJogador jogador);


/***********************************************************************
*
*  $FC Fun��o: JOGO  -Preencher tabuleiro
*
*  $ED Descri��o da fun��o
*      Carrega o tabuleiro de um arquivo e insere as pe�as no tabuleiro
*
*  $EP Par�metros
*     jogo - Ponteiro para o tipo jogo.
*     arqTabuleiro - string contedo o path (diretorio/nome) do arquivo.
*
*  $FV Valor retornado
*     CondRetArquivoInvalido - Caso o arquivo n�o seja aberto ou n�o exista.
*     CondRetJogoNaoInicializado - O jogo ainda nao foi inicializado
*     CondRetArquivoCorrompido - Dados invalidos
*     CondRetOk - Tabuleiro foi preenchido com sucesso
*
***********************************************************************/

static JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro );

int main(void)
{
    printf("------JOGO DE DAMAS--------")

}





/* Defini��o de fun��es encapsuladas no m�dulo */

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Iniciar o jogo
*
***********************************************************************/

static JOGO_tppJogo JOGO_InicializarJogo (char *NomeJogador1, char *NomeJogador2) {
    JOGO_tppJogo jogo;
    JOGO_tppJogador jogador1, jogador2;

    jogo = (JOGO_tppJogo) malloc(sizeof(JOGO_tpJogo));
    if (jogo == NULL)
    {
        return NULL;
    }
    jogo->jogador1 = JOGO_CriarJogador(NomeJogador1, PECA_CorBranca);
    jogo->jogador2 = JOGO_CriarJogador(NomeJogador2, PECA_CorPreta);
    if (jogo->jogador1 == NULL || jogo->jogador2 == NULL)
    {
        JOGO_DestruirJogo(jogo);
        return NULL;
    }

    jogo->tabuleiro = TAB_CriarTabuleiro(COLUNAS, LINHAS, PECA_DestruirPeca);
    if (jogo->tabuleiro == NULL)
    {
        JOGO_DestruirJogo(jogo);
        return NULL;
    }

    JOGO_PreencherTabuleiro(jogo, fopen(PATH_TAB_INICIAL, "r"));
}

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Destruir Jogo
*
***********************************************************************/

static void JOGO_DestruirJogo (JOGO_tppJogo jogo) {
    if (jogo != NULL)
    {
        TAB_DestruirTabuleiro(jogo->tabuleiro);
        jogo->tabuleiro = NULL;
        JOGO_DestruirJogador(jogo->jogador1);
        jogo->jogador1 = NULL;
        JOGO_DestruirJogador(jogo->jogador2);
        jogo->jogador2 = NULL;
        free(jogo)
        jogo = NULL;
    }
}

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Criar jogador
*
***********************************************************************/

static JOGO_tppJogador JOGO_CriarJogador (char *Nome, PECA_tpCor cor)
{
    JOGO_tppJogador jog = (JOGO_tppJogador) malloc (sizeof(JOGO_tppJogador));
    size_t strsize = 0;
    if (jog == NULL)
        return NULL;

    strsize = strlen(Nome);
    jog->nome = (char *) malloc (strsize);

    if (jog->nome == NULL)
    {
        free(jog);
        return NULL;
    }

    strcpy(jog->nome, Nome);
    jog->cor = cor;
    jog->jogadas = 0;
    return jog;
}

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Destruir Jogador
*
***********************************************************************/

static void JOGO_DestruirJogador (JOGO_tppJogador jogador) {
    if (jogador != NULL)
    {
        free(jogador->nome);
        jogador->nome = NULL;
        free(jogador);
        jogador = NULL;
    }
}

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Preencher tabuleiro
*
***********************************************************************/

static JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro ) {
    int i, j;
    PECA_tpStatus status;
    PECA_tpCor cor;
    PECA_tppPeca peca;
    TAB_tpPosicao pos;
    TAB_tpCondRed tabCondRet;

    if (fp == NULL)
    {
        return JOGO_CondRetArquivoInvalido;
    }

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            if (fscanf(fp, " %d %d ", &status, &cor) == EOF)
            {
                return JOGO_CondRetArquivoCorrompido;
            }
            pos.linha = i;
            pos.coluna = j;
            peca = PECA_CriarPeca(cor);
            if (status == PECA_StatusDama)
            {
                PECA_VirarDama(peca);
            }
            if (TAB_IncluirPeca(jogo->tabuleiro, peca, pos) == TAB_CondRetPosicaoInvalida)
            {
                return JOGO_CondRetArquivoCorrompido;
            }
        }
    }
    return JOGO_CondRetOk;
}


/********** FIM DO M�DULO DE IMPLEMENTA��O: TAB  TABULEIRO DE DAMAS OU QUALQUER OUTRO JOGO **********/
