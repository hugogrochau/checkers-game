/***************************************************************************
*  $MCI Módulo de implementação: TPECA Teste de peça de damas
*
*  Arquivo gerado:              testpeca.c
*  Letras identificadoras:      TPECA
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Hugo Pedrotti Grochau (hpg), Lucas Reis (lr)
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data         Observações
*     2       lr       21/apr/2014  Documentação
*     1       hpg      31/mar/2014  Desenvolvimento inicial
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "generico.h"
#include    "lerparm.h"

#include    "peca.h"

static const char RESETAR_PECAS_CMD [ ] = "=resetarpecas"   ;
static const char CRIAR_PECA_CMD    [ ] = "=criarpeca"      ;
static const char OBTER_COR_CMD     [ ] = "=obtercor"       ;
static const char OBTER_STATUS_CMD  [ ] = "=obterstatus"    ;
static const char VIRAR_DAMA_CMD    [ ] = "=virardama"      ;
static const char DESTRUIR_PECA_CMD [ ] = "=destruirpeca"   ;

#define TRUE  1
#define FALSE 0

#define DIM_VT_PECA 10

PECA_tppPeca vtPecas[ DIM_VT_PECA ];

/***** Protótipos das funções encapuladas no módulo *****/

static int ValidarCor( int cor ) ;

static int ValidarInxPeca( int inxPeca ) ;

static int ValidarStatus( int status );

/*****  Código das funções exportadas pelo módulo  *****/

/***********************************************************************
*
*  $FC Função: TPECA &Testar peça
*
*  $ED Descrição da função
*     Podem ser criadas até 10 peças, identificadas pelos índices 0 a 10
*
*     Comandos disponíveis:
*
*     =resetarpecas
*     =destruirpeca       inxPeca
*     =obtercor           inxPeca   corPecaEsp
*     =obterstatus        inxPeca   statusPecaEsp
*     =criarpeca          inxPeca   corPeca
*     =obtercor           inxPeca   corPecaEsp
*     =virardama          inxPeca   condRetEsp
*     =destruirpeca       inxPeca
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char *ComandoTeste )
{

    int inxPeca = -1 ,
        numLidos = -1 ,
        corPeca = -1 ,
        valEsp = - 1 ,
        nDado = -1 ,
        condRetEsp = -1;

    int i;

    if ( strcmp( ComandoTeste , RESETAR_PECAS_CMD ) == 0 )
    {

        for ( i = 0 ; i < DIM_VT_PECA ; i++ )
        {

            if ( vtPecas[ i ] != NULL )
            {
                PECA_DestruirPeca( vtPecas[ i ] );
                vtPecas[ i ] = NULL ;
            }
        }

        return TST_CondRetOK ;
    }
    else if ( strcmp( ComandoTeste , CRIAR_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "ii" , &inxPeca, &corPeca );

        if ( numLidos != 2 || !ValidarInxPeca( inxPeca ) )
        {
            return TST_CondRetParm ;
        }

        if ( !ValidarCor( corPeca ) )
        {
            return TST_NotificarFalha( "Cor tem que ser preta (0) ou branca (1)" );
        }

        vtPecas[ inxPeca ] = PECA_CriarPeca( corPeca ) ;

        return TST_CompararPonteiroNulo( 1 , vtPecas[ inxPeca ] ,
                                         "Erro em criar nova peça."  ) ;

    }
    else if ( strcmp( ComandoTeste, OBTER_COR_CMD ) == 0 )
    {
        numLidos = LER_LerParametros( "ii", &inxPeca, &valEsp );

        if ( numLidos != 2 || !ValidarInxPeca( inxPeca ) )
        {
            return TST_CondRetParm ;
        }

        if ( !ValidarCor( valEsp ) )
        {
            return TST_NotificarFalha( "Cor tem que ser preta (0) ou branca (1)" );
        }

        nDado = PECA_ObterCor( vtPecas[ inxPeca ] ) ;

        if ( !ValidarCor( nDado ) )
        {
            return TST_NotificarFalha( "Cor tem que ser preta (0) ou branca (1)." );
        }

        return TST_CompararInt( valEsp , nDado,
                                "Cor esperada é diferente da cor obtida." ) ;
    }
    else if ( strcmp( ComandoTeste, OBTER_STATUS_CMD ) == 0 )
    {
        numLidos = LER_LerParametros( "ii", &inxPeca, &valEsp );

        if ( numLidos != 2 || !ValidarInxPeca( inxPeca ) )
        {
            return TST_CondRetParm ;
        }

        if ( !ValidarStatus( valEsp ) )
        {
            return TST_NotificarFalha( "Status tem que ser normal (0) ou dama (1)." );
        }

        nDado = PECA_ObterStatus( vtPecas[ inxPeca ] ) ;

        if ( !ValidarStatus( nDado ) )
        {
            return TST_NotificarFalha( "Stauts tem que ser normal (0) ou dama (1)." );
        }

        return TST_CompararInt( valEsp , nDado,
                                "Cor esperada é diferente da cor obtida." ) ;
    }
    else if ( strcmp( ComandoTeste , VIRAR_DAMA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "ii" , &inxPeca, &condRetEsp ) ;

        if ( numLidos != 2 || !ValidarInxPeca( inxPeca ) )
        {
            return TST_CondRetParm ;
        }

        nDado = PECA_VirarDama( vtPecas[ inxPeca ] ) ;

        return TST_CompararInt( condRetEsp , nDado ,
                                "Valor de retorno nao confere com o esperado."  ) ;

    }
    else if ( strcmp( ComandoTeste , DESTRUIR_PECA_CMD ) == 0 )
    {

        numLidos = LER_LerParametros( "i" , &inxPeca ) ;

        if ( numLidos != 1 || !ValidarInxPeca( inxPeca ) )
        {
            return TST_CondRetParm ;
        }

        PECA_DestruirPeca( vtPecas[ inxPeca ] ) ;
        vtPecas[ inxPeca ] = NULL;

        return TST_CondRetOK ;

    }
    return TST_CondRetNaoConhec ;

}

/*****  Código das funções encapsuladas no módulo  *****/

/***********************************************************************
*
*  $FC Função: TPECA -Validar o status da peça
*
***********************************************************************/

static int ValidarStatus( int status )
{
    return status >= -1 && status <= 1 ;
}

/***********************************************************************
*
*  $FC Função: TPECA -Validar a cor da peça
*
***********************************************************************/

static int ValidarCor( int cor )
{
    return cor >= -1 && cor <= 1 ;
}

/***********************************************************************
*
*  $FC Função: TPECA -Validar indice das peças de teste
*
***********************************************************************/

static int ValidarInxPeca( int inxPeca )
{
    return inxPeca >= 0 && inxPeca <= DIM_VT_PECA ;
}

/********** Fim do módulo de implementação: TPECA Teste de peça de damas **********/
