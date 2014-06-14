/***************************************************************************
*  $MCI Módulo de implementação: TAB  tabuleiro de damas ou qualquer outro jogo
*
*  Arquivo gerado:              tabuleiro.c
*  Letras identificadoras:      TAB
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor     Data        Observações
*     6       hpg,gmm   13/jun/2014  Tornou a estrutura auto-verificável
*     5       hpg,gmm   13/jun/2014  Tornou ChecarPos publica
*     4       hpg,gmm   30/abr/2014  Comentários
*     3       gmm       19/abr/2014  Mais implementação
*     2       gmm       18/abr/2014  Mais implementação
*     1       gmm       16/abr/2014  início desenvolvimento
*
***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define tabuleiro_OWN
#include "tabuleiro.h"
#undef tabuleiro_OWN

#ifdef _DEBUG
#include "generico.h"
#include "cespdin.h"
#include "..\\tabelas\\TiposEspacosTabuleiro.def"
#include "conta.h"
#endif

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
    /* Ponteiro para o começo da lista de listas */
};

/***** Protótipos das funções encapuladas no módulo *****/

/***********************************************************************
*
*  $FC Função: TAB  -Destruir linha do tabuleiro
*
***********************************************************************/

static void TAB_DestruirLinha (void *lstLin);

/***********************************************************************
*
*  $FC Função: TAB  -Destruir uma lista por completo
*
*  $ED Descrição da função
*      Destroi por completo uma lista. Destroi os elementos e dps a cabeça da lista
*
***********************************************************************/

static void TAB_DestruirLista (LIS_tppLista lst);

/***********************************************************************
*
*  $FC Função: TAB  -Obter casa
*
*  $ED Descrição da função
*      Retorna uma lista apontando para a posição desejada
*
***********************************************************************/

static LIS_tppLista TAB_ObterCasa (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Função: TAB  -Ir inicio tabuleiro
*
*  $ED Descrição da função
*      Vai para o inicio do tabuleiro, resetando posição onde estava
*
***********************************************************************/

static void TAB_IrInicioTabuleiro (TAB_tppTabuleiro tab);

/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  Função: TAB &Criar tabuleiro
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

#ifdef _DEBUG
    CED_DefinirTipoEspaco(lstCol, TAB_TipoEspacoColuna);
#endif

    /* Cria coluna de referencia para as linhas. se a coluna for null, destroi o tabuleiro e retorna null */

    if (lstCol == NULL)
    {
        free(tab);
        return NULL;
    }

    /* Cria as linhas e as insere na lista coluna. os elementos das listas linhas sao inicializados com null. se ocorrer algum problema de alocacao,
    detrou toda a memoria alocada até entao e retorna null */

    for (i = 0; i < linhas ; i++)
    {
        lstLin = LIS_CriarLista(DestruirPeca);
#ifdef _DEBUG
        CED_DefinirTipoEspaco(lstLin, TAB_TipoEspacoLinha);
#endif
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

    /* Atualiza o tamanho do tabuleiro e seta o ponteiro para a coluna de referência */

    tab->tam.colunas = colunas;
    tab->tam.linhas = linhas;
    tab->coluna = lstCol;

#ifdef _DEBUG
    CED_DefinirTipoEspaco(tab, TAB_TipoEspacoCabeca);
#endif

    return tab;
}

/***********************************************************************
*
*  Função: TAB &Destruir tabuleiro
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
*  Função: TAB &Obter tamanho
*
***********************************************************************/

TAB_tpTamanho TAB_ObterTamanho (TAB_tppTabuleiro tab)
{
    return tab->tam;
}

/***********************************************************************
*
*  Função: TAB &Incluir peca
*
***********************************************************************/

TAB_tpCondRet TAB_IncluirPeca (TAB_tppTabuleiro tab, void *pPeca, TAB_tpPosicao pos)
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
*  Função: TAB &Obter tamanho
*
***********************************************************************/

TAB_tpCondRet TAB_MoverPeca (TAB_tppTabuleiro tab, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    LIS_tppLista lst;
    void *pDado;

    if (tab == NULL)
    {
        return TAB_CondRetTabuleiroVazio;
    }

    if (!TAB_ChecarPos(tab, origem) || !TAB_ChecarPos(tab, destino))
    {
        return TAB_CondRetPosicaoInvalida;
    }

    lst = TAB_ObterCasa(tab, origem);
    pDado = LIS_ObterValor(lst);
    if (pDado == NULL)
    {
        TAB_IrInicioTabuleiro(tab);
        return TAB_CondRetOk;
    }
    LIS_SobrescreverValorCorrente(lst, NULL);
    TAB_IrInicioTabuleiro(tab);
    lst = TAB_ObterCasa(tab, destino);
    LIS_SobrescreverValorCorrente(lst, pDado);
    TAB_IrInicioTabuleiro(tab);
    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Obter peca
*
***********************************************************************/

void *TAB_ObterPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
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
*  Função: TAB &Destruir peca
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
*  Função: TAB &Remover peca
*
***********************************************************************/

void *TAB_RemoverPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
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

/***********************************************************************
*
*  Função: TAB &Checar pos
*
***********************************************************************/

int TAB_ChecarPos (TAB_tppTabuleiro tab, TAB_tpPosicao pos)
{
    if (((tab->tam.colunas <= pos.coluna) || (pos.coluna < 0)) ||
            ((tab->tam.linhas <= pos.linha) || (pos.linha < 0)))
    {
        return FALSE;
    }
    return TRUE;
}


#ifdef _DEBUG

/***********************************************************************
*
*  Função: TAB &Verificar Tabuleiro
*
***********************************************************************/
TAB_tbCondRet TAB_VerificarTabuleiro(TAB_tppTabuleiro tab)
{
    if (tab == NULL)
    {
        TST_NotificarFalha( "Tentou verificar tabuleiro inexistente." );
        return TAB_CondRetErroEstrutura;
    }

    if (TAB_VerificarCabeca(tab) != TAB_CondRetOk)
    {
        TAB_IrInicioTabuleiro(tab);
        return TAB_CondRetErroEstrutura;
    }
    TAB_IrInicioTabuleiro(tab);
    CED_MarcarEspacoAtivo(tab);

    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Verificar Cabeça
*
***********************************************************************/
TAB_tbCondRet TAB_VerificarCabeca(TAB_tppTabuleiro tab)
{
    if (tab == NULL)
    {
        TST_NotificarFalha( "Tentou verificar tabuleiro inexistente." );
        return TAB_CondRetErroEstrutura;
    }

    if (!CED_VerificarEspaco( tab, NULL ))
    {
        TST_NotificarFalha( "Controle do espaço acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    if (TST_CompararInt(TAB_TipoEspacoCabeca,
                        CED_ObterTipoEspaco(tab),
                        "Tipo do espaço de dados não é um tabuleiro.") != TST_CondRetOk )
    {
        return TAB_CondRetErroEstrutura;
    }

    return TAB_VerificarColuna(tab);
}

/***********************************************************************
*
*  Função: TAB &Verificar Coluna
*
***********************************************************************/
TAB_tbCondRet TAB_VerificarColuna(TAB_tppTabuleiro tab)
{
    LIS_tppLista coluna = tab->coluna;
    LIS_tppLista linha;
    TAB_tbCondRet condRetLinha;
    if (coluna == NULL)
    {
        TST_NotificarFalha( "Tabuleiro tem coluna NULL." );
        return TAB_CondRetErroEstrutura;
    }

    if (!CED_VerificarEspaco( coluna, NULL ))
    {
        TST_NotificarFalha( "Controle do espaço acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    if (TST_CompararInt(TAB_TipoEspacoColuna,
                        CED_ObterTipoEspaco(coluna),
                        "Tipo do espaço de dados não é uma coluna.") != TST_CondRetOk )
    {
        return TAB_CondRetErroEstrutura;
    }
    while (LIS_AvancarElementoCorrente(coluna, 1) != CondRetFimLista)
    {
        linha = (LIS_tppLista) LIS_ObterValor(coluna);
        if ((condRetLinha = TAB_VerificarLinha(linha)) != TAB_CondRetOk)
        {
            return TAB_CondRetErroEstrutura;
        }
    }

    CED_MarcarEspacoAtivo(coluna);

    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Verificar Linha
*
***********************************************************************/
TAB_tbCondRet TAB_VerificarLinha(LIS_tppLista linha)
{
    if (linha == NULL)
    {
        TST_NotificarFalha( "Coluna tem linha NULL." );
        return TAB_CondRetErroEstrutura;
    }

    if (!CED_VerificarEspaco( linha, NULL ))
    {
        TST_NotificarFalha( "Controle do espaço acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    if (TST_CompararInt(TAB_TipoEspacoLinha,
                        CED_ObterTipoEspaco(linha),
                        "Tipo do espaço de dados não é uma linha.") != TST_CondRetOk )
    {
        return TAB_CondRetErroEstrutura;
    }

    while (LIS_AvancarElementoCorrente(linha, 1) != CondRetFimLista)
    {
        linha = (LIS_tppLista) LIS_ObterValor(coluna);
        if ((condRetLinha = TAB_VerificarLinha(linha)) != TAB_CondRetOk)
        {
            IrInicioLista(linha);
            return TAB_CondRetErroEstrutura;
        }
    }
    IrInicioLista(linha);

    /* TODO: Verificar Peças */

    CED_MarcarEspacoAtivo(linha);

    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Verificar Peca
*
***********************************************************************/

TAB_tbCondRet TAB_VerificarPeca(PECA_tppPeca peca)
{
    if (peca == NULL)
    {
        return TAB_CondRetOK;
    }

    if (!CED_VerificarEspaco( peca, NULL ))
    {
        TST_NotificarFalha("Controle do espaço acusou erro.");
        return TAB_CondRetErroEstrutura;
    }

    if (TST_CompararInt(TAB_TipoEspacoPeca,
                        CED_ObterTipoEspaco(peca),
                        "Tipo do espaço de dados não é uma peça.") != TST_CondRetOk )
    {
        return TAB_CondRetErroEstrutura;
    }

    if (PECA_ObterCor(peca) != PECA_CorPreta || PECA_ObterCor(peca) != PECA_CorBranca)
    {
        TST_NotificarFalha("Peça tem cor invalida.");
        return TAB_CondRetErroEstrutura;
    }

    if (PECA_ObterStatus(peca) != PECA_StatusNormal || PECA_ObterStatus(peca) != PECA_StatusDama)
    {
        TST_NotificarFalha("Peça tem status invalido.");
        return TAB_CondRetErroEstrutura;
    }

    CED_MarcarEspacoAtivo(peca);
    return TAB_CondRetOk;
}

#endif

/* Código das funções encapsuladas no módulo */


/***********************************************************************
*
*  Função: TAB &Destruir linha
*
***********************************************************************/

static void TAB_DestruirLinha (void *lstLin)
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
*  Função: TAB &Destruir lista
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
*  Função: TAB &Obter casa
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
*  Função: TAB &Ir inicio tabuleiro
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

/********** Fim do módulo de implementação: TAB  tabuleiro de damas ou qualquer outro jogo **********/
