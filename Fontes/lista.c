
/***************************************************************************
*  $MCI Módulo de implementacao: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              lista.c
*  Letras identificadoras:      LIS
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs, Hugo Pedrotti Grochau (hpg), Gustavo Marques Martins (gmm)
*
*  $HA Histórico de evolucao:
*     Versao  Autor    Data     Observacões
*     7       hpg   16/jun/2014 auto-verificacao do tabuleiro
*     6       gmm   19/abr/2014 implementacao da funcao alterar valor corrente e deletar valor elemento corrente
*     5       hpg   31/mar/2014 implementacao da funcao de insercao ordenada
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformizacao dos exemplos
*     2       avs   07/jul/2003 unificacao de todos os módulos em um só projeto
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
    #include "conta.h"
    #include "..\\tabelas\\TiposEspacosTabuleiro.def"
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

#ifdef _DEBUG
    void *pCabecaTabuleiro ;
#endif

} LIS_tpElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeca de lista
*
*
***********************************************************************/

typedef struct LIS_tagLista
{

    LIS_tpElemLista *pOrigemLista ;
    /* Ponteiro para a origem da lista */

    LIS_tpElemLista *pFimLista ;
    /* Ponteiro para o final da lista */

    LIS_tpElemLista *pElemCorr ;
    /* Ponteiro para o elemento corrente da lista */

    int numElem ;
    /* Número de elementos da lista */

    void ( * ExcluirValor ) ( void *pValor ) ;
    /* Ponteiro para a funcao de destruicao do valor contido em um elemento */

} LIS_tpLista ;

/***** Protótipos das funcões encapuladas no módulo *****/

static void LiberarElemento( LIS_tppLista   pLista ,
                             LIS_tpElemLista   *pElem   ) ;

static LIS_tpElemLista *CriarElemento( LIS_tppLista pLista ,
                                   void        *pValor  ) ;

static void LimparCabeca( LIS_tppLista pLista ) ;

/*****  Código das funcões exportadas pelo módulo  *****/

/***************************************************************************
*
*  Funcao: LIS  &Colocar Cabeca
*  ****/

#ifdef _DEBUG

void LIS_ColocarCabecaTabuleiro(LIS_tppLista lis, void* pCabeca, int tipoEspaco)
{   
    lis->pElemCorr->pCabecaTabuleiro = pCabeca;
    CED_DefinirTipoEspaco(lis->pElemCorr, tipoEspaco);
}

#endif

/***************************************************************************
*
*  Funcao: LIS  &Criar lista
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

} /* Fim funcao: LIS  &Criar lista */

/***************************************************************************
*
*  Funcao: LIS  &Destruir lista
*  ****/

void LIS_DestruirLista( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    LIS_EsvaziarLista( pLista ) ;

    free( pLista ) ;

} /* Fim funcao: LIS  &Destruir lista */

/***************************************************************************
*
*  Funcao: LIS  &Esvaziar lista
*  ****/

void LIS_EsvaziarLista( LIS_tppLista pLista )
{

    LIS_tpElemLista *pElem ;
    LIS_tpElemLista *pProx ;

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

} /* Fim funcao: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Funcao: LIS  &Inserir elemento ordenado
*  ****/

LIS_tpCondRet LIS_InserirElementoOrdenado( LIS_tppLista pLista ,
        void *pValor,
        int (*pComp) (void *a, void *b))
{
    LIS_tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    /* Criar elemento a inserir antes */
    pElem = CriarElemento( pLista , pValor ) ;
    if ( pElem == NULL )
    {
        return LIS_CondRetFaltouMemoria ;
    } /* if */
    /* Ir para o comeco da lista */
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

} /* Fim funcao: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Funcao: LIS  &Inserir elemento antes
*  ****/

LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                        void *pValor        )
{
    LIS_tpElemLista *pElem ;

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

} /* Fim funcao: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Funcao: LIS  &Inserir elemento após
*  ****/

LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                       void *pValor        )
{

    LIS_tpElemLista *pElem ;

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

} /* Fim funcao: LIS  &Inserir elemento após */

/***************************************************************************
*
*  Funcao: LIS  &Excluir elemento
*  ****/

LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
{

    LIS_tpElemLista *pElem ;

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

} /* Fim funcao: LIS  &Excluir elemento */

/***************************************************************************
*
*  Funcao: LIS  &Excluir valor elemento corrente
*  ****/

LIS_tpCondRet LIS_ExcluirValorElementoCorrente ( LIS_tppLista pLista )
{
    if (pLista == NULL)
        return LIS_CondRetListaVazia;
    pLista->ExcluirValor(pLista->pElemCorr->pValor);
    pLista->pElemCorr->pValor = NULL;
    return LIS_CondRetOK;
} /* Fim funcao : LIS &Excluir valor elemento corrente */

/***************************************************************************
*
*  Funcao: LIS  &Obter referência para o valor contido no elemento
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

} /* Fim funcao: LIS  &Obter referência para o valor contido no elemento */

/***************************************************************************
*
*  Funcao: LIS  &Sobrescrever valor do elemento corrente
*  ****/

void LIS_SobrescreverValorCorrente ( LIS_tppLista pLista, void *pDado )
{
    if (pLista != NULL)
		pLista->pElemCorr->pValor = pDado;
} /* Fim funcao: LIS  &Sobrescrever valor do elemento corrente */

/***************************************************************************
*
*  Funcao: LIS  &Ir para o elemento inicial
*  ****/

void IrInicioLista( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    pLista->pElemCorr = pLista->pOrigemLista ;

} /* Fim funcao: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Funcao: LIS  &Ir para o elemento final
*  ****/

void IrFinalLista( LIS_tppLista pLista )
{

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    pLista->pElemCorr = pLista->pFimLista ;

} /* Fim funcao: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Funcao: LIS  &Avancar elemento
*  ****/

LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
        int numElem )
{

    int i ;

    LIS_tpElemLista *pElem ;

#ifdef _DEBUG
    assert( pLista != NULL ) ;
#endif

    /* Tratar lista vazia */

    if ( pLista->pElemCorr == NULL )
    {

        return LIS_CondRetListaVazia ;

    } /* fim ativa: Tratar lista vazia */

    /* Tratar avancar para frente */

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

    } /* fim ativa: Tratar avancar para frente */

    /* Tratar avancar para tras */

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

    } /* fim ativa: Tratar avancar para tras */

    /* Tratar nao avancar */

    return LIS_CondRetOK ;

} /* Fim funcao: LIS  &Avancar elemento */

/***************************************************************************
*
*  Funcao: LIS  &Procurar elemento contendo valor
*  ****/

LIS_tpCondRet LIS_ProcurarValor( LIS_tppLista pLista ,
                                 void *pValor        )
{

    LIS_tpElemLista *pElem ;

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

} /* Fim funcao: LIS  &Procurar elemento contendo valor */


/*****  Código das funcões encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Funcao: LIS  &Liberar elemento da lista
*
*  $ED Descricao da funcao
*     Elimina os espacos apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

void LiberarElemento( LIS_tppLista   pLista ,
                      LIS_tpElemLista   *pElem   )
{

    if ( ( pLista->ExcluirValor != NULL )
            && ( pElem->pValor != NULL        ))
    {
        pLista->ExcluirValor( pElem->pValor ) ;
    } /* if */

    free( pElem ) ;

    pLista->numElem-- ;

} /* Fim funcao: LIS  &Liberar elemento da lista */


/***********************************************************************
*
*  $FC Funcao: LIS  &Criar o elemento
*
***********************************************************************/

LIS_tpElemLista *CriarElemento( LIS_tppLista pLista ,
                            void        *pValor  )
{

    LIS_tpElemLista *pElem ;

    pElem = ( LIS_tpElemLista *) malloc( sizeof( LIS_tpElemLista )) ;

    if ( pElem == NULL )
    {
        return NULL ;
    } /* if */

    pElem->pValor = pValor ;
    pElem->pAnt   = NULL  ;
    pElem->pProx  = NULL  ;

    pLista->numElem ++ ;

    return pElem ;

} /* Fim funcao: LIS  &Criar o elemento */


/***********************************************************************
*
*  $FC Funcao: LIS  &Limpar a cabeca da lista
*
***********************************************************************/

void LimparCabeca( LIS_tppLista pLista )
{

    pLista->pOrigemLista = NULL ;
    pLista->pFimLista = NULL ;
    pLista->pElemCorr = NULL ;
    pLista->numElem   = 0 ;

} /* Fim funcao: LIS  &Limpar a cabeca da lista */


#ifdef _DEBUG

/***********************************************************************
*
*  Funcao: TAB &Verificar elemento
*
***********************************************************************/

LIS_tpCondRet LIS_VerificarElemento(LIS_tppLista lista) 
{
    LIS_tpElemLista *elem = lista->pElemCorr;
    CNT_CONTAR("Verificar elemento");

    if (elem == NULL)
    {
        CNT_CONTAR("Elemento nulo");

        TST_NotificarFalha("Tentou verificar elemento nulo.");
        return LIS_CondRetErroEstrutura;
    }

    CNT_CONTAR("Elemento nao nulo");

    if (TST_CompararInt(4, /*TAB_TipoEspacoElemento*/
                        CED_ObterTipoEspaco(elem),
                        "Tipo do espaco de dados nao é um elemento.") != TST_CondRetOK )
    {

        CNT_CONTAR("Tipo elemento invalido");

        return LIS_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo elemento valido");

    if (elem->pProx == NULL && elem->pAnt == NULL)
    {
        
        CNT_CONTAR("Elemento solto");

        TST_NotificarFalha("Elemento esta solto");
        return LIS_CondRetErroEstrutura;
    }

    CNT_CONTAR("Elemento ligado");

    CED_MarcarEspacoAtivo(elem);

    CNT_CONTAR("Acaba verificar elemento");

    return LIS_CondRetOK;
}  /* Fim funcao: LIS &Verificar elemento */

/***********************************************************************
*
*  Funcao: TAB &Deturpar elemento
*
***********************************************************************/

void LIS_DeturparElemento(LIS_tppLista lista, int modoDeturpar)
{ 
    LIS_tpElemLista *elem = lista->pElemCorr;
    switch(modoDeturpar)
    {
        case 4: /* DeturpaTipoElemento */
            CED_DefinirTipoEspaco(elem, CED_ID_TIPO_VALOR_NULO);
            break;

        case 7: /* DeturpaPtElementoNulo */
            lista->pElemCorr = NULL;
            break;

        case 8: /* DeturpaDesencadeiaElemento */
            elem->pProx->pAnt = NULL;
            elem->pProx->pProx = NULL;
            break;
    }
}  /* Fim funcao: LIS &Deturpar elemento */

#endif 


/********** Fim do módulo de implementacao: LIS  Lista duplamente encadeada **********/

