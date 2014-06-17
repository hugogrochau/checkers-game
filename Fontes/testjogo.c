/***************************************************************************
*  $MCI Módulo de implementação: TJOGO  Teste de jogo de damas
*
*  Arquivo gerado:              testjogo.c
*  Letras identificadoras:      TJOGO
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor      Data         Observaçõesão
*     1       gmm, hpg  16/jun/2014  Implementação e documentação
*
***************************************************************************/


#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "generico.h"
#include    "lerparm.h"

#include    "jogo.h"


/* Tabela dos nomes dos comandos de teste específicos */

static const char CRIAR_JOGO_CMD                 [ ] = "=criarjogo"            ;
static const char DESTRUIR_JOGO_CMD              [ ] = "=destruirjogo"         ;
static const char PREENCHER_TABULEIRO_CMD        [ ] = "=preenchertabuleiro"   ;
static const char IMPRIMIR_TABULEIRO_CMD         [ ] = "=imprimirtabuleiro"    ;
static const char EXECUTAR_JOGADA_CMD            [ ] = "=executarjogada"       ;
static const char RESETAR_JOGO_CMD               [ ] = "=resetarjogo"          ;

#define TRUE  1
#define FALSE 0

#define PATH_TAB_INICIAL "TabuleiroInicial.tab"

#define DIM_VT_JOGO   10

JOGO_tppJogo vtJogo[ DIM_VT_JOGO ];

/***** Protótipos das funções encapuladas no módulo *****/

static int ValidarInxJogo( int inxJogo ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***********************************************************************
*
*  $FC Função: TJOGO &Testar jogo
*
*  $ED Descrição da função
*     Podem ser criadas até 10 jogos, identificadas pelos índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetarjogo             
*     =criarjogo               inxTab   nome1 [str]   nome2 [str] 
*     =destruirjogo            inxTab
*     =preenchertabuleiro      inxTab 
*     =imprimirtabuleiro       inxTab 
*     =executarjogada          inxTab   Linha    Coluna      LinhaDest   ColunaDest   condEsp
*
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char *ComandoTeste )
{

    int inxJogo  = -1 ,
        numLidos   = -1 ;
    FILE *fp;
    char nome1[120];
    char nome2[120];

    int Linha, Coluna, LinhaDest , ColunaDest;

    JOGO_tpCondRet condEsp;
    JOGO_tpCondRet jogoCondRet;

    int valEsp = -1 ;

    int i ;

    /* Efetuar reset de teste de jogo */

    if ( strcmp( ComandoTeste , RESETAR_JOGO_CMD ) == 0 )
    {

        for ( i = 0 ; i < DIM_VT_JOGO ; i++ )
        {
            vtJogo[ i ] = NULL ;
        } /* for */

        return TST_CondRetOK ;

    } /* fim ativa: Efetuar reset de teste de jogo */

    /* Testar Criar Jogo */

    else if ( strcmp( ComandoTeste , CRIAR_JOGO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iss" ,
                                      &inxJogo, nome1, nome2 ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxJogo( inxJogo )))
        {
            return TST_CondRetParm ;
        }

        vtJogo[ inxTab ] =
            JOGO_CriarJogo( nome1, nome2 ) ;

        return TST_CompararPonteiroNulo( 1 , vtJogo[ inxJogo ] ,
                                         "Erro em ponteiro de novo jogo."  ) ;

    } /* fim ativa: Testar CriarJogo */

    /* Testar Destruir jogo  */

    else if ( strcmp( ComandoTeste , DESTRUIR_JOGO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxJogo ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxTabuleiro( inxJogo )))
        {
            return TST_CondRetParm ;
        }

        JOGO_DestruirJogo( vtJogo[ inxJogo ] ) ;
        vtJogo[ inxJogo ] = NULL;

        return TST_CondRetOK ;

    } /* fim ativa: Testar Destruir Jogo*/

    /* Testar Preencher Tabuleiro */

    else if ( strcmp( ComandoTeste , PREENCHER_TABULEIRO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxJogo ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxJogo( inxJogo )))
        {
            return TST_CondRetParm ;
        }

        fp = fopen(PATH_TAB_INICIAL,"r");
        if (fp == NULL)
        {
        	return TST_CompararPonteiroNulo(1,fp,"Erro na abertura do arquivo." );
        }
        if (JOGO_PreencherTabuleiro(vtJogo[ inxJogo ], fp) == JOGO_CondRetOK)
        {
        	TST_CondRetOK ;
        }
        return TST_CondRetErro ;

    } /* fim ativa: Testar Preencher Tabuleiro */

    /* Testar Imprimir tabuleiro */
    else if ( strcmp( ComandoTeste , IMPRIMIR_TABULEIRO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxJogo  ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxJogo( inxJogo )) )
        {
            return TST_CondRetParm ;
        }

        if (JOGO_ImprimirTabuleiro(vtJogo[ inxJogo ]) == JOGO_CondRetOK)
        {
        	return TST_CondRetOK ;
        }

        return TST_CondRetErro ;
    } /* fim ativa: Testar imprimir tabuleiro*/

    /* Testar Executar Jogada*/
    else if ( strcmp( ComandoTeste , executarjogada ) == 0 )
    {

        numLidos = LER_LerParametros( "iiiiii" ,
                                      &inxJogo , &Linha, &Coluna, &LinhaDest , &ColunaDest, &condEsp ) ;
        if ( ( numLidos != 6 )
                || ( ! ValidarInxTabuleiro( inxTab )) )
        {
            return TST_CondRetParm ;
        }


        jogoCondRet = JOGO_ExecutarJogada( vtJogo[ inxJogo ] , Linha , Coluna, LinhaDest, ColunaDest ) ;


        return TST_CompararInt( condEsp , jogoCondRet ,
                                "Condicao de retorno errada ao executar uma jogada." ) ;

    } /* fim ativa: Testar Executar Jogada*/

    return TST_CondRetNaoConhec ;

} /* Fim função: TJOGO &Testar jogo */



/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
*
*  $FC Função: TJOGO -Validar indice do jogo
*
***********************************************************************/

int ValidarInxJogo( int inxJogo )
{

    if ( ( inxTabuleiro <  0 )
            || ( inxTabuleiro >= DIM_VT_JOGO ))
    {
        return FALSE ;
    }

    return TRUE ;

} /* Fim função: TJOGO -Validar indice de jogo */

/********** Fim do módulo de implementação: TJOGO  Teste de jogo genérico **********/


