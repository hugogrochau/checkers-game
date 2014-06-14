/***************************************************************************
*  $MCI M�dulo de implementa��o: PECA  Pe�a de tabuleiro de damas
*
*  Arquivo gerado:              peca.c
*  Letras identificadoras:      PECA
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Hugo Pedrotti Grochau (hpg)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     2       hpg   21/apr/2014 melhorando a documenta��o
*     1       hpg   31/mar/2014 in�cio desenvolvimento
*
***************************************************************************/

#include <malloc.h>
#include <assert.h>
#include <stdlib.h>

#define PECA_OWN
#include "peca.h"
#undef PECA_OWN

/***********************************************************************
*
*  $TC Tipo de dados: PECA Descritor da pe�a
*
***********************************************************************/

typedef struct PECA_tagPeca
{
    PECA_tpCor cor;
    PECA_tpStatus status;
} PECA_tpPeca;


/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***********************************************************************
*
*  Fun��o: PECA &Criar Pe�a
*
***********************************************************************/

PECA_tppPeca PECA_CriarPeca(PECA_tpCor cor)
{
    PECA_tppPeca pPeca = NULL;
    pPeca = (PECA_tppPeca) malloc(sizeof(PECA_tpPeca));
    if ( pPeca == NULL )
    {
        return NULL;
    }
    pPeca->cor = cor;
    pPeca->status = PECA_StatusNormal;
    return pPeca;
}

/***********************************************************************
*
*  Fun��o: PECA &Obter cor
*
***********************************************************************/

PECA_tpCor PECA_ObterCor(PECA_tppPeca pPeca)
{
    if (pPeca == NULL)
    {
        return PECA_CorInvalida;
    }
    return pPeca->cor;
}

/***********************************************************************
*
*  Fun��o: PECA &Obter status
*
***********************************************************************/

PECA_tpStatus PECA_ObterStatus(PECA_tppPeca pPeca)
{
    if (pPeca == NULL)
    {
        return PECA_StatusInvalido;
    }
    return pPeca->status;
}

/***********************************************************************
*
*  Fun��o: PECA &Virar dama
*
***********************************************************************/

PECA_tpCondRet PECA_VirarDama(PECA_tppPeca pPeca)
{
    if (pPeca == NULL)
    {
        return PECA_CondRetNaoExiste;
    }
    if (pPeca->status == PECA_StatusDama)
    {
        return PECA_CondRetEhDama;
    }
    pPeca->status = PECA_StatusDama;
    return PECA_CondRetOK;
}

/***********************************************************************
*
*  Fun��o: PECA &DestruirPeca
*
***********************************************************************/

void PECA_DestruirPeca( PECA_tppPeca pPeca )
{
    if ( pPeca != NULL)
    {
        free( pPeca );
    }
}

/********** Fim do m�dulo de implementa��o: PECA  Pe�a de tabuleiro de damas **********/
