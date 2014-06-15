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
*     8       hpg       14/jun/2014  Implementação de contagem
*     7       hpg       14/jun/2014  Implementação de deturpação
*     6       hpg       13/jun/2014  Tornou a estrutura auto-verificável
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
#define TAM_ELEM_LIS 12
#define MIN_TAM_PECA 2
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

#ifdef _DEBUG
    CED_DefinirTipoEspaco(pPeca, TAB_TipoEspacoPeca);
#endif

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
/* Código das funções de instrumentação */

/***********************************************************************
*
*  Função: TAB &Verificar Tabuleiro
*
***********************************************************************/
TAB_tbCondRet TAB_VerificarTabuleiro(TAB_tppTabuleiro tab)
{
    CNT_CONTAR("Verificar tabuleiro");

    if (tab == NULL)
    {

        CNT_CONTAR("Tabuleiro nulo");

        TST_NotificarFalha( "Tentou verificar tabuleiro inexistente." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tabuleiro não nulo");

    CED_MarcarEspacoAtivo(tab);

    if (!CED_VerificarEspaco( tab, NULL ))
    {

        CNT_CONTAR("Espaço tabuleiro inválido");

        TST_NotificarFalha( "Controle do espaço acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaço tabuleiro válido");

    if (TST_CompararInt(TAB_TipoEspacoCabeca,
                        CED_ObterTipoEspaco(tab),
                        "Tipo do espaço de dados não é um tabuleiro.") != TST_CondRetOk )
    {

        CNT_CONTAR("Tipo tabuleiro inválido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo tabuleiro válido");

    if (TAB_VerificarColuna(tab) != TAB_CondRetOk)
    {
        CNT_CONTAR("Coluna inválida");
        TAB_IrInicioTabuleiro(tab);
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Coluna válida");

    TAB_IrInicioTabuleiro(tab);

    CNT_CONTAR("Acaba verificar tabuleiro");

    return CondRetOk;
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

    CNT_CONTAR("Verificar coluna");

    if (coluna == NULL)
    {

        CNT_CONTAR("Coluna nula");

        TST_NotificarFalha( "Tabuleiro tem coluna NULL." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Coluna não nula");

    if (!CED_VerificarEspaco( coluna, NULL ))
    {

        CNT_CONTAR("Espaço coluna inválido");

        TST_NotificarFalha( "Controle do espaço acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaço coluna válido");

    if (TST_CompararInt(TAB_TipoEspacoColuna,
                        CED_ObterTipoEspaco(coluna),
                        "Tipo do espaço de dados não é uma coluna.") != TST_CondRetOk )
    {

        CNT_CONTAR("Tipo coluna inválido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo coluna válido");

    while (LIS_AvancarElementoCorrente(coluna, 1) != CondRetFimLista)
    {

        CNT_CONTAR("Avança coluna");

        linha = (LIS_tppLista) LIS_ObterValor(coluna);
        if ((condRetLinha = TAB_VerificarLinha(linha)) != TAB_CondRetOk)
        {

            CNT_CONTAR("Linha inválida");

            return TAB_CondRetErroEstrutura;
        }

        CNT_CONTAR("Linha válida");

    }

    CNT_CONTAR("Linhas válidas");


    CED_MarcarEspacoAtivo(coluna);

    CNT_CONTAR("Acaba verificar coluna");

    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Verificar Linha
*
***********************************************************************/
TAB_tbCondRet TAB_VerificarLinha(LIS_tppLista linha)
{

    CNT_CONTAR("Verificar linha");

    PECA_tppPeca peca;
    if (linha == NULL)
    {

        CNT_CONTAR("Linha nula");

        TST_NotificarFalha( "Coluna tem linha NULL." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Linha não nula");

    if (!CED_VerificarEspaco( linha, NULL ))
    {

        CNT_CONTAR("Espaço linha inválido");

        TST_NotificarFalha( "Controle do espaço acusou erro." );
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaço linha válido");

    if (TST_CompararInt(TAB_TipoEspacoLinha,
                        CED_ObterTipoEspaco(linha),
                        "Tipo do espaço de dados não é uma linha.") != TST_CondRetOk )
    {

        CNT_CONTAR("Tipo linha inválido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo linha válido");

    while (LIS_AvancarElementoCorrente(linha, 1) != CondRetFimLista)
    {

        CNT_CONTAR("Avança linha");

        peca = (PECA_tppPeca) LIS_ObterValor(peca);
        if ((condRetLinha = TAB_VerificarPeca(peca)) != TAB_CondRetOk)
        {

            CNT_CONTAR("Peça inválida");

            IrInicioLista(linha);
            return TAB_CondRetErroEstrutura;
        }

        CNT_CONTAR("Peça válida");

    }

    CNT_CONTAR("Peças válidas");

    IrInicioLista(linha);

    /* TODO: Verificar Peças */

    CED_MarcarEspacoAtivo(linha);

    CNT_CONTAR("Acaba verificar linha");

    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Verificar Peca
*
***********************************************************************/

TAB_tbCondRet TAB_VerificarPeca(PECA_tppPeca peca)
{

    CNT_CONTAR("Verificar peça");

    if (peca == NULL)
    {

        CNT_CONTAR("Peça nula");

        return TAB_CondRetOK;
    }

    CNT_CONTAR("Peça não nula");

    if (!CED_VerificarEspaco( peca, NULL ))
    {

        CNT_CONTAR("Espaço peça inválido");

        TST_NotificarFalha("Controle do espaço acusou erro.");
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Espaço peça válido");

    if (TST_CompararInt(TAB_TipoEspacoPeca,
                        CED_ObterTipoEspaco(peca),
                        "Tipo do espaço de dados não é uma peça.") != TST_CondRetOk )
    {

        CNT_CONTAR("Tipo peça inválido");

        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Tipo peça válido");

    if (PECA_ObterCor(peca) != PECA_CorPreta || PECA_ObterCor(peca) != PECA_CorBranca)
    {

        CNT_CONTAR("Cor peça inválida");

        TST_NotificarFalha("Peça tem cor invalida.");
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Cor peça válida");

    if (PECA_ObterStatus(peca) != PECA_StatusNormal || PECA_ObterStatus(peca) != PECA_StatusDama)
    {

        CNT_CONTAR("Status peça inválida");

        TST_NotificarFalha("Peça tem status invalido.");
        return TAB_CondRetErroEstrutura;
    }

    CNT_CONTAR("Status peça válida");

    CED_MarcarEspacoAtivo(peca);

    CNT_CONTAR("Acaba verificar peça");

    return TAB_CondRetOk;
}

/***********************************************************************
*
*  Função: TAB &Deturpar tabuleiro
*
***********************************************************************/

void TAB_Deturpar( TAB_tppTabuleiro tab, TAB_tpModosDeturpacao modoDeturpar)
{
    LIS_tppLista coluna = tab->coluna;
    LIS_tppLista linha = (LIS_tppLista) LIS_ObterValor(coluna);
    PECA_tppPeca peca = NULL;
    if (tab == NULL || coluna == NULL || linha == NULL)
    {
        return;
    }

    while (peca == NULL && LIS_AvancarElementoCorrente(linha, 1) != CondRetFimLista)
    {
         peca = (PECA_tppPeca) LIS_ObterValor(linha);
    }
    IrInicioLista(linha);
    if (peca == NULL) /* não existe uma peça na primeira linha */
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

    case DeturpaTipoPeca:
        CED_DefinirTipoEspaco(peca, CED_ID_TIPO_VALOR_NULO);
        break;

    /* espaços */
    case DeturparEspacoTabuleiro:
        memcpy((char *) tab,
               LIXO(sizeof(TAB_tagTabuleiro)),
               sizeof(TAB_tagTabuleiro));
        break;

    case DeturpaEspacoColuna:
        memcpy((char *) coluna,
               LIXO(TAM_ELEM_LIS),
               TAM_ELEM_LIS);
        break;

    case DeturpaEspacoLinha:
        memcpy((char *) linha,
           LIXO(TAM_ELEM_LIS),
           TAM_ELEM_LIS);
        break;

    case DeturpaEspacoPeca:
        memcpy((char *) peca,
        LIXO(MIN_TAM_PECA),
        MIN_TAM_PECA);
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

    case DeturpaPecaLixo:
        LIS_SobrescreverValorCorrente(linha, EspacoLixo);
        break;

    case DeturpaPecaStatus:
        peca->status = -1;
        break;

    case DeturpaPecaCor:
        peca->cor = -1;
        break;
    }
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
