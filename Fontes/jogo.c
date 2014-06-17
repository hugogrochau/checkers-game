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

static JOGO_tpCondRet JOGO_TentarJogada(JOGO_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino);

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

static int JOGO_PodeComer(JOGO_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino);

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

static TAB_tpPosicao JOGO_AvancarPosicao (TAB_tpPosicao posicao, JOGO_tpDirecao direcao);

/* Código das funções exportadas pelo módulo */

/***********************************************************************
*
*  $FC Função: JOGO  -Criar jogo
*
***********************************************************************/

JOGO_tppJogo JOGO_CriarJogo (char *nomeJogador1, char *nomeJogador2)
{
    JOGO_tppJogo jogo;

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

    jogo->jogadorDaVez = jogo->jogador1;

    jogo->tab = TAB_CriarTabuleiro(COLUNAS, LINHAS, PECA_DestruirPeca);
    if (jogo->tab == NULL)
    {
        JOGO_DestruirJogo(jogo);
        return NULL;
    }

    JOGO_PreencherTabuleiro(jogo, fopen(PATH_TAB_INICIAL, "r"));
    return jogo;
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
        free(jogo);
        jogo = NULL;
    }
}

/***********************************************************************
*
*  $FC Função: JOGO  -Preencher tabuleiro
*
***********************************************************************/

JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp)
{
    short int i, j;
    PECA_tpStatus status;
    PECA_tpCor cor;
    PECA_tppPeca peca;
    TAB_tpPosicao pos;

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
                    if (jogo->jogador1->cor == cor)
                    {
                        jogo->jogador1->numDamas++;
                    }
                    else
                    {
                        jogo->jogador2->numDamas++;
                    }
                }
                if (TAB_IncluirPeca(jogo->tab, peca, pos) == TAB_CondRetPosicaoInvalida)
                {
                    return JOGO_CondRetArquivoCorrompido;
                }
                if (jogo->jogador1->cor == cor)
                {
                    jogo->jogador1->numPecas++;
                }
                else
                {
                    jogo->jogador2->numPecas++;
                }
            }
        }
    }

    fclose(fp);

    return JOGO_CondRetOK;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Imprimir tabuleiro
*
***********************************************************************/

JOGO_tpCondRet JOGO_ImprimirTabuleiro (JOGO_tppJogo jogo)
{
    short int i, j;
    PECA_tppPeca peca;
    TAB_tpPosicao pos;

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }
    printf(" | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n");
    for (i = 0; i < LINHAS; i++)
    {
        printf("----------------------------------\n");
        printf("%c", 'A' + i);
        printf("|");
        for (j = 0; j < COLUNAS; j++)
        {
            pos.linha = i;
            pos.coluna = j;
            peca = (PECA_tppPeca) TAB_ObterPeca(jogo->tab, pos);

            if (peca != NULL)
            {
                JOGO_ImprimirComCor(" O ", PECA_ObterCor(peca), PECA_ObterStatus(peca) == PECA_StatusDama);
            }
            else
            {
                printf("   ");
            }
            printf("|");
        }
        printf("\n");
    }
    return JOGO_CondRetOK;
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
    TAB_tpPosicao origem, destino, posComida;
    PECA_tppPeca pecaMovimentada, pecaComida;
    JOGO_tpCondRet condRetJogada;
    JOGO_tpDirecao direcaoInversa;

    if (jogo == NULL)
    {
        return JOGO_CondRetJogoNaoInicializado;
    }

    origem.coluna = colunaOrigem;
    origem.linha = linhaOrigem;
    destino.coluna = colunaDestino;
    destino.linha = linhaDestino;

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

    pecaMovimentada = (PECA_tppPeca) TAB_ObterPeca(jogo->tab, destino);
    if (condRetJogada == JOGO_CondRetComeuPeca)
    {
        printf("origem.coluna %d\norigem.linha%d\ndestino.coluna %d\ndestino.linha %d\n",
                origem.coluna,    origem.linha,   destino.coluna,    destino.linha);
        direcaoInversa = JOGO_ObterDirecao(destino, origem);
        printf("direcaoInversa.x %d\ndirecaoInversa.y %d\n",
                direcaoInversa.x,    direcaoInversa.y);
        /* move destino para a peça que foi comida */
        posComida = JOGO_AvancarPosicao(destino, direcaoInversa);
        if (PECA_ObterStatus(pecaMovimentada) == PECA_StatusDama)
        {
            while(TAB_ObterPeca(jogo->tab, posComida) == NULL)
            {
                posComida = JOGO_AvancarPosicao(posComida, direcaoInversa);
            }
        }
        printf("posComida.linha = %d\nposComida.coluna = %d\n", posComida.linha, posComida.coluna);
        pecaComida = (PECA_tppPeca) TAB_RemoverPeca(jogo->tab, posComida);

        JOGO_ObterJogadorNaoDaVez(jogo)->numPecas--;

        if (PECA_ObterStatus(pecaComida) == PECA_StatusDama)
        {
            JOGO_ObterJogadorNaoDaVez(jogo)->numDamas--;
        }

        PECA_DestruirPeca(pecaComida);
    }

    jogo->jogadorDaVez->jogadas++;

    if (jogo->jogador1->numPecas == 0)
    {
        return JOGO_CondRetJogador2Ganhou;
    }

    if (jogo->jogador2->numPecas == 0)
    {
        return JOGO_CondRetJogador1Ganhou;
    }

    if (jogo->jogadorDaVez->cor == PECA_CorBranca && destino.linha == 0)
    {
        PECA_VirarDama(pecaMovimentada);
        jogo->jogadorDaVez->numDamas++;
    }
    else if (jogo->jogadorDaVez->cor == PECA_CorPreta && destino.linha == LINHAS - 1)
    {
        PECA_VirarDama(pecaMovimentada);
        jogo->jogadorDaVez->numDamas++;
    }

    if (jogo->jogador1->numPecas <= 2 && jogo->jogador2->numPecas <= 2)
    {
        if ((jogo->jogador1->numDamas <= 2 && jogo->jogador2->numDamas <= 2) ||
                (jogo->jogador1->numDamas <= 2 && jogo->jogador2->numDamas == 1 && jogo->jogador2->numPecas == 2) ||
                (jogo->jogador1->numDamas == 1 && jogo->jogador1->numPecas == 2 && jogo->jogador2->numDamas <= 2))
        {
            return JOGO_CondRetEmpate;
        }
    }

    jogo->jogadorDaVez = JOGO_ObterJogadorNaoDaVez(jogo);

    return JOGO_CondRetJogadaValida;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Obter Jogador Não Da Vez
*
***********************************************************************/

JOGO_tppJogador JOGO_ObterJogadorNaoDaVez(JOGO_tppJogo jogo)
{
    if (jogo->jogador1 == jogo->jogadorDaVez)
    {
        return jogo->jogador2;
    }
    return jogo->jogador1;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Imprimir Com Cor
*
***********************************************************************/

void JOGO_ImprimirComCor(char* str, PECA_tpCor cor, int negrito)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    WORD formato = 0;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    if (cor == PECA_CorBranca)
    {
        formato = FOREGROUND_BLUE;
    }
    else if (cor == PECA_CorPreta)
    {
        formato = FOREGROUND_RED;
    }
    if (negrito)
    {
        formato |= FOREGROUND_INTENSITY;
    }

    SetConsoleTextAttribute(hConsole, formato);
    printf(str);
    SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* Código das funções encapsuladas no módulo */

/***********************************************************************
*
*  $FC Função: JOGO  -Criar Jogador
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
    jogador->numPecas = 0;
    jogador->numDamas = 0;
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
*  $FC Função: JOGO  -Obter Direcao
*
***********************************************************************/

static JOGO_tpDirecao JOGO_ObterDirecao (TAB_tpPosicao origem, TAB_tpPosicao destino)
{   
    JOGO_tpDirecao direcao = {0, 0};

    if (destino.coluna > origem.coluna)
    {
        direcao.x = 1;
    }
    else
    {
        direcao.x = -1;
    }

    if (destino.linha > origem.linha)
    {
        direcao.y = 1;
    }
    else
    {
        direcao.y = -1;
    }
    return direcao;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Valida Diagonal
*
***********************************************************************/

static int JOGO_ValidaDiagonal(TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    if (abs(destino.linha - origem.linha) == abs(destino.coluna - origem.coluna))
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
        origem = JOGO_AvancarPosicao(origem, direcao);
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

static TAB_tpPosicao JOGO_AvancarPosicao(TAB_tpPosicao posicao, JOGO_tpDirecao direcao)
{
    posicao.linha += direcao.y;
    posicao.coluna += direcao.x;
    return posicao;
}

/***********************************************************************
*
*  $FC Função: JOGO  -Movimento Valido
*
***********************************************************************/

static JOGO_tpCondRet JOGO_TentarJogada(JOGO_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    PECA_tppPeca pecaMov = (PECA_tppPeca) TAB_ObterPeca(jogo->tab, origem);
    JOGO_tpDirecao direcao;
    int distancia = -1;
    int qtd = -1;

    /* conferindo se a peça existe ou se já existe alguma peça no destino */
    if (pecaMov == NULL || TAB_ObterPeca(jogo->tab, destino) != NULL)
    {
        return JOGO_CondRetJogadaInvalida;
    }

    /* Checa se o jogador esta movendo uma peca que nao lhe pertence */
    if (PECA_ObterCor(pecaMov) != jogo->jogadorDaVez->cor)
    {
        return JOGO_CondRetJogadaInvalida;
    }

    /* Checa se a peca nao esta se movendo na diagonal */
    if (!JOGO_ValidaDiagonal(origem, destino))
    {
        return JOGO_CondRetJogadaInvalida;
    }

    /* destino fora do tabuleiro */
    if (!TAB_ChecarPos(jogo->tab, destino))
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
            if (direcao.y < 0)
            {
                return JOGO_CondRetJogadaInvalida;
            }
        }
        else if (PECA_ObterCor(pecaMov) == PECA_CorBranca)
        {
            if (direcao.y > 0)
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

static int JOGO_PodeComer(JOGO_tppJogo jogo, TAB_tpPosicao origem, TAB_tpPosicao destino)
{
    JOGO_tpDirecao direcao;
    int achouPeca = 0;

    direcao = JOGO_ObterDirecao(origem, destino);
    while (origem.linha != destino.linha && origem.coluna != destino.coluna)
    {
        origem = JOGO_AvancarPosicao(origem, direcao);

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
            achouPeca = TRUE;
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
    return FALSE;
}

/********** Fim do módulo de implementação: JOGO  gerenciador do jogo de damas **********/
