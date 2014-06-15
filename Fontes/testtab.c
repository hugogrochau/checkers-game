/***************************************************************************
*  $MCI Módulo de implementação: TTAB  Teste de tabuleiro genérico
*
*  Arquivo gerado:              testtab.c
*  Letras identificadoras:      TTAB
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs, Gustavo Marques Martins (gmm), Lucas Reis (lr)
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data         Observações
*     2       hpg      14/jun/2014  Deturpação
*     1       gmm, lr  21/abr/2014  Implementação e documentação
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "tst_espc.h"

#include    "generico.h"
#include    "lerparm.h"

#include    "peca.h"
#include    "tabuleiro.h"


static const char CRIAR_TABULEIRO_CMD            [ ] = "=criartabuleiro"     ;
static const char DESTRUIR_TABULEIRO_CMD         [ ] = "=destruirtabuleiro"  ;
static const char OBTER_TAMANHO_CMD              [ ] = "=obtertamanho"       ;
static const char INCLUIR_PECA_CMD               [ ] = "=incluirpeca"        ;
static const char MOVER_PECA_CMD                 [ ] = "=moverpeca"          ;
static const char OBTER_PECA_CMD                 [ ] = "=obterpeca"          ;
static const char DESTRUIR_PECA_CMD              [ ] = "=destruirpeca"       ;
static const char REMOVER_PECA_CMD               [ ] = "=removerpeca"        ;
static const char RESETAR_TABULEIRO_CMD          [ ] = "=resetartabuleiro"   ;

static const char VERIFICAR_TABULEIRO_CMD        [ ] = "=verificartabuleiro" ;
static const char VERIFICAR_COLUNA_CMD           [ ] = "=verificarcoluna"    ;
static const char VERIFICAR_LINHA_CMD            [ ] = "=verificarlinha"     ;
static const char VERIFICAR_PECA_CMD             [ ] = "=verificarpeca"      ;
static const char DETURPAR_CMD                   [ ] = "=deturpar"           ;

#define TRUE  1
#define FALSE 0

#define DIM_VT_TABULEIRO   10

TAB_tppTabuleiro vtTab[ DIM_VT_TABULEIRO ];

/***** Protótipos das funções encapuladas no módulo *****/

static void DestruirPeca (void *pPeca);

static int ValidarInxTabuleiro( int inxTabuleiro ) ;

static int Comparacao( void *a, void *b ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***********************************************************************
*
*  $FC Função: TTAB &Testar tabuleiro
*
*  $ED Descrição da função
*     Podem ser criadas até 10 tabuleiros, identificadas pelos índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetartabuleiro
*     =criartabuleiro               inxTab   tamColuna   tamLinha   ValEsp
*     =destruirtabuleiro            inxTab
*     =obtertamanho                 inxTab   tamColuna   tamLinha
*     =incluirpeca                  inxTab   Coluna      Linha      cor          condEsp
*     =moverpeca                    inxTab   Coluna      Linha      ColunaDest   LinhaDest   condEsp
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char *ComandoTeste )
{

    int inxTab  = -1 ,
        numLidos   = -1 ;

    short int tamColuna = -1 ,
              tamLinha  = -1 ,
              Coluna    = -1 ,
              Linha     = -1 ,
              ColunaDest = -1,
              LinhaDest  = -1;

    PECA_tpCor cor;

    TAB_tpTamanho tam;
    TAB_tpPosicao pos,
                  posOrigem,
                  posDestino;
    TAB_tpCondRet condEsp;
    TAB_tpCondRet tabCondRet;

    PECA_tppPeca pDado ;
    int ValEsp = -1 ;

    int i ;

    /* Efetuar reset de teste de tabuleiro */

    if ( strcmp( ComandoTeste , RESETAR_TABULEIRO_CMD ) == 0 )
    {

        for ( i = 0 ; i < DIM_VT_TABULEIRO ; i++ )
        {
            vtTab[ i ] = NULL ;
        } /* for */

        return TST_CondRetOK ;

    } /* fim ativa: Efetuar reset de teste de tabuleiro */

    /* Testar Criar Tabuleiro */

    else if ( strcmp( ComandoTeste , CRIAR_TABULEIRO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iiii" ,
                                      &inxTab, &tamColuna, &tamLinha, &ValEsp ) ;

        if ( ( numLidos != 4 )
                || ( ! ValidarInxTabuleiro( inxTab )))
        {
            return TST_CondRetParm ;
        }

        vtTab[ inxTab ] =
            TAB_CriarTabuleiro( tamColuna, tamLinha, DestruirPeca ) ;

        return TST_CompararPonteiroNulo( 1 , vtTab[ inxTab ] ,
                                         "Erro em ponteiro de nova tabuleiro."  ) ;

    } /* fim ativa: Testar CriarTabuleiro */

    /* Testar Destruir Tabuleiro  */

    else if ( strcmp( ComandoTeste , DESTRUIR_TABULEIRO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" ,
                                      &inxTab ) ;

        if ( ( numLidos != 1 )
                || ( ! ValidarInxTabuleiro( inxTab )))
        {
            return TST_CondRetParm ;
        }

        TAB_DestruirTabuleiro( vtTab[ inxTab ] ) ;
        vtTab[ inxTab ] = NULL;

        return TST_CondRetOK ;

    } /* fim ativa: Testar Destruir Tabuleiro*/

    /* Testar Obter Tamanho */

    else if ( strcmp( ComandoTeste , OBTER_TAMANHO_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iii" ,
                                      &inxTab, &tamColuna, &tamLinha ) ;

        if ( ( numLidos != 3 )
                || ( ! ValidarInxTabuleiro( inxTab )))
        {
            return TST_CondRetParm ;
        }

        tam = TAB_ObterTamanho( vtTab[ inxTab ] ) ;
        if ((tam.colunas == tamColuna) && (tam.linhas == tamLinha))
            return TST_CondRetOK ;
        return TST_CondRetErro;

    } /* fim ativa: Testar Obter Tamanho */

    /* Testar Incluir Peca */
    else if ( strcmp( ComandoTeste , INCLUIR_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iiiii" ,
                                      &inxTab , &Coluna, &Linha, &cor , &condEsp ) ;

        if ( ( numLidos != 5 )
                || ( ! ValidarInxTabuleiro( inxTab )) )
        {
            return TST_CondRetParm ;
        }

        pDado = (PECA_tppPeca) PECA_CriarPeca (cor) ;
        if ( pDado == NULL )
        {
            return TST_CondRetMemoria ;
        }

        pos.coluna = Coluna;
        pos.linha = Linha;
        tabCondRet = TAB_IncluirPeca( vtTab[ inxTab ] , pDado , pos) ;

        if ( tabCondRet != TAB_CondRetOk )
        {
            PECA_DestruirPeca(pDado) ;
        }

        return TST_CompararInt( condEsp , tabCondRet ,
                                "Condicao de retorno errada ao incluir peca no tabuleiro." ) ;

    } /* fim ativa: Testar Incluir Peca*/
    /* Testar Mover Peça*/
    else if ( strcmp( ComandoTeste , MOVER_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iiiiii" ,
                                      &inxTab , &Coluna, &Linha, &ColunaDest , &LinhaDest, &condEsp ) ;

        if ( ( numLidos != 6 )
                || ( ! ValidarInxTabuleiro( inxTab )) )
        {
            return TST_CondRetParm ;
        }


        posOrigem.coluna = Coluna;
        posOrigem.linha = Linha;
        posDestino.coluna = ColunaDest;
        posDestino.linha = LinhaDest;
        tabCondRet = TAB_MoverPeca( vtTab[ inxTab ] , posOrigem , posDestino) ;


        return TST_CompararInt( condEsp , tabCondRet ,
                                "Condicao de retorno errada ao mover a peca no tabuleiro." ) ;

    } /* fim ativa: Testar Mover Peca*/
    /* Testar Obter Peca */
    else if ( strcmp( ComandoTeste , OBTER_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iiii" ,
                                      &inxTab , &Coluna, &Linha, &ValEsp ) ;

        if ( ( numLidos != 4  )
                || ( ! ValidarInxTabuleiro( inxTab )) )
        {
            return TST_CondRetParm ;
        }

        pos.coluna = Coluna;
        pos.linha = Linha;
        pDado = (PECA_tppPeca) TAB_ObterPeca( vtTab[ inxTab ] , pos) ;

        if ( ValEsp == 0 )
        {
            return TST_CompararPonteiroNulo( 0, pDado,
                                             "Peça não devia existir");
        }

        if ( pDado == NULL)
        {
            return TST_CompararPonteiroNulo( 1, pDado,
                                             "Peça deveria existir");
        }
        return TST_CondRetOK;

    } /* fim ativa: Testar Obter Peca*/
    /* Testar Destruir Peca */
    else if ( strcmp( ComandoTeste , DESTRUIR_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iiii" ,
                                      &inxTab , &Coluna, &Linha, &condEsp ) ;

        if ( ( numLidos != 4 )
                || ( ! ValidarInxTabuleiro( inxTab )) )
        {
            return TST_CondRetParm ;
        }

        pos.coluna = Coluna;
        pos.linha = Linha;
        tabCondRet = TAB_DestruirPeca( vtTab[ inxTab ] , pos) ;

        return TST_CompararInt( condEsp , tabCondRet ,
                                "Condicao de retorno errada ao excluir peca do tabuleiro." ) ;

    } /* fim ativa: Testar Destruir Peca*/
    /* Testar Remover Peca */
    else if ( strcmp( ComandoTeste , REMOVER_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "iiii" ,
                                      &inxTab , &Coluna, &Linha, &ValEsp ) ;

        if ( ( numLidos != 4  )
                || ( ! ValidarInxTabuleiro( inxTab )) )
        {
            return TST_CondRetParm ;
        }

        pos.coluna = Coluna;
        pos.linha = Linha;
        pDado = (PECA_tppPeca) TAB_RemoverPeca( vtTab[ inxTab ] , pos) ;

        if ( ValEsp == 0 )
        {
            return TST_CompararPonteiroNulo( 0, pDado, "Peça não devia existir");
        }

        if ( pDado == NULL )
        {
            return TST_CompararPonteiroNulo( 1, pDado, "Peça deveria existir");
        }
        return TST_CondRetOK;

    } /* fim ativa: Testar Remover Peca*/
    /* Testar Verificar Tabuleiro */
    else if ( strcmp( ComandoTeste, VERIFICAR_TABULEIRO_CMD ) == 0 )
    {

    }

    return TST_CondRetNaoConhec ;

} /* Fim função: TTAB &Testar tabuleiro */


/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
*
*  $FC Função: TTAB -Comparacao
*
***********************************************************************/

int Comparacao( void *a, void *b )
{
    return strcmp((const unsigned char *)a, (const unsigned char *)b);
}

/***********************************************************************
*
*  $FC Função: TTAB -Destruir peca
*
***********************************************************************/

static void DestruirPeca (void *pPeca)
{
    PECA_tppPeca peca = (PECA_tppPeca) pPeca;
    PECA_DestruirPeca(peca);
}

/***********************************************************************
*
*  $FC Função: TTAB -Validar indice do tabuleiro
*
***********************************************************************/

int ValidarInxTabuleiro( int inxTabuleiro )
{

    if ( ( inxTabuleiro <  0 )
            || ( inxTabuleiro >= DIM_VT_TABULEIRO ))
    {
        return FALSE ;
    }

    return TRUE ;

} /* Fim função: TTAB -Validar indice de tabuleiro */

/********** Fim do módulo de implementação: TTAB  Teste de tabuleiro genérico **********/

