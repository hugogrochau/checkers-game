/***************************************************************************
*  $MCI Módulo de implementação: JOGO  gerenciador do jogo de damas
*
*  Arquivo gerado:              jogo.c
*  Letras identificadoras:      JOGO
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor     Data        Observações
*     1       hpg/gmm   09/jun/2014 início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa a lógica e funcionamento geral do jogo de damas.
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


/* Tipo referência para o jogo */
typedef JOGO_tpJogo * JOGO_tppJogo;

/***********************************************************************
*
*  $TC Tipo de dados: JOGO jogo
*
*
*  $ED Descrição do tipo
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

/* Tipo referência para um jogador */
typedef JOGO_tpJogador * JOGO_tppJogador;

/***********************************************************************
*
*  $TC Tipo de dados: JOGO jogador
*
*
*  $ED Descrição do tipo
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
    /* Número de jogadas */

} JOGO_tpJogador

/***********************************************************************
*
*  $TC Tipo de dados: JOGO condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções do jogo
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
    /* O jogo não foi inicializado */

} JOGO_tpCondRet

/***** Protótipo das funções encapsuladas no módulo *****/

/***********************************************************************
*
*  $FC Função: JOGO  -Inicializar jogo
*
*  $ED Descrição da função
*      Destroi por completo uma lista. Destroi os elementos e dps a cabeça da lista
*
***********************************************************************/

static JOGO_tppJogo JOGO_InicializarJogo (char *NomeJogador1, char *NomeJogador2);

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir jogo
*
*  $ED Descrição da função
*      Deleta o jogo, limpado o espaço de memória alocado para ele 
*      o tabuleiro, e os jogadores
*
*  $EP Parâmetros
*     jogo - referência para o tipo jogo a ser deletado.
*
***********************************************************************/

static void JOGO_DestruirJogo (JOGO_tppJogo jogo);

/***********************************************************************
*
*  $FC Função: JOGO  -Criar jogador
*
*  $ED Descrição da função
*      Cria um jogador, dando um nome e uma cor a ele(a).
*
*  $EP Parâmetros
*     Nome - String contendo o nome do jogador.
*     cor - A cor das peças do jogador (pretas ou brancas).
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o jogador.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL.
*     Não será dada mais informação quanto ao problema ocorrido.
*
***********************************************************************/

static JOGO_tppJogador JOGO_CriarJogador (char *Nome, PECA_tpCor cor);

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir jogador
*
*  $ED Descrição da função
*      Deleta um jogador, limpado o espaço de memória alocado para ele 
*      e seu nome.
*
*  $EP Parâmetros
*     jogador - referência para o tipo jogador a ser deletado.
*
***********************************************************************/

static void JOGO_DestruirJogador (JOGO_tppJogador jogador);


/***********************************************************************
*
*  $FC Função: JOGO  -Preencher tabuleiro
*
*  $ED Descrição da função
*      Carrega o tabuleiro de um arquivo e insere as peças no tabuleiro
*
*  $EP Parâmetros
*     jogo - Ponteiro para o tipo jogo.
*     arqTabuleiro - string contedo o path (diretorio/nome) do arquivo.
*
*  $FV Valor retornado
*     CondRetArquivoInvalido - Caso o arquivo não seja aberto ou não exista.
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





/* Definição de funções encapsuladas no módulo */

/***********************************************************************
*
*  $FC Função: JOGO  -Iniciar o jogo
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
*  $FC Função: JOGO  -Destruir Jogo
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
*  $FC Função: JOGO  -Criar jogador
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
*  $FC Função: JOGO  -Destruir Jogador
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
*  $FC Função: JOGO  -Preencher tabuleiro
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


/********** FIM DO MÓDULO DE IMPLEMENTAÇÃO: TAB  TABULEIRO DE DAMAS OU QUALQUER OUTRO JOGO **********/
