/***************************************************************************
*  $MCI Módulo de implementação: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              lista.c
*  Letras identificadoras:      LIS
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs, Hugo Pedrotti Grochau (hpg), Gustavo Marques Martins (gmm)
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     6       gmm   19/abr/2014 implementação da função alterar valor corrente e deletar valor elemento corrente
*     5       hpg   31/mar/2014 implementação da função de inserção ordenada
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#define LISTA_OWN
#include "lista.h"
#undef LISTA_OWN

#ifdef _DEBUG
    #include "generico.h"
    #include "cespdin.h"
#endif


/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

typedef struct tagElemLista
{

    void *pValor ;
    /* Ponteiro para o valor contido no elemento */

    struct tagElemLista *pAnt ;
    /* Ponteiro para o elemento predecessor */

    struct tagElemLista *pProx ;
    /* Ponteiro para o elemento sucessor */

} tpElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeça de lista
*
*
***********************************************************************/

typedef struct LIS_tagLista
{

    tpElemLista *pOrigemLista ;
    /* Ponteiro para a origem da lista */

    tpElemLista *pFimLista ;
    /* Ponteiro para o final da lista */

    tpElemLista *pElemCorr ;
    /* Ponteiro para o elemento corrente da lista */

    int numElem ;
    /* Número de elementos da lista */

    void ( * ExcluirValor ) ( void *pValor ) ;
    /* Ponteiro para a função de destruição do valor contido em um elemento */

} LIS_tpLista ;

/***** Protótipos das funções encapuladas no módulo *****/

static void LiberarElemento( LIS_tppLista   pLista ,
                             tpElemLista   *pElem   ) ;

static tpElemLista *CriarElemento( LIS_tppLista pLista ,
                                   void        *pValor  ) ;

static void LimparCabeca( LIS_tppLista pLista ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LIS  &Criar lista
*  ****/

LIS_tppLista LIS_CriarLista(
    void   ( * ExcluirValor ) ( void *pDado ) )
{

    LIS_tpLista *pLista = NULL ;

    pLista = ( LIS_tpLista *) malloc( sizeof( LIS_tpLista )) ;
    if ( pLista == NULL )
    {
        return NULL ;
    } /* if */

    LimparCabeca( pLista ) ;

    pLista->ExcluirValor = ExcluirValor ;

    return pLista ;

} /* Fim função: LIS  &Criar lista */

/***************************************************************************
*
*  Função: LIS  &Destruir lista
*  ****/

void LIS_DestruirLista( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    LIS_EsvaziarLista( pLista ) ;

    free( pLista ) ;

} /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  Função: LIS  &Esvaziar lista
*  ****/

void LIS_EsvaziarLista( LIS_tppLista pLista )
{

    tpElemLista *pElem ;
    tpElemLista *pProx ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    pElem = pLista->pOrigemLista ;
    while ( pElem != NULL )
    {
        pProx = pElem->pProx ;
        LiberarElemento( pLista , pElem ) ;
        pElem = pProx ;
    } /* while */

    LimparCabeca( pLista ) ;

} /* Fim função: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento ordenado
*  ****/

LIS_tpCondRet LIS_InserirElementoOrdenado( LIS_tppLista pLista ,
        void *pValor,
        int (*pComp) (void *a, void *b))
{
    tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    /* Criar elemento a inserir antes */
    pElem = CriarElemento( pLista , pValor ) ;
    if ( pElem == NULL )
    {
        return LIS_CondRetFaltouMemoria ;
    } /* if */
    /* Ir para o começo da lista */
    IrInicioLista (pLista);
    /* Percorrer a lista até encontrar um valor superior ou igual */
    while (LIS_AvancarElementoCorrente(pLista, 1) != LIS_CondRetFimLista)
    {
        if (pComp(LIS_ObterValor(pLista), pValor) > 0)
        {
            break ;
        } /* if */
    } /* while */
    /* Encadear o elemento antes do elemento superior */
    return LIS_InserirElementoAntes(pLista, pValor);

} /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento antes
*  ****/

LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                        void *pValor        )
{
    tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    /* Criar elemento a inserir antes */

    pElem = CriarElemento( pLista , pValor ) ;
    if ( pElem == NULL )
    {
        return LIS_CondRetFaltouMemoria ;
    } /* if */

    /* Encadear o elemento antes do elemento corrente */

    if ( pLista->pElemCorr == NULL )
    {
        pLista->pOrigemLista = pElem ;
        pLista->pFimLista = pElem ;
    }
    else
    {
        if ( pLista->pElemCorr->pAnt != NULL )
        {
            pElem->pAnt  = pLista->pElemCorr->pAnt ;
            pLista->pElemCorr->pAnt->pProx = pElem ;
        }
        else
        {
            pLista->pOrigemLista = pElem ;
        } /* if */

        pElem->pProx = pLista->pElemCorr ;
        pLista->pElemCorr->pAnt = pElem ;
    } /* if */

    pLista->pElemCorr = pElem ;

    return LIS_CondRetOK ;

} /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento após
*  ****/

LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                       void *pValor        )
{

    tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    /* Criar elemento a inserir após */

    pElem = CriarElemento( pLista , pValor ) ;
    if ( pElem == NULL )
    {
        return LIS_CondRetFaltouMemoria ;
    } /* if */

    /* Encadear o elemento após o elemento */

    if ( pLista->pElemCorr == NULL )
    {
        pLista->pOrigemLista = pElem ;
        pLista->pFimLista = pElem ;
    }
    else
    {
        if ( pLista->pElemCorr->pProx != NULL )
        {
            pElem->pProx  = pLista->pElemCorr->pProx ;
            pLista->pElemCorr->pProx->pAnt = pElem ;
        }
        else
        {
            pLista->pFimLista = pElem ;
        } /* if */

        pElem->pAnt = pLista->pElemCorr ;
        pLista->pElemCorr->pProx = pElem ;

    } /* if */

    pLista->pElemCorr = pElem ;

    return LIS_CondRetOK ;

} /* Fim função: LIS  &Inserir elemento após */

/***************************************************************************
*
*  Função: LIS  &Excluir elemento
*  ****/

LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
{

    tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista  != NULL ) ;
#endif

    if ( pLista->pElemCorr == NULL )
    {
        return LIS_CondRetListaVazia ;
    } /* if */

    pElem = pLista->pElemCorr ;

    /* Desencadeia à esquerda */

    if ( pElem->pAnt != NULL )
    {
        pElem->pAnt->pProx   = pElem->pProx ;
        pLista->pElemCorr    = pElem->pAnt ;
    }
    else
    {
        pLista->pElemCorr    = pElem->pProx ;
        pLista->pOrigemLista = pLista->pElemCorr ;
    } /* if */

    /* Desencadeia à direita */

    if ( pElem->pProx != NULL )
    {
        pElem->pProx->pAnt = pElem->pAnt ;
    }
    else
    {
        pLista->pFimLista = pElem->pAnt ;
    } /* if */

    LiberarElemento( pLista , pElem ) ;

    return LIS_CondRetOK ;

} /* Fim função: LIS  &Excluir elemento */

/***************************************************************************
*
*  Função: LIS  &Excluir valor elemento corrente
*  ****/

LIS_tpCondRet LIS_ExcluirValorElementoCorrente ( LIS_tppLista pLista )
{
    if (pLista == NULL)
        return LIS_CondRetListaVazia;
    pLista->ExcluirValor(pLista->pElemCorr->pValor);
    pLista->pElemCorr->pValor = NULL;
    return LIS_CondRetOK;
} /* Fim função : LIS &Excluir valor elemento corrente */

/***************************************************************************
*
*  Função: LIS  &Obter referência para o valor contido no elemento
*  ****/

void *LIS_ObterValor( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    if ( pLista->pElemCorr == NULL )
    {
        return NULL ;
    } /* if */

    return pLista->pElemCorr->pValor ;

} /* Fim função: LIS  &Obter referência para o valor contido no elemento */

/***************************************************************************
*
*  Função: LIS  &Sobrescrever valor do elemento corrente
*  ****/

void LIS_SobrescreverValorCorrente ( LIS_tppLista pLista, void *pDado )
{
    if (pLista != NULL)
		pLista->pElemCorr->pValor = pDado;
}

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento inicial
*  ****/

void IrInicioLista( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    pLista->pElemCorr = pLista->pOrigemLista ;

} /* Fim função: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento final
*  ****/

void IrFinalLista( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    pLista->pElemCorr = pLista->pFimLista ;

} /* Fim função: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Função: LIS  &Avançar elemento
*  ****/

LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
        int numElem )
{

    int i ;

    tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    /* Tratar lista vazia */

    if ( pLista->pElemCorr == NULL )
    {

        return LIS_CondRetListaVazia ;

    } /* fim ativa: Tratar lista vazia */

    /* Tratar avançar para frente */

    if ( numElem > 0 )
    {

        pElem = pLista->pElemCorr ;
        for ( i = numElem ; i > 0 ; i-- )
        {
            if ( pElem == NULL )
            {
                break ;
            } /* if */
            pElem    = pElem->pProx ;
        } /* for */

        if ( pElem != NULL )
        {
            pLista->pElemCorr = pElem ;
            return LIS_CondRetOK ;
        } /* if */

        pLista->pElemCorr = pLista->pFimLista ;
        return LIS_CondRetFimLista ;

    } /* fim ativa: Tratar avançar para frente */

    /* Tratar avançar para trás */

    else if ( numElem < 0 )
    {

        pElem = pLista->pElemCorr ;
        for ( i = numElem ; i < 0 ; i++ )
        {
            if ( pElem == NULL )
            {
                break ;
            } /* if */
            pElem    = pElem->pAnt ;
        } /* for */

        if ( pElem != NULL )
        {
            pLista->pElemCorr = pElem ;
            return LIS_CondRetOK ;
        } /* if */

        pLista->pElemCorr = pLista->pOrigemLista ;
        return LIS_CondRetFimLista ;

    } /* fim ativa: Tratar avançar para trás */

    /* Tratar não avançar */

    return LIS_CondRetOK ;

} /* Fim função: LIS  &Avançar elemento */

/***************************************************************************
*
*  Função: LIS  &Procurar elemento contendo valor
*  ****/

LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista ,
                                 void *pValor        )
{

    tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista  != NULL ) ;
#endif

    if ( pLista->pElemCorr == NULL )
    {
        return LIS_CondRetListaVazia ;
    } /* if */

    for ( pElem  = pLista->pElemCorr ;
            pElem != NULL ;
            pElem  = pElem->pProx )
    {
        if ( pElem->pValor == pValor )
        {
            pLista->pElemCorr = pElem ;
            return LIS_CondRetOK ;
        } /* if */
    } /* for */

    return LIS_CondRetNaoAchou ;

} /* Fim função: LIS  &Procurar elemento contendo valor */


/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: LIS  -Liberar elemento da lista
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

void LiberarElemento( LIS_tppLista   pLista ,
                      tpElemLista   *pElem   )
{

    if ( ( pLista->ExcluirValor != NULL )
            && ( pElem->pValor != NULL        ))
    {
        pLista->ExcluirValor( pElem->pValor ) ;
    } /* if */

    free( pElem ) ;

    pLista->numElem-- ;

} /* Fim função: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Função: LIS  -Criar o elemento
*
***********************************************************************/

tpElemLista *CriarElemento( LIS_tppLista pLista ,
                            void        *pValor  )
{

    tpElemLista *pElem ;

    pElem = ( tpElemLista *) malloc( sizeof( tpElemLista )) ;

    if ( pElem == NULL )
    {
        return NULL ;
    } /* if */

    pElem->pValor = pValor ;
    pElem->pAnt   = NULL  ;
    pElem->pProx  = NULL  ;

    pLista->numElem ++ ;

    return pElem ;

} /* Fim função: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Função: LIS  -Limpar a cabeça da lista
*
***********************************************************************/

void LimparCabeca( LIS_tppLista pLista )
{

    pLista->pOrigemLista = NULL ;
    pLista->pFimLista = NULL ;
    pLista->pElemCorr = NULL ;
    pLista->numElem   = 0 ;

} /* Fim função: LIS  -Limpar a cabeça da lista */

/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/

