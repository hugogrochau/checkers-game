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
*     4       hpg       15/jun/2014 Termina desenvolvimento
*     3       lr        14/jun/2014 Documentação
*     2       hpg/gmm   13/jun/2014 Início da lógica de movimento
*     1       hpg/gmm   09/jun/2014 Início desenvolvimento
*
***************************************************************************/


#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "jogo.h"
#include "tabuleiro.h"
#include "peca.h"

#define jogo_OWN
#include "jogo.h"
#undef jogo_OWN

#define TRUE  1
#define FALSE 0
#define PATH_TAB_INICIAL "TabuleiroInicial.tab"
#define LINHAS 8
#define COLUNAS 8
#define NUM_PECAS_INI 12;

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
    TAB_tppTabuleiro tab;
    /* Tabuleiro do jogo */

    JOGO_tppJogador jogador1;
    /* Primeiro jogador */

    JOGO_tppJogador jogador2;
    /* Segundo jogador */

    JOGO_tppJogador jogadorDaVez;
} JOGO_tpJogo;

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
    char *nome;
    /* Nome do jogador */

    PECA_tpCor cor;
    /* Cor do jogador */

    unsigned int jogadas;
    /* Número de jogadas */

    unsigned int numPecas;
    /* Número de peças restantes */
} JOGO_tpJogador;

typedef struct
{
    short int x;

    short int y;
} JOGO_tpDirecao;


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

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Direcao
*
*  $ED Descrição da função
*     Obtém a direção que a peça irá mover
*
*  $EP Parâmetros
*     origem- posição de origem da peça
*     destino- posição de destino da peça
*
*  $FV Valor retornado
*     Retorna a direção a qual a peça irá mover
*     (nordeste, sudeste, sudoeste ou noroeste)
*     NE = { 1, 1}
*     SE = { 1,-1}
*     SO = {-1,-1}
*     NO = {-1, 1}
*
***********************************************************************/

static JOGO_tpDirecao JOGO_ObterDirecao (TAB_tpPosicao origem, TAB_tpPosicao destino);

/***********************************************************************
*
*  $FC Função: JOGO  -Valida Diagonal
*
*  $ED Descrição da função
*      Valida se a diagonal na qual a peça irá mover é válida
*
*  $EP Parâmetros
*     origem - posição de origem da peça
*     destino - posição de destino da peça
*
*  $FV Valor retornado
*     TRUE se a diagonal for válida
*     FALSE se a diagonal não for válida
*
***********************************************************************/

static int JOGO_ValidaDiagonal(TAB_tpPosicao origem, TAB_tpPosicao destino);

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Distancia Diagonal
*
*  $ED Descrição da função
*     Calcula o tamanho da distância da diagonal
*
*  $EP Parâmetros
*     origem - posição de origem da peça
*     destino - posição de destino da peça
*
*  $FV Valor retornado
*     Retorna a distância da diagonal entre a origem e o destino
*
***********************************************************************/

static int JOGO_ObterDistanciaDiagonal(TAB_tpPosicao origem, TAB_tpPosicao destino);

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Quantidade Pecas No Caminho
*
*  $ED Descrição da função
*      Calcula a quantidade de peças que a peça movida tem pelo caminho
*      até seu destino
*
*  $EP Parâmetros
*     jogo - referência para o jogo em que irá se calcular o número de
*            peças pelo caminho
*     origem - posição de origem da peça
*     destino - posição de destino da peça
*
*  $FV Valor retornado
*     Retorna um inteiro referente ao número de peças pelo caminho da
*     peça movimentada até seu destino
*
***********************************************************************/

static int JOGO_ObterQuantidadePecasNoCaminho(JOGO_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino);

/***********************************************************************
*
*  $FC Função: JOGO  -Tentar Jogada
*
*  $ED Descrição da função
*      Decide o que vai acontecer para uma certa jogada
*
*  $EP Parâmetros
*     jogo - referência para o jogo em que iremos avaliar a jogada
            realizada
*     origem - posição de origem da peça
*     destino- posição de destino da peça
*
*  $FV Valor retornado
*     CondRetJogadaInvalida - Se a jogada for inválida
*     CondRetComeuPeca - Se a jogada for válida e uma peça foi
*                             comida
*     CondRetJogadaInvalida - Se jogada for válida e nenhuma peça
*                                  foi comida
*
***********************************************************************/

static JOGO_tpCondRet JOGO_TentarJogada(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino);

/***********************************************************************
*
*  $FC Função: JOGO  -Pode Comer
*
*  $ED Descrição da função
*      Verifica se no movimento realizado, a peça movimentada pode comer
*      alguma outra peça
*
*  $EP Parâmetros
*     jogo - referência para o jogo em que iremos se o movimento feito
*            pela peça irá resultar em uma captura de alguma outra peça
*            do adversário
*     origem - posição de origem da peça
*     destino - posição de destino da peça
*
*  $FV Valor retornado
*     TRUE se o jogador puder comer a peça do adversário
*     FALSE se o jogador não puder comer a peça do adversário
*
***********************************************************************/

static int JOGO_PodeComer(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino);

/***********************************************************************
*
*  $FC Função: JOGO  -Avançar Posição
*
*  $ED Descrição da função
*     Avança a posição na diagonal especificada pela direção
*
*  $EP Parâmetros
*     direcao - a direção na qual a posição será avançada
*     posicao - a posição que sera avançada
*
*  $FV Valor retornado
*     A nova posição avançada
*
***********************************************************************/

static JOGO_tpPosicao JOGO_AvancarPosicao(JOGO_tpPosicao posicao, JOGO_tpDirecao direcao);


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

    jogo->jogadorDaVez = jogador1;

    jogo->tab = TAB_CriarTabuleiro(COLUNAS, LINHAS, PECA_DestruirPeca);
    if (jogo->tab == NULL)
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
        TAB_DestruirTabuleiro(jogo->tab);
        jogo->tab = NULL;
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
            if ((cor == PECA_CorBranca || cor == PECA_CorPreta) && (status == PECA_StatusNormal || status == PECA_StatusDama))
            {
                pos.linha = i;
                pos.coluna = j;
                peca = PECA_CriarPeca(cor);
                if (status == PECA_StatusDama)
                {
                    PECA_VirarDama(peca);
                }
                if (TAB_IncluirPeca(jogo->tab, peca, pos) == TAB_CondRetPosicaoInvalida)
                {
                    return JOGO_CondRetArquivoCorrompido;
                }
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
            peca = (PECA_tppPeca) TAB_ObterPeca(jogo->tab, pos);
            if (peca != NULL)
            {
                SetConsoleTextAttribute(hConsole, JOGO_ObterCorPeca(peca));
                printf(" ø ");
            }
            else
            {
                printf("   ");
            }
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

JOGO_tpCondRet JOGO_ExecutarJogada(JOGO_tppJogo jogo,
                                   short int linhaOrigem, short int colunaOrigem,
                                   short int linhaDestino, short int colunaDestino)
{
    char temp1, temp2;
    TAB_tpPosicao origem, destino;
    PECA_tppPeca = pecaMov;
    JOGO_tpCondRet condRetJogada;
    JOGO_tpDirecao direcao;

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }

    origem.coluna = (short int) temp1;
    destino.coluna = (short int) temp2;

    condRetJogada = JOGO_TentarJogada(jogo, origem, destino);

    if (condRetJogada == JOGO_CondRetJogadaInvalida)
    {
        return condRetJogada;
    }

    switch (TAB_MoverPeca(jogo->tab, origem, destino))
    {
    case TAB_CondRetTabuleiroVazio:
        return JOGO_CondRetJogoNaoInicializado;
    case TAB_CondRetPosicaoInvalida:
        return JOGO_CondRetJogadaInvalida;
    }

    if (condRetJogada == JOGO_CondRetComeuPeca)
    {
        direcao = JOGO_ObterDirecao(origem, destino);
        /* move destino para a peça que foi comida */
        destino = JOGO_AvancarPosicao(destino, direcao);
        TAB_DestruirPeca(jogo->tab, destino);
        ObterJogadorNaoDaVez(jogo)->numPecas--;
    }
    else
    {
        /* se nenhuma peça for comida, é a vez do outro jogador */
        jogo->jogadorDaVez = ObterJogadorNaoDaVez(jogo);
    }

    jogo->jogadorDaVez->jogadas++;

    if (jogo->jogador1->numPecas == 0)
    {
        return TAB_CondRetJogador1Ganhou;
    }

    if (jogo->jogador2->numPecas == 0)
    {
        return TAB_CondRetJogador2Ganhou;
    }

    if (jogo->jogador1->numPecas == jogo->jogador2->numPecas == 1)
    {
        return TAB_CondRetEmpate;
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
    JOGO_tppJogador jogador = (JOGO_tppJogador) malloc (sizeof(JOGO_tpJogador));
    if (jogador == NULL)
    {
        return NULL;
    }

    jogador->nome = nome;
    jogador->cor = cor;
    jogador->jogadas = 0;
    jogador->numPecas = NUM_PECAS_INI;
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

static WORD JOGO_ObterCorPeca(PECA_tppPeca peca)
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
*  $FC Função: JOGO  -Obter Direcao
*
***********************************************************************/

static JOGO_tpDirecao JOGO_ObterDirecao (TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    JOGO_tpDirecao direcao = {0, 0};

    direcao.x = (origem.coluna - destino.coluna) / abs(origem.coluna - destino.coluna); /* +1 ou -1 */
    direcao.y = (origem.linha - destino.linha) / abs(origem.linha - destino.linha);

    return direcao;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Valida Diagonal
*
***********************************************************************/

static int JOGO_ValidaDiagonal(TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    if (abs(destino.linha - origem.linha) == abs(destino.linha - origem.linha))
    {
        return TRUE;
    }
    return FALSE;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Distancia Diagonal
*
***********************************************************************/

static int JOGO_ObterDistanciaDiagonal(TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    return abs(origem.linha - destino.linha);
}

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Quantidade Pecas No Caminho
*
***********************************************************************/

static int JOGO_ObterQuantidadePecasNoCaminho(JOGO_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    int qtd = 0;
    JOGO_tpDirecao direcao = JOGO_ObterDirecao(origem, destino);
    while (origem.linha != destino.linha && origem.coluna != destino.coluna)
    {
        origem = JOGO_AvancarPosicao(origem, direcao)
                 if (TAB_ObterPeca(jogo->tab, origem) != NULL)
        {
            qtd++;
        }
    }
    return qtd;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Avancar Posicao
*
***********************************************************************/

static JOGO_tpPosicao JOGO_AvancarPosicao(JOGO_tpPosicao posicao, JOGO_tpDirecao direcao)
{
    posicao.linha += direcao.x;
    posicao.coluna += direcao.y;
    return posicao;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Movimento Valido
*
***********************************************************************/

static JOGO_tpCondRet JOGO_TentarJogada(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    PECA_tppPeca pecaMov = (PECA_tppPeca) TAB_ObterPeca(jogo->tab, origem);
    JOGO_tpDirecao direcao;
    TAB_tpPosicao tmp;
    int distancia = -1;
    int qtd = -1;

    /* conferindo se a peça existe ou se já existe alguma peça no destino */
    if (pecaMov == NULL || TAB_ObterPeca(jogo->tab, destino) != NULL))
    {
        return JOGO_CondRetJogadaInvalida;
    }

    /* Checa se o jogador esta movendo uma peca que nao lhe pertence */
    if (PECA_ObterCor(pecaMov) != jogadorDaVez->cor)
    {
        return JOGO_CondRetJogadaInvalida;
    }

    /* Checa se a peca nao esta se movendo na diagonal */
    if (JOGO_ValidaDiagonal(origem, destino))
    {
        return JOGO_CondRetJogadaInvalida;
    }

    /* destino fora do tabuleiro */
    if (!TAB_ChecarPos(jogo->tab, destino)
    {
        return JOGO_CondRetJogadaInvalida;
    }

    direcao = JOGO_ObterDirecao(origem, destino);

    if (PECA_ObterStatus(pecaMov) == PECA_StatusNormal)
    {
        /* normal */
        distancia = JOGO_ObterDistanciaDiagonal(origem, destino);
        if (distancia > 2)
        {
            return JOGO_CondRetJogadaInvalida;
        }
        if (PECA_ObterCor(pecaMov) == PECA_CorPreta)
        {
            if ((direcao == NE) || (direcao == NO))
            {
                return JOGO_CondRetJogadaInvalida;
            }
        }
        else if (PECA_ObterCor(pecaMov) == PECA_CorBranca)
        {
            if ((direcao == SE) || (direcao == SO))
            {
                return JOGO_CondRetJogadaInvalida;
            }
        }
        else
        {
            return JOGO_CondRetJogadaInvalida;
        }

        if (distancia == 1)
        {
            return JOGO_CondRetJogadaValida;
        }

        if (JOGO_PodeComer(jogo, origem, destino))
        {
            return JOGO_CondRetComeuPeca;
        }
        return JOGO_CondRetJogadaInvalida;
    }
    else
    {
        /*dama*/
        qtd = JOGO_ObterQuantidadePecasNoCaminho(jogo, origem, destino);
        if (qtd > 1)
        {
            return JOGO_CondRetJogadaInvalida;
        }
        else if (qtd == 0)
        {
            return JOGO_CondRetJogadaValida;
        }
        else
        {
            if (JOGO_PodeComer(jogo, origem, destino))
            {
                return JOGO_CondRetComeuPeca;
            }
        }
    }
    return JOGO_CondRetJogadaInvalida;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Pode comer
*
***********************************************************************/

static int JOGO_PodeComer(TAB_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    tpDirecao direcao;
    int achouPeca = 0;

    direcao = JOGO_ObterDirecao(origem, destino);

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

        if (TAB_ObterPeca(jogo->tab, origem) != NULL)
        {
            /* Já tinha achado uma peça na última iteração */
            if (achouPeca)
            {
                return FALSE;
            }
            if (jogo->jogadorDaVez->cor ==
                    PECA_ObterCor((PECA_tppPeca) TAB_ObterPeca(jogo->tab, origem)))
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
        if (!TAB_ChecarPos(jogo->tab, origem))
        {
            return FALSE;
        }
    }
}

static JOGO_tppJogador ObterJogadorNaoDaVez(JOGO_tppJogo jogo)
{
    if (jogo->jogador1 == jogo->jogadorDaVez)
    {
        return jogo->jogador2;
    }
    return jogo->jogador1;
}

/********** Fim do módulo de implementação: JOGO  gerenciador do jogo de damas **********/
