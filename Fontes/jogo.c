/***************************************************************************
*  $MCI Módulo de implementação: JOGO  gerenciador do jogo de damas
*
*  Arquivo gerado:              jogo.c
*  Letras identificadoras:      JOGO
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor     Data        Observações
*     1       hpg/gmm   09/jun/2014 início desenvolvimento
*     2       hpg/gmm   13/jun/2014 início da lógica de movimento
*
***************************************************************************/


#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "jogo.h"
#include "tabuleiro.h"
#include "peca.h"

#define jogo_OWN
#include "jogo.h"
#undef jogo_OWN

#define TRUE  1
#define FALSE 0
#define PATH_TAB_INICIAL "Objetos/TabuleiroInicial.tab"
#define LINHAS 8
#define COLUNAS 8

/***********************************************************************
*
*  $TC Tipo de dados: JOGO jogo
*
*
*  $ED Descrição do tipo
*     Dados sobre o jogo de damas
*
***********************************************************************/

typedef struct JOGO_tagJogo
{

    TAB_tppTabuleiro tabuleiro ;
    /* Tabuleiro do jogo */

    JOGO_tppJogador jogador1;
    /* Primeiro jogador */

    JOGO_tppJogador jogador2;
    /* Segundo jogador */

    JOGO_tppJogador jogadarDaVez; 

} JOGO_tpJogo ;

/* Tipo referência para um jogador */
typedef JOGO_tpJogador *JOGO_tppJogador;

/***********************************************************************
*
*  $TC Tipo de dados: JOGO jogador
*
*
*  $ED Descrição do tipo
*     Dados de um dos jogadores
*
***********************************************************************/

typedef struct JOGO_tagJogador
{
    char *nome ;
    /* Nome do jogador */

    PECA_tpCor cor ;
    /* Cor do jogador */

    unsigned int jogadas ;
    /* Número de jogadas */

} JOGO_tpJogador


/***** Protótipo das funções encapsuladas no módulo *****/

/***********************************************************************
*
*  $FC Função: JOGO  -Criar jogador
*
*  $ED Descrição da função
*      Cria um jogador, dando um nome e uma cor a ele(a).
*
*  $EP Parâmetros
*     Nome - String contendo o nome do jogador.
*     cor - A cor das peças do jogador (pretas ou brancas).
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o jogador.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL.
*     Não será dada mais informação quanto ao problema ocorrido.
*
***********************************************************************/

static JOGO_tppJogador JOGO_CriarJogador (char *Nome, PECA_tpCor cor);

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir jogador
*
*  $ED Descrição da função
*      Deleta um jogador, limpado o espaço de memória alocado para ele
*      e seu nome.
*
*  $EP Parâmetros
*     jogador - referência para o tipo jogador a ser deletado.
*
***********************************************************************/

static void JOGO_DestruirJogador (JOGO_tppJogador jogador);

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Cor Peça
*
*  $ED Descrição da função
*      Calcula com que cor a peça será imprimida dependendo do seu status
*      e cor.
*
*  $EP Parâmetros
*     peca - a peça que está sendo impressa.
*  
*  $FV Valor retornado
*     FOREGROUND_BLUE se a peça for azul
*     FOREGROUND_RED se a peça for vermelha
*     COR | FOREGROUND_INTENSITY se a peça for uma dama
*     NULL se a peça tiver uma cor inválida
*
***********************************************************************/

static WORD JOGO_ObterCorPeca(PECA_tppPeca peca);

static int JOGO_MovimentoValido(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino);

static int JOGO_PodeComer(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)


/* Código das funções exportadas pelo módulo */


/***********************************************************************
*
*  $FC Função: JOGO  -Criar jogo
*
***********************************************************************/

JOGO_tppJogo JOGO_CriarJogo (char *nomeJogador1, char *nomeJogador2)
{
    JOGO_tppJogo jogo;
    JOGO_tppJogador jogador1, jogador2;

    jogo = (JOGO_tppJogo) malloc(sizeof(JOGO_tpJogo));
    if (jogo == NULL)
    {
        return NULL;
    }
    jogo->jogador1 = JOGO_CriarJogador(nomeJogador1, PECA_CorBranca);
    jogo->jogador2 = JOGO_CriarJogador(nomeJogador2, PECA_CorPreta);
    if (jogo->jogador1 == NULL || jogo->jogador2 == NULL)
    {
        JOGO_DestruirJogo(jogo);
        return NULL;
    }

    jogo->tabuleiro = TAB_CriarTabuleiro(COLUNAS, LINHAS, PECA_DestruirPeca);
    if (jogo->tabuleiro == NULL)
    {
        JOGO_DestruirJogo(jogo);
        return NULL;
    }

    JOGO_PreencherTabuleiro(jogo, fopen(PATH_TAB_INICIAL, "r"));
}

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir Jogo
*
***********************************************************************/

void JOGO_DestruirJogo (JOGO_tppJogo jogo)
{
    if (jogo != NULL)
    {
        TAB_DestruirTabuleiro(jogo->tabuleiro);
        jogo->tabuleiro = NULL;
        JOGO_DestruirJogador(jogo->jogador1);
        jogo->jogador1 = NULL;
        JOGO_DestruirJogador(jogo->jogador2);
        jogo->jogador2 = NULL;
        free(jogo)
        jogo = NULL;
    }
}

/***********************************************************************
*
*  $FC Função: JOGO  -Preencher tabuleiro
*
***********************************************************************/

JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro )
{
    int i, j;
    PECA_tpStatus status;
    PECA_tpCor cor;
    PECA_tppPeca peca;
    TAB_tpPosicao pos;
    TAB_tpCondRet tabCondRet;

    if (fp == NULL)
    {
        return JOGO_CondRetArquivoInvalido;
    }

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            if (fscanf(fp, " %d %d ", &status, &cor) == EOF)
            {
                return JOGO_CondRetArquivoCorrompido;
            }
            pos.linha = i;
            pos.coluna = j;
            peca = PECA_CriarPeca(cor);
            if (status == PECA_StatusDama)
            {
                PECA_VirarDama(peca);
            }
            if (TAB_IncluirPeca(jogo->tabuleiro, peca, pos) == TAB_CondRetPosicaoInvalida)
            {
                return JOGO_CondRetArquivoCorrompido;
            }
        }
    }

    fclose(fp);

    return JOGO_CondRetOk;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Imprimir tabuleiro
*
***********************************************************************/

JOGO_tpCondRet JOGO_ImprimirTabuleiro (JOGO_tppJogo jogo)
{
    int i, j;
    PECA_tpStatus status;
    PECA_tpCor cor;
    PECA_tppPeca peca;
    TAB_tpPosicao pos;
    TAB_tpCondRet tabCondRet;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }

    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            pos.linha = i;
            pos.coluna = j;
            peca = (PECA_tppPeca) TAB_ObterPeca(jogo->tabuleiro, pos);
            SetConsoleTextAttribute(hConsole, JOGO_ObterCorPeca(peca));
            printf(" ø ");
        }
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, saved_attributes);
    return JOGO_CondRetOk;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Executar Jogada
*
***********************************************************************/

JOGO_tpCondRet JOGO_ExecutarJogada(JOGO_tppJogo jogo)
{
    char temp1, temp2;
    TAB_tpPosicao origem, destino;
    PECA_tppPeca = pecaMov;

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }

    printf(" Escolha sua jogada (ex: 3a 5b): ");
    scanf(" %hd%c %hd%c", &origem.linha, &temp1,
                        &destino.linha,  &temp2);

    /* convertendo as letras das colunas coordenadas */
    tolower(temp1);
    tolower(temp2);
    temp1 -= 'a'; 
    temp2 -= 'a';
    origem.coluna = (short int) temp1;
    destino.coluna = (short int) temp2;
    
    if (!JOGO_MovimentoValido(jogo, origem, destino))
    {
        return JOGO_CondRetJogadaInvalida;
    }

    switch (TAB_MoverPeca(jogo->tabuleiro, origem, destino))
    {
    case TAB_CondRetTabuleiroVazio:
        return JOGO_CondRetJogoNaoInicializado;
    case TAB_CondRetPosicaoInvalida:
        return JOGO_CondRetJogadaInvalida;
    }
    
    return JOGO_CondRetOk;
}

/* Código das funções encapsuladas no módulo */

/***********************************************************************
*
*  $FC Função: JOGO  -Criar jogador
*
***********************************************************************/

static JOGO_tppJogador JOGO_CriarJogador (char *nome, PECA_tpCor cor)
{
    JOGO_tppJogador jogador = (JOGO_tppJogador) malloc (sizeof(JOGO_tppJogador));
    size_t strsize = 0;
    if (jogador == NULL)
        return NULL;

    strsize = strlen(nome);
    jogador->nome = (char *) malloc (strsize);

    if (jogador->nome == NULL)
    {
        free(jogador);
        return NULL;
    }

    strcpy(jogador->nome, nome);
    jogador->cor = cor;
    jogador->jogadas = 0;
    return jogador;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir Jogador
*
***********************************************************************/

static void JOGO_DestruirJogador (JOGO_tppJogador jogador)
{
    if (jogador != NULL)
    {
        free(jogador->nome);
        jogador->nome = NULL;
        free(jogador);
        jogador = NULL;
    }
}

/***********************************************************************
*
*  $FC Função: JOGO  -Obter cor peca
*
***********************************************************************/

WORD JOGO_ObterCorPeca(PECA_tppPeca peca) 
{
    WORD cor = NULL;
    if (PECA_ObterCor(peca) == PECA_CorBranca)
    {
        cor = FOREGROUND_BLUE;
    }
    else if (PECA_ObterCor(peca) == PECA_CorPreta)
    {
        cor = FOREGROUND_RED;
    }
    if (PECA_ObterStatus(peca) == PECA_StatusDama)
    {    
        cor |= FOREGROUND_INTENSITY;
    }
    return cor;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Forçado a comer
*
***********************************************************************/

static int JOGO_MovimentoValido(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    pecaMov = (PECA_tppPeca *) TAB_ObterPeca(jogo->tabuleiro, origem);

    /* conferindo se a peça existe ou se já existe alguma peça no destino */
    if (pecaMov == NULL || TAB_ObterPeca(jogo->tabuleiro, destino) != NULL))
    {
        return FALSE;
    }

    if (PECA_ObterCor(pecaMov) != jogadorDaVez->cor)
    {
        return FALSE;
    }

    if (PECA_ObterCor(pecaMov) == PECA_CorPreta)
    {
        if (PECA_ObterStatus(pecaMov) == PECA_StatusNormal)
        {
            if (destino.linha < origem.linha)
            {
                return FALSE;
            }
            if (JOGO_PodeComer(jogo, origem, destino))
            {
                return TRUE;
            }
        }
    }
    else if (PECA_ObterCor(pecaMov) == PECA_CorBranca)
    {
        if (PECA_ObterStatus(pecaMov) == PECA_StatusNormal)
        {
            if (destino.linha > origem.linha)
            {
                return FALSE;
            }
        }
    }

    return FALSE;

}

/***********************************************************************
*
*  $FC Função: JOGO  -Pode comer
*
***********************************************************************/
static int JOGO_PodeComer(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    typedef enum {
        NE = 1,
        /* nordeste*/
        SE = 2,
        /* sudeste*/
        SO = 3,
        /* sudoeste*/
        NO = 4
        /* noroeste*/
    } tpDirecao;

    tpDirecao direcao;
    int achouPeca = 0;

    if (destino.linha > origem.linha) {
        if (destino.coluna > origem.coluna)
        {
            direcao = SE;
        } 
        else
        {
            direco = SO;
        }
    }
    else
    {
        if (destino.coluna > origem.coluna)
        {
            direcao = NE;
        } 
        else
        {
            direco = NO;
        }
    }

    while (origem.linha != destino.linha && origem.coluna != destino.coluna)
    {
        switch (direcao)
        {
            case NE:
                origem.linha--;
                origem.coluna++;
                break;
            case SE:
                origem.linha++;
                origem.coluna++;
                break;
            case SO:
                origem.linha++;
                origem.coluna--;
                break;
            case NO:
                origem.linha--;
                origem.coluna--;
                break;

        }

        if (TAB_ObterPeca(jogo->tabuleiro, origem) != NULL)
        {
            /* Já tinha achado uma peça na última iteração */
            if (achouPeca)
            {
                return FALSE;
            }
            achouPeca = true;
        } 
        else
        {
            if (achouPeca)
            {
                return TRUE;
            }
        }

        /* Chegou no final do tabuleiro */
        if (!TAB_ChecarPos(jogo->tabuleiro, origem))
        {
            return FALSE;
        }
    }
}
/********** Fim do módulo de implementação: JOGO  gerenciador do jogo de damas **********/
