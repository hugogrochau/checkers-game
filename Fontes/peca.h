#if !defined( PECA_ )
#define PECA_

/***************************************************************************
*
*  $MCD M�dulo de defini��o: PECA  Pe�a de tabuleiro de damas
*
*  Arquivo gerado:              peca.h
*  Letras identificadoras:      PECA
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Hugo Pedrotti Grochau (hpg)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     2       hpg   21/apr/2014 melhorando a documenta��o
*     1       hpg   31/mar/2014 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa uma pe�a de tabuleiro de damas.
*     Cada pe�a cont�m uma cor (preta ou branca) e um status (normal ou dama)
*
***************************************************************************/

#if defined( PECA_OWN )
#define PECA_EXT
#else
#define PECA_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para uma pe�a */

typedef struct PECA_tagPeca *PECA_tppPeca;

/***********************************************************************
*
*  $TC Tipo de dados: PECA cores
*
*
*  $ED Descri��o do tipo
*     A cor da pe�a (preto ou branco)
*
***********************************************************************/

typedef enum
{
    PECA_CorInvalida = -1,
    PECA_CorPreta = 0,
    PECA_CorBranca = 1
} PECA_tpCor;

/***********************************************************************
*
*  $TC Tipo de dados: PECA status
*
*
*  $ED Descri��o do tipo
*     O status da pe�a (dama ou normal)
*
***********************************************************************/

typedef enum
{
    PECA_StatusInvalido = -1,
    PECA_StatusNormal = 0,
    PECA_StatusDama = 1
} PECA_tpStatus;

/***********************************************************************
*
*  $TC Tipo de dados: PECA Condi��es de retorno
*
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es da pe�a
*
***********************************************************************/

typedef enum
{
    PECA_CondRetOK = 0,
    /* Concluiu corretamente */

    PECA_CondRetEhDama = 1,
    /* A pe�a j� � uma dama */

    PECA_CondRetNaoExiste = 2
    /* A pe�a ou foi deletada ou criada incorretamente */
} PECA_tpCondRet;

/***********************************************************************
*
*  $FC Fun��o: PECA &Criar Pe�a
*
*  $ED Descri��o da fun��o
*     Cria uma pe�a com a cor especificada.
*
*  $EP Par�metros
*     cor - A cor desejada.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para a pe�a.
*
*     Se ocorreu algum erro, por exemplo falta de mem�ria ou dados errados,
*     a fun��o retornar� NULL.
*
***********************************************************************/

PECA_tppPeca PECA_CriarPeca( PECA_tpCor cor );

/***********************************************************************
*
*  $FC Fun��o: PECA &Obter Cor
*
*  $ED Descri��o da fun��o
*     Retorna a cor de uma pe�a.
*
*  $EP Par�metros
*     pPeca - O ponteiro para a pe�a cuja cor voc� quer receber.
*
*  $FV Valor retornado
*     A cor da pe�a (PECA_CorBranca ou PECA_CorPreta) ou -1 (PECA_CorInvalida) se a pe�a n�o existir.
*
***********************************************************************/

PECA_tpCor PECA_ObterCor( PECA_tppPeca pPeca );

/***********************************************************************
*
*  $FC Fun��o: PECA &Obter Status
*
*  $ED Descri��o da fun��o
*     Retorna o status de uma pe�a.
*
*  $EP Par�metros
*     pPeca - O ponteiro para a pe�a cujo status voc� quer receber.
*
*  $FV Valor retornado
*     O status da pe�a (PECA_StatusNormal ou PECA_StatusDama) ou -1 (PECA_CorInvalida) se a pe�a n�o existir.
*
***********************************************************************/

PECA_tpStatus PECA_ObterStatus( PECA_tppPeca pPeca );

/***********************************************************************
*
*  $FC Fun��o: PECA &Virar Dama
*
*  $ED Descri��o da fun��o
*     Transforma uma pe�a numa dama.
*
*  $EP Par�metros
*     pPeca - O ponteiro para a pe�a que deve virar uma dama.
*
*  $FV Valor retornado
*   PECA_CondRetOK se a pe�a virou uma dama
*   PECA_CondRetEhDama se a pe�a j� era uma dama
*   PECA_CondRetNaoExiste se a pe�a n�o existe
*
***********************************************************************/

PECA_tpCondRet PECA_VirarDama( PECA_tppPeca pPeca );

/***********************************************************************
*
*  $FC Fun��o: PECA &Destruir Pe�a
*
*  $ED Descri��o da fun��o
*     Destroi uma pe�a da mem�ria.
*
*  $EP Par�metros
*     pPeca - O ponteiro para a pe�a que deve ser destru�da.
*
***********************************************************************/

void PECA_DestruirPeca( PECA_tppPeca pPeca );

#undef PECA_EXT

/********** Fim do m�dulo de defini��o: PECA  Pe�a de tabuleiro de damas **********/

#else
#endif
