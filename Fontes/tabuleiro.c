/***************************************************************************
*  $MCI M�dulo de implementa��o: TAB  tabuleiro de damas ou qualquer outro jogo
*
*  Arquivo gerado:              tabuleiro.c
*  Letras identificadoras:      TAB
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor     Data        Observa��es
*     4       hpg,gmm   30/abr/2014  Coment�rios
*     3       gmm       19/abr/2014  Mais implementa��o
*     2       gmm       18/abr/2014  Mais implementa��o
*     1       gmm       16/abr/2014  in�cio desenvolvimento
*
***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define tabuleiro_OWN
#include "tabuleiro.h"
#undef tabuleiro_OWN

#define TRUE  1
#define FALSE 0

/***********************************************************************
*
*  $TC Tipo de dados: TAB Tabuleiro
*
***********************************************************************/

struct TAB_tagTabuleiro
{
    TAB_tpTamanho tam;
    
    LIS_tppLista coluna;
    /* Ponteiro para o come�o da lista de listas */ 
};

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

/***********************************************************************
*
*  $FC Fun��o: TAB  -Destruir linha do tabuleiro
*
***********************************************************************/

static void TAB_DestruirLinha (void * lstLin);

/***********************************************************************
*
*  $FC Fun��o: TAB  -Destruir uma lista por completo
*
*  $ED Descri��o da fun��o
*      Destroi por completo uma lista. Destroi os elementos e dps a cabe�a da lista
*
***********************************************************************/

static void TAB_DestruirLista (LIS_tppLista lst);

/***********************************************************************
*
*  $FC Fun��o: TAB  -Checar posi��o
*
*  $ED Descri��o da fun��o
*      Checa se a posi��o � invalida ou nao. Para ser invalida,
*      POS < 0 ou POS >= tabuleiro.tamanho
*      Se a posicao for valida retorna 1, se for invaliad retorna 0
*
***********************************************************************/

static int TAB_ChecarPos (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Fun��o: TAB  -Obter casa
*
*  $ED Descri��o da fun��o
*      Retorna uma lista apontando para a posi��o desejada 
*
***********************************************************************/

static LIS_tppLista TAB_ObterCasa (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Fun��o: TAB  -Ir inicio tabuleiro
*
*  $ED Descri��o da fun��o
*      Vai para o inicio do tabuleiro, resetando posi��o onde estava 
*
***********************************************************************/

static void TAB_IrInicioTabuleiro (TAB_tppTabuleiro tab);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  Fun��o: TAB &Criar tabuleiro
*
***********************************************************************/

TAB_tppTabuleiro TAB_CriarTabuleiro (short int colunas, short int linhas, void (*DestruirPeca) (void *pDado))
{
    TAB_tppTabuleiro tab = NULL;
    int i, j;
    LIS_tppLista lstCol = NULL;
    LIS_tppLista lstLin = NULL;

    tab = (TAB_tppTabuleiro) malloc(sizeof(struct TAB_tagTabuleiro));

    if (tab == NULL) 
    {
        return NULL;
    }

    lstCol = LIS_CriarLista(TAB_DestruirLinha);

    /* Cria coluna de referencia para as linhas. se a coluna for null, destroi o tabuleiro e retorna null */

    if (lstCol == NULL)
    {
        free(tab);
        return NULL;
    }

    /* Cria as linhas e as insere na lista coluna. os elementos das listas linhas sao inicializados com null. se ocorrer algum problema de alocacao,
    detrou toda a memoria alocada at� entao e retorna null */

    for (i = 0; i < linhas ; i++)
    {
        lstLin = LIS_CriarLista(DestruirPeca);  /* PASSAR A FUN��O DE DESTRUIR A PECA AQUI */
        for (j = 0; j < colunas; j++)
        {
            if (LIS_InserirElementoAntes(lstLin, NULL) != LIS_CondRetOK)
            {
                TAB_DestruirLista(lstLin);
                return NULL;
            }
        }
        if (LIS_InserirElementoAntes(lstCol, lstLin) != LIS_CondRetOK)
        {
            LIS_EsvaziarLista(lstCol);
            return NULL;
        }
    }

    /* Atualiza o tamanho do tabuleiro e seta o ponteiro para a coluna de refer�ncia */

    tab->tam.colunas = colunas;
    tab->tam.linhas = linhas;
    tab->coluna = lstCol;
    return tab;
}

/***********************************************************************
*
*  Fun��o: TAB &Destruir tabuleiro
*
***********************************************************************/

void TAB_DestruirTabuleiro (TAB_tppTabuleiro tab)
{
    if (tab == NULL) 
    {
        return;
    }
    TAB_DestruirLista(tab->coluna);
    free(tab);
}

/***********************************************************************
*
*  Fun��o: TAB &Obter tamanho
*
***********************************************************************/

TAB_tpTamanho TAB_ObterTamanho (TAB_tppTabuleiro tab)
{
    return tab->tam;
}

/***********************************************************************
*
*  Fun��o: TAB &Incluir peca
*
***********************************************************************/

TAB_tpCondRet TAB_IncluirPeca (TAB_tppTabuleiro tab, void * pPeca, TAB_tpPosicao pos)
{
    LIS_tppLista lst;
    if (tab == NULL) 
    {
        return TAB_CondRetTabuleiroVazio;
    }

    if (!TAB_ChecarPos(tab, pos))
    {
        return TAB_CondRetPosicaoInvalida;
    }

    lst = TAB_ObterCasa(tab, pos);
    LIS_SobrescreverValorCorrente(lst, pPeca);
    TAB_IrInicioTabuleiro(tab);
    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Fun��o: TAB &Obter tamanho
*
***********************************************************************/

TAB_tpCondRet TAB_MoverPeca (TAB_tppTabuleiro tab, TAB_tpPosicao Original, TAB_tpPosicao Destino)
{
    LIS_tppLista lst;
    void *pDado;

    if (tab == NULL) 
    {
        return TAB_CondRetTabuleiroVazio;
    }

    if (!TAB_ChecarPos(tab, Original) || !TAB_ChecarPos(tab, Destino)) 
    {
        return TAB_CondRetPosicaoInvalida;
    }

    lst = TAB_ObterCasa(tab, Original);
    pDado = LIS_ObterValor(lst);
    if (pDado == NULL)
	{
		TAB_IrInicioTabuleiro(tab);
        return TAB_CondRetOk;
	}
    LIS_SobrescreverValorCorrente(lst, NULL);
    TAB_IrInicioTabuleiro(tab);
    lst = TAB_ObterCasa(tab, Destino);
    LIS_SobrescreverValorCorrente(lst, pDado);
    TAB_IrInicioTabuleiro(tab);
    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Fun��o: TAB &Obter peca
*
***********************************************************************/

void * TAB_ObterPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
{
    LIS_tppLista lst;
    void *pDado;
	if (tab == NULL)
    {
		return NULL;
    }
    if (!TAB_ChecarPos(tab, pos))
    {
        return NULL;
    }
    lst = TAB_ObterCasa(tab, pos);
    pDado = LIS_ObterValor(lst);
    TAB_IrInicioTabuleiro(tab);
    return pDado;
}

/***********************************************************************
*
*  Fun��o: TAB &Destruir peca
*
***********************************************************************/

TAB_tpCondRet TAB_DestruirPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
{
    LIS_tppLista lst;
    if (tab == NULL)
    {
        return TAB_CondRetTabuleiroVazio;
    }
    if (!TAB_ChecarPos(tab, pos))
    {
        return TAB_CondRetPosicaoInvalida;
    }
    lst = TAB_ObterCasa(tab, pos);
    LIS_ExcluirValorElementoCorrente(lst);
    TAB_IrInicioTabuleiro(tab);
    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Fun��o: TAB &Remover peca
*
***********************************************************************/

void * TAB_RemoverPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
{
    void *pDado;
    LIS_tppLista lst;
    if (tab == NULL)
    {
        return NULL;
    }
    if (!TAB_ChecarPos(tab, pos))
    { 
            return NULL;
    }

    lst = TAB_ObterCasa(tab, pos);
    pDado = LIS_ObterValor(lst);
    LIS_SobrescreverValorCorrente(lst, NULL);
    TAB_IrInicioTabuleiro(tab);
    return pDado;
}

/* C�digo das fun��es encapsuladas no m�dulo */


/***********************************************************************
*
*  Fun��o: TAB &Destruir linha
*
***********************************************************************/

static void TAB_DestruirLinha (void * lstLin)
{
    LIS_tppLista lst = (LIS_tppLista) lstLin;
    if (lst == NULL)
    {
        return;
    }
    LIS_EsvaziarLista(lst);
    LIS_DestruirLista(lst);
}

/***********************************************************************
*
*  Fun��o: TAB &Destruir lista
*
***********************************************************************/

static void TAB_DestruirLista (LIS_tppLista lst)
{
    if (lst == NULL)
    {
        return;
    }
    LIS_EsvaziarLista(lst);
    LIS_DestruirLista(lst);
}

/***********************************************************************
*
*  Fun��o: TAB &Checar pos
*
***********************************************************************/

static int TAB_ChecarPos (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
{
    if (((tab->tam.colunas <= pos.coluna) || (pos.coluna < 0)) ||
        ((tab->tam.linhas <= pos.linha) || (pos.linha < 0)))
    {
        return FALSE;
    }
    return TRUE;
}

/***********************************************************************
*
*  Fun��o: TAB &Obter casa
*
***********************************************************************/

static LIS_tppLista TAB_ObterCasa (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
{
    LIS_tppLista lst = NULL;
    LIS_AvancarElementoCorrente(tab->coluna, pos.linha);
    lst = (LIS_tppLista) LIS_ObterValor(tab->coluna);
    LIS_AvancarElementoCorrente(lst, pos.coluna);
    return lst;
}

/***********************************************************************
*
*  Fun��o: TAB &Ir inicio tabuleiro
*
***********************************************************************/

static void TAB_IrInicioTabuleiro (TAB_tppTabuleiro tab)
{
    LIS_tppLista lst;
    if (tab == NULL)
    {
        return;
    }
    lst = (LIS_tppLista)LIS_ObterValor(tab->coluna);
    if (lst != NULL)
    {
        IrInicioLista(lst);
    }
    IrInicioLista(tab->coluna);
}

/********** Fim do m�dulo de implementa��o: TAB  tabuleiro de damas ou qualquer outro jogo **********/
