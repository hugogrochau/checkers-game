#if !defined( PECA_ )
#define PECA_

/***************************************************************************
*
*  $MCD Módulo de definição: PECA  Peça de tabuleiro de damas
*
*  Arquivo gerado:              peca.h
*  Letras identificadoras:      PECA
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     2       hpg   21/apr/2014 melhorando a documentação
*     1       hpg   31/mar/2014 início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa uma peça de tabuleiro de damas.
*     Cada peça contém uma cor (preta ou branca) e um status (normal ou dama)
*
***************************************************************************/

#if defined( PECA_OWN )
#define PECA_EXT
#else
#define PECA_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma peça */

typedef struct PECA_tagPeca *PECA_tppPeca;

/***********************************************************************
*
*  $TC Tipo de dados: PECA cores
*
*
*  $ED Descrição do tipo
*     A cor da peça (preto ou branco)
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
*  $ED Descrição do tipo
*     O status da peça (dama ou normal)
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
*  $TC Tipo de dados: PECA Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções da peça
*
***********************************************************************/

typedef enum
{
    PECA_CondRetOK = 0,
    /* Concluiu corretamente */

    PECA_CondRetEhDama = 1,
    /* A peça já é uma dama */

    PECA_CondRetNaoExiste = 2
    /* A peça ou foi deletada ou criada incorretamente */
} PECA_tpCondRet;

/***********************************************************************
*
*  $FC Função: PECA &Criar Peça
*
*  $ED Descrição da função
*     Cria uma peça com a cor especificada.
*
*  $EP Parâmetros
*     cor - A cor desejada.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para a peça.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL.
*
***********************************************************************/

PECA_tppPeca PECA_CriarPeca( PECA_tpCor cor );

/***********************************************************************
*
*  $FC Função: PECA &Obter Cor
*
*  $ED Descrição da função
*     Retorna a cor de uma peça.
*
*  $EP Parâmetros
*     pPeca - O ponteiro para a peça cuja cor você quer receber.
*
*  $FV Valor retornado
*     A cor da peça (PECA_CorBranca ou PECA_CorPreta) ou -1 (PECA_CorInvalida) se a peça não existir.
*
***********************************************************************/

PECA_tpCor PECA_ObterCor( PECA_tppPeca pPeca );

/***********************************************************************
*
*  $FC Função: PECA &Obter Status
*
*  $ED Descrição da função
*     Retorna o status de uma peça.
*
*  $EP Parâmetros
*     pPeca - O ponteiro para a peça cujo status você quer receber.
*
*  $FV Valor retornado
*     O status da peça (PECA_StatusNormal ou PECA_StatusDama) ou -1 (PECA_CorInvalida) se a peça não existir.
*
***********************************************************************/

PECA_tpStatus PECA_ObterStatus( PECA_tppPeca pPeca );

/***********************************************************************
*
*  $FC Função: PECA &Virar Dama
*
*  $ED Descrição da função
*     Transforma uma peça numa dama.
*
*  $EP Parâmetros
*     pPeca - O ponteiro para a peça que deve virar uma dama.
*
*  $FV Valor retornado
*   PECA_CondRetOK se a peça virou uma dama
*   PECA_CondRetEhDama se a peça já era uma dama
*   PECA_CondRetNaoExiste se a peça não existe
*
***********************************************************************/

PECA_tpCondRet PECA_VirarDama( PECA_tppPeca pPeca );

/***********************************************************************
*
*  $FC Função: PECA &Destruir Peça
*
*  $ED Descrição da função
*     Destroi uma peça da memória.
*
*  $EP Parâmetros
*     pPeca - O ponteiro para a peça que deve ser destruída.
*
***********************************************************************/

void PECA_DestruirPeca( PECA_tppPeca pPeca );

#undef PECA_EXT

/********** Fim do módulo de definição: PECA  Peça de tabuleiro de damas **********/

#else
#endif
