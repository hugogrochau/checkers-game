/***************************************************************************
*  $MCI M�dulo de implementa��o: TLIS Teste lista de s�mbolos
*
*  Arquivo gerado:              testlis.c
*  Letras identificadoras:      TLIS
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs, Hugo Pedrotti Grochau (hpg), Gustavo Martins Marques (gmm)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     6       gmm   19/abr/2014 implementa��o da fun��o alterar valor corrente e
*                               deletar valor elemento corrente
*     5       hpg   31/mar/2014 implementa��o da fun��o de inser��o ordenada
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "tst_espc.h"

#include    "generico.h"
#include    "lerparm.h"

#include    "lista.h"


static const char RESET_LISTA_CMD         [ ] = "=resetteste"        ;
static const char CRIAR_LISTA_CMD         [ ] = "=criarlista"        ;
static const char DESTRUIR_LISTA_CMD      [ ] = "=destruirlista"     ;
static const char ESVAZIAR_LISTA_CMD      [ ] = "=esvaziarlista"     ;
static const char INS_ELEM_ORDENADO_CMD   [ ] = "=inselemord"        ;
static const char INS_ELEM_ANTES_CMD      [ ] = "=inselemantes"      ;
static const char INS_ELEM_APOS_CMD       [ ] = "=inselemapos"       ;
static const char OBTER_VALOR_CMD         [ ] = "=obtervalorelem"    ;
static const char EXC_ELEM_CMD            [ ] = "=excluirelem"       ;
static const char IR_INICIO_CMD           [ ] = "=irinicio"          ;
static const char IR_FIM_CMD              [ ] = "=irfinal"           ;
static const char AVANCAR_ELEM_CMD        [ ] = "=avancarelem"       ;
static const char SOBRESCREVER_VALOR_CMD  [ ] = "=sobrescrevervalor" ;
static const char EXCLUIR_VALOR_CMD       [ ] = "=excluirvalor"      ;

#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1

#define DIM_VT_LISTA   10
#define DIM_VALOR     100

LIS_tppLista   vtListas[ DIM_VT_LISTA ] ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static void DestruirValor( void *pValor ) ;

static int ValidarInxLista( int inxLista , int Modo ) ;

static int Comparacao( void *a, void *b ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TLIS &Testar lista
*
*  $ED Descri��o da fun��o
*     Podem ser criadas at� 10 listas, identificadas pelos �ndices 0 a 10
*
*     Comandos dispon�veis:
*
*     =resetteste
*           - anula o vetor de listas. Provoca vazamento de mem�ria
*     =criarlista                   inxLista
*     =destruirlista                inxLista
*     =esvaziarlista                inxLista
*     =inselemantes                 inxLista  string  CondRetEsp
*     =inselemapos                  inxLista  string  CondRetEsp
*     =obtervalorelem               inxLista  string  CondretPonteiro
*     =excluirelem                  inxLista  CondRetEsp
*     =irinicio                     inxLista
*     =irfinal                      inxLista
*     =avancarelem                  inxLista  numElem CondRetEsp
*     =sobrescrevervalor            inxLista  string
*     =excluirvalor                 inxLista
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char *ComandoTeste )
{

    int inxLista  = -1 ,
        numLidos   = -1 ,
        CondRetEsp = -1  ;

    TST_tpCondRet CondRet ;

    char   StringDado[  DIM_VALOR ] ;
    char *pDado ;

    int ValEsp = -1 ;

    int i ;

    int numElem = -1 ;

    StringDado[ 0 ] = 0 ;

    /* Efetuar reset de teste de lista */

    if ( strcmp( ComandoTeste , RESET_LISTA_CMD ) == 0 )
    {

        for ( i = 0 ; i < DIM_VT_LISTA ; i++ )
        {
            vtListas[ i ] = NULL ;
        } /* for */

        return TST_CondRetOK ;

    } /* fim ativa: Efetuar reset de teste de lista */

    /* Testar CriarLista */

    else if ( strcmp( ComandoTeste , CRIAR_LISTA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxLista ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxLista( inxLista , VAZIO )))
        {
            return TST_CondRetParm ;
        } /* if */

        vtListas[ inxLista ] =
            LIS_CriarLista( DestruirValor ) ;

        return TST_CompararPonteiroNulo( 1 , vtListas[ inxLista ] ,
                                         "Erro em ponteiro de nova lista."  ) ;

    } /* fim ativa: Testar CriarLista */

    /* Testar Esvaziar lista lista */

    else if ( strcmp( ComandoTeste , ESVAZIAR_LISTA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxLista ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )))
        {
            return TST_CondRetParm ;
        } /* if */

        LIS_EsvaziarLista( vtListas[ inxLista ] ) ;

        return TST_CondRetOK ;

    } /* fim ativa: Testar Esvaziar lista lista */

    /* Testar Destruir lista */

    else if ( strcmp( ComandoTeste , DESTRUIR_LISTA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxLista ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )))
        {
            return TST_CondRetParm ;
        } /* if */

        LIS_DestruirLista( vtListas[ inxLista ] ) ;
        vtListas[ inxLista ] = NULL ;

        return TST_CondRetOK ;

    } /* fim ativa: Testar Destruir lista */

    /* Testar inserir elemento ordenado */
    else if ( strcmp( ComandoTeste , INS_ELEM_ORDENADO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "isi" ,
                                      &inxLista , StringDado , &CondRetEsp ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        pDado = ( char *) malloc( strlen( StringDado ) + 1 ) ;
        if ( pDado == NULL )
        {
            return TST_CondRetMemoria ;
        } /* if */

        strcpy( pDado , StringDado ) ;


        CondRet = LIS_InserirElementoOrdenado( vtListas[ inxLista ] , pDado , &Comparacao) ;

        if ( CondRet != LIS_CondRetOK )
        {
            free( pDado ) ;
        } /* if */

        return TST_CompararInt( CondRetEsp , CondRet ,
                                "Condicao de retorno errada ao inserir antes."                   ) ;

    } /* fim ativa: Testar inserir elemento antes */

    /* Testar inserir elemento antes */

    else if ( strcmp( ComandoTeste , INS_ELEM_ANTES_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "isi" ,
                                      &inxLista , StringDado , &CondRetEsp ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        pDado = ( char *) malloc( strlen( StringDado ) + 1 ) ;
        if ( pDado == NULL )
        {
            return TST_CondRetMemoria ;
        } /* if */

        strcpy( pDado , StringDado ) ;


        CondRet = LIS_InserirElementoAntes( vtListas[ inxLista ] , pDado ) ;

        if ( CondRet != LIS_CondRetOK )
        {
            free( pDado ) ;
        } /* if */

        return TST_CompararInt( CondRetEsp , CondRet ,
                                "Condicao de retorno errada ao inserir antes."                   ) ;

    } /* fim ativa: Testar inserir elemento antes */

    /* Testar inserir elemento apos */

    else if ( strcmp( ComandoTeste , INS_ELEM_APOS_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "isi" ,
                                      &inxLista , StringDado , &CondRetEsp ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        pDado = ( char *) malloc( strlen( StringDado ) + 1 ) ;
        if ( pDado == NULL )
        {
            return TST_CondRetMemoria ;
        } /* if */

        strcpy( pDado , StringDado ) ;


        CondRet = LIS_InserirElementoApos( vtListas[ inxLista ] , pDado ) ;

        if ( CondRet != LIS_CondRetOK )
        {
            free( pDado ) ;
        } /* if */

        return TST_CompararInt( CondRetEsp , CondRet ,
                                "Condicao de retorno errada ao inserir apos."                   ) ;

    } /* fim ativa: Testar inserir elemento apos */

    /* Testar excluir simbolo */

    else if ( strcmp( ComandoTeste , EXC_ELEM_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "ii" ,
                                      &inxLista , &CondRetEsp ) ;

        if ( ( numLidos != 2 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        return TST_CompararInt( CondRetEsp ,
                                LIS_ExcluirElemento( vtListas[ inxLista ] ) ,
                                "Condi��o de retorno errada ao excluir."   ) ;

    } /* fim ativa: Testar excluir simbolo */

    /* Testar obter valor do elemento corrente */

    else if ( strcmp( ComandoTeste , OBTER_VALOR_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "isi" ,
                                      &inxLista , StringDado , &ValEsp ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        pDado = ( char *) LIS_ObterValor( vtListas[ inxLista ] ) ;

        if ( ValEsp == 0 )
        {
            return TST_CompararPonteiroNulo( 0 , pDado ,
                                             "Valor n�o deveria existir." ) ;
        } /* if */

        if ( pDado == NULL )
        {
            return TST_CompararPonteiroNulo( 1 , pDado ,
                                             "Dado tipo um deveria existir." ) ;
        } /* if */

        return TST_CompararString( StringDado , pDado ,
                                   "Valor do elemento errado." ) ;

    } /* fim ativa: Testar obter valor do elemento corrente */

    /* Testar ir para o elemento inicial */

    else if ( strcmp( ComandoTeste , IR_INICIO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" , &inxLista ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        IrInicioLista( vtListas[ inxLista ] ) ;

        return TST_CondRetOK ;

    } /* fim ativa: Testar ir para o elemento inicial */

    /* LIS  &Ir para o elemento final */

    else if ( strcmp( ComandoTeste , IR_FIM_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" , &inxLista ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        IrFinalLista( vtListas[ inxLista ] ) ;

        return TST_CondRetOK ;

    } /* fim ativa: LIS  &Ir para o elemento final */

    /* LIS  &Avan�ar elemento */

    else if ( strcmp( ComandoTeste , AVANCAR_ELEM_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iii" , &inxLista , &numElem ,
                                      &CondRetEsp ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        return TST_CompararInt( CondRetEsp ,
                                LIS_AvancarElementoCorrente( vtListas[ inxLista ] , numElem ) ,
                                "Condicao de retorno errada ao avancar" ) ;

    } /* fim ativa: LIS  &Avan�ar elemento */
	else if ( strcmp( ComandoTeste , SOBRESCREVER_VALOR_CMD ) == 0 )
	{
       numLidos = LER_LerParametros( "is" ,
                                      &inxLista , StringDado ) ;

        if ( ( numLidos != 2 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

        pDado = ( char *) malloc( strlen( StringDado ) + 1 ) ;
        
		if ( pDado == NULL )
        {
            return TST_CondRetMemoria ;
        } /* if */

        strcpy( pDado , StringDado ) ;


        LIS_SobrescreverValorCorrente( vtListas[ inxLista ] , pDado ) ;

        return TST_CondRetOK;
	}
	else if ( strcmp( ComandoTeste , EXCLUIR_VALOR_CMD ) == 0 )
	{
       numLidos = LER_LerParametros( "i" ,
                                      &inxLista ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
        {
            return TST_CondRetParm ;
        } /* if */

		LIS_ExcluirValorElementoCorrente( vtListas[ inxLista ] );

		return TST_CondRetOK;
	}
    return TST_CondRetNaoConhec ;

} /* Fim fun��o: TLIS &Testar lista */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: TLIS -Destruir valor
*
***********************************************************************/

int Comparacao( void *a, void *b )
{
    return strcmp((const unsigned char *)a, (const unsigned char *)b);
}

/***********************************************************************
*
*  $FC Fun��o: TLIS -Destruir valor
*
***********************************************************************/

void DestruirValor( void *pValor )
{

    free( pValor ) ;

} /* Fim fun��o: TLIS -Destruir valor */


/***********************************************************************
*
*  $FC Fun��o: TLIS -Validar indice de lista
*
***********************************************************************/

int ValidarInxLista( int inxLista , int Modo )
{

    if ( ( inxLista <  0 )
            || ( inxLista >= DIM_VT_LISTA ))
    {
        return FALSE ;
    } /* if */

    if ( Modo == VAZIO )
    {
        if ( vtListas[ inxLista ] != 0 )
        {
            return FALSE ;
        } /* if */
    }
    else
    {
        if ( vtListas[ inxLista ] == 0 )
        {
            return FALSE ;
        } /* if */
    } /* if */

    return TRUE ;

} /* Fim fun��o: TLIS -Validar indice de lista */

/********** Fim do m�dulo de implementa��o: TLIS Teste lista de s�mbolos **********/

