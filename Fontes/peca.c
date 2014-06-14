/***************************************************************************
*  $MCI Módulo de implementação: PECA  Peça de tabuleiro de damas
*
*  Arquivo gerado:              peca.c
*  Letras identificadoras:      PECA
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     2       hpg   21/apr/2014 melhorando a documentação
*     1       hpg   31/mar/2014 início desenvolvimento
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
*  $TC Tipo de dados: PECA Descritor da peça
*
***********************************************************************/

typedef struct PECA_tagPeca
{
    PECA_tpCor cor;
    PECA_tpStatus status;
} PECA_tpPeca;


/*****  Código das funções exportadas pelo módulo  *****/

/***********************************************************************
*
*  Função: PECA &Criar Peça
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
*  Função: PECA &Obter cor
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
*  Função: PECA &Obter status
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
*  Função: PECA &Virar dama
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
*  Função: PECA &DestruirPeca
*
***********************************************************************/

void PECA_DestruirPeca( PECA_tppPeca pPeca )
{
    if ( pPeca != NULL)
    {
        free( pPeca );
    }
}

/********** Fim do módulo de implementação: PECA  Peça de tabuleiro de damas **********/
