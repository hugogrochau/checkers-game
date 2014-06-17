/***************************************************************************
*  $MCI Módulo de implementacao: TAB  tabuleiro de damas ou qualquer outro jogo
*
*  Arquivo gerado:              tabuleiro.c
*  Letras identificadoras:      TAB
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolucao:
*     Versao  Autor     Data        Observacões
*     8       hpg       14/jun/2014  Contagem
*     7       hpg       14/jun/2014  Deturpacao
*     6       hpg       13/jun/2014  Auto-verificacao
*     5       hpg,gmm   13/jun/2014  Tornou ChecarPos publica
*     4       hpg,gmm   30/abr/2014  Comentarios
*     3       gmm       19/abr/2014  Mais implementacao
*     2       gmm       18/abr/2014  Mais implementacao
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
static char EspacoLixo[ 256 ] =
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;

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
    /* Ponteiro para o comeco da lista de listas */
};

/***** Protótipos das funcões encapuladas no módulo *****/

/***********************************************************************
*
*  $FC Funcao: TAB  -Destruir linha do tabuleiro
*
***********************************************************************/

static void TAB_DestruirLinha (void *lstLin);

/***********************************************************************
*
*  $FC Funcao: TAB  -Destruir uma lista por completo
*
*  $ED Descricao da funcao
*      Destroi por completo uma lista. Destroi os elementos e dps a cabeca da lista
*
***********************************************************************/

static void TAB_DestruirLista (LIS_tppLista lst);

/***********************************************************************
*
*  $FC Funcao: TAB  -Obter casa
*
*  $ED Descricao da funcao
*      Retorna uma lista apontando para a posicao desejada
*
***********************************************************************/

static LIS_tppLista TAB_ObterCasa (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Funcao: TAB  -Ir inicio tabuleiro
*
*  $ED Descricao da funcao
*      Vai para o inicio do tabuleiro, resetando posicao onde estava
*
***********************************************************************/

static void TAB_IrInicioTabuleiro (TAB_tppTabuleiro tab);

/*****  Código das funcões exportadas pelo módulo  *****/


/***********************************************************************
*
*  Funcao: TAB &Criar tabuleiro
*
***********************************************************************/

TAB_tppTabuleiro TAB_CriarTabuleiro (unsigned short int colunas, unsigned short int linhas, void (*DestruirPeca) (void *pDado))
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

#ifdef _DEBUG
    CED_DefinirTipoEspaco(tab, TAB_TipoEspacoCabeca);
#endif

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
#ifdef _DEBUG
            LIS_ColocarCabecaTabuleiro(lstLin, tab, TAB_TipoEspacoElemento);
#endif

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
    return tab;
}

/***********************************************************************
*
*  Funcao: TAB &Destruir tabuleiro
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
*  Funcao: TAB &Obter tamanho
*
***********************************************************************/

TAB_tpTamanho TAB_ObterTamanho (TAB_tppTabuleiro tab)
{
    return tab->tam;
}

/***********************************************************************
*
*  Funcao: TAB &Incluir peca
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
    return TAB_CondRetOK;
}

/***********************************************************************
*
*  Funcao: TAB &Obter tamanho
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
        return TAB_CondRetOK;
    }
    LIS_SobrescreverValorCorrente(lst, NULL);
    TAB_IrInicioTabuleiro(tab);
    lst = TAB_ObterCasa(tab, destino);
    LIS_SobrescreverValorCorrente(lst, pDado);
    TAB_IrInicioTabuleiro(tab);
    return TAB_CondRetOK;
}

/***********************************************************************
*
*  Funcao: TAB &Obter peca
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
*  Funcao: TAB &Destruir peca
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
    return TAB_CondRetOK;
}

/***********************************************************************
*
*  Funcao: TAB &Remover peca
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
*  Funcao: TAB &Checar pos
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
/* Código das funcões de instrumentacao */

/***********************************************************************
*
*  Funcao: TAB &Verificar Tabuleiro
*
***********************************************************************/
TAB_tpCondRet TAB_VerificarTabuleiro(TAB_tppTabuleiro tab)
{
    CNT_CONTAR("Verificar tabuleiro");

    if (tab == NULL)
    {

        CNT_CONTAR("Tabuleiro nulo");

        TST_NotificarFalha( "Tentou verificar tabuleiro inexistente." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tabuleiro nao nulo");

    CED_MarcarEspacoAtivo(tab);

    if (!CED_VerificarEspaco( tab, NULL ))
    {

        CNT_CONTAR("Espaco tabuleiro invalido");

        TST_NotificarFalha( "Controle do espaco acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaco tabuleiro valido");

    if (TST_CompararInt(TAB_TipoEspacoCabeca,
                        CED_ObterTipoEspaco(tab),
                        "Tipo do espaco de dados nao é um tabuleiro.") != TST_CondRetOK )
    {

        CNT_CONTAR("Tipo tabuleiro invalido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo tabuleiro valido");

    if (TAB_VerificarColuna(tab) != TAB_CondRetOK)
    {
        CNT_CONTAR("Coluna invalida");
        TAB_IrInicioTabuleiro(tab);
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Coluna valida");

    TAB_IrInicioTabuleiro(tab);

    CNT_CONTAR("Acaba verificar tabuleiro");

    return TAB_CondRetOK;
}

/***********************************************************************
*
*  Funcao: TAB &Verificar Coluna
*
***********************************************************************/
TAB_tpCondRet TAB_VerificarColuna(TAB_tppTabuleiro tab)
{
    LIS_tppLista coluna = tab->coluna;
    LIS_tppLista linha;
    TAB_tpCondRet condRetLinha;

    CNT_CONTAR("Verificar coluna");

    if (coluna == NULL)
    {

        CNT_CONTAR("Coluna nula");

        TST_NotificarFalha( "Tabuleiro tem coluna NULL." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Coluna nao nula");

    if (!CED_VerificarEspaco( coluna, NULL ))
    {

        CNT_CONTAR("Espaco coluna invalido");

        TST_NotificarFalha( "Controle do espaco acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaco coluna valido");

    if (TST_CompararInt(TAB_TipoEspacoColuna,
                        CED_ObterTipoEspaco(coluna),
                        "Tipo do espaco de dados nao é uma coluna.") != TST_CondRetOK )
    {

        CNT_CONTAR("Tipo coluna invalido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo coluna valido");

    do {

        linha = (LIS_tppLista) LIS_ObterValor(coluna);
        if ((condRetLinha = TAB_VerificarLinha(linha)) != TAB_CondRetOK)
        {

            CNT_CONTAR("Linha invalida");

            return TAB_CondRetErroEstrutura;
        }

        CNT_CONTAR("Linha valida");

        CNT_CONTAR("Avanca coluna");

    } while (LIS_AvancarElementoCorrente(coluna, 1) != LIS_CondRetFimLista);


    CNT_CONTAR("Linhas validas");


    CED_MarcarEspacoAtivo(coluna);

    CNT_CONTAR("Acaba verificar coluna");

    return TAB_CondRetOK;
}

/***********************************************************************
*
*  Funcao: TAB &Verificar Linha
*
***********************************************************************/

TAB_tpCondRet TAB_VerificarLinha(LIS_tppLista linha)
{
    LIS_tpCondRet condRetLinha;

    CNT_CONTAR("Verificar linha");

    if (linha == NULL)
    {

        CNT_CONTAR("Linha nula");

        TST_NotificarFalha( "Coluna tem linha NULL." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Linha nao nula");

    if (!CED_VerificarEspaco( linha, NULL ))
    {

        CNT_CONTAR("Espaco linha invalido");

        TST_NotificarFalha( "Controle do espaco acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaco linha valido");

    if (TST_CompararInt(TAB_TipoEspacoLinha,
                        CED_ObterTipoEspaco(linha),
                        "Tipo do espaco de dados nao é uma linha.") != TST_CondRetOK )
    {

        CNT_CONTAR("Tipo linha invalido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo linha valido");

    do {

        if ((condRetLinha = LIS_VerificarElemento(linha)) != TAB_CondRetOK)
        {

            CNT_CONTAR("Elemento invalido");

            IrInicioLista(linha);
            return TAB_CondRetErroEstrutura;
        }

        CNT_CONTAR("Elemento valido");

        CNT_CONTAR("Avanca linha");

    } while (LIS_AvancarElementoCorrente(linha, 1) != LIS_CondRetFimLista);

    CNT_CONTAR("Elementos validos");

    IrInicioLista(linha);

    CED_MarcarEspacoAtivo(linha);

    CNT_CONTAR("Acaba verificar linha");

    return TAB_CondRetOK;
}



/***********************************************************************
*
*  Funcao: TAB &Deturpar tabuleiro
*
***********************************************************************/

void TAB_Deturpar( TAB_tppTabuleiro tab, TAB_tpModoDeturpacao modoDeturpar)
{
    LIS_tppLista coluna = tab->coluna;
    LIS_tppLista linha = (LIS_tppLista) LIS_ObterValor(coluna);
    if (tab == NULL || coluna == NULL || linha == NULL)
    {
        return;
    }

    switch (modoDeturpar)
    {
    /* tipos */
    case DeturpaTipoTabuleiro:
        CED_DefinirTipoEspaco(tab, CED_ID_TIPO_VALOR_NULO);
        break;

    case DeturpaTipoColuna:
        CED_DefinirTipoEspaco(coluna, CED_ID_TIPO_VALOR_NULO);
        break;

    case DeturpaTipoLinha:
        CED_DefinirTipoEspaco(linha, CED_ID_TIPO_VALOR_NULO);
        break;

    /* espacos */
    case DeturpaEspacoTabuleiro:
        memcpy((char *) tab,
               LIXO(8),
               8);
        break;

    case DeturpaEspacoColuna:
        memcpy((char *) coluna,
               LIXO(12),
               12);
        break;

    case DeturpaEspacoLinha:
        memcpy((char *) linha,
               LIXO(12),
               12);
        break;

    /* ponteiro nulo */
    case DeturpaPtColunaNulo:
        tab->coluna = NULL;
        break;

    case DeturpaPtLinhaNulo:
        LIS_SobrescreverValorCorrente(coluna, NULL);
        break;

    /* ponteiro para lixo */
    case DeturpaColunaLixo:
        tab->coluna = (LIS_tppLista) EspacoLixo;
        break;

    case DeturpaLinhaLixo:
        LIS_SobrescreverValorCorrente(coluna, EspacoLixo);
        break;
    
    /* elemento */
    default:
        LIS_DeturparElemento(linha, modoDeturpar);
    }
}

#endif

/* Código das funcões encapsuladas no módulo */


/***********************************************************************
*
*  Funcao: TAB &Destruir linha
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
*  Funcao: TAB &Destruir lista
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
*  Funcao: TAB &Obter casa
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
*  Funcao: TAB &Ir inicio tabuleiro
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

/********** Fim do módulo de implementacao: TAB  tabuleiro de damas ou qualquer outro jogo **********/
