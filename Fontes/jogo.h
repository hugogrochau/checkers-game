#if !defined( JOGO_ )
#define JOGO_

#include "tabuleiro.h"
#include "peca.h"

/***************************************************************************
*  $MCI Módulo de definição: JOGO  gerenciador do jogo de damas
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
*  $ED Descrição do módulo
*     Implementa a lógica e funcionamento geral do jogo de damas.
*
***************************************************************************/

#if defined( JOGO_OWN )
#define JOGO_EXT
#else
#define JOGO_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/***********************************************************************
*
*  $TC Tipo de dados: JOGO condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções do jogo
*
***********************************************************************/

typedef enum
{
    JOGO_CondRetOK = 1,
    /* Concluiu corretamente */

    JOGO_CondRetJogador1Ganhou = 2,
    /* O jogo terminou e o jogador 1 venceu */

    JOGO_CondRetJogador2Ganhou = 3,
    /* O jogo terminou e o jogador 2 venceu */

    JOGO_CondRetEmpate = 4,
    /* O jogo terminou num empate */

    JOGO_CondRetJogadaInvalida = 5,
    /* O jogador tentou fazer uma jogada inválida */

    JOGO_CondRetJogadaValida = 6,
    /* O jogador fez uma jogada válida */

    JOGO_CondRetComeuPeca = 7,
    /* O jogador fez uma jogada que comeu a peça do adversário  */

    JOGO_CondRetArquivoInvalido = 8,
    /* Ocorreu erro a ler o arquivo */

    JOGO_CondRetArquivoCorrompido = 9,
    /* Ocorreu erro a interpretar o arquivo */

    JOGO_CondRetJogoNaoInicializado = 10,
    /* O jogo não foi inicializado */

    JOGO_CondRetJogaOutraVez = 11,
    /* O jogador deve jogar mais uma vez */

} JOGO_tpCondRet;

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

    unsigned int numDamas;
    /* Número de damas */
} JOGO_tpJogador;

/* Tipo referência para um jogador */
typedef JOGO_tpJogador *JOGO_tppJogador;

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

/* Tipo referência para o jogo */
typedef struct JOGO_tagJogo * JOGO_tppJogo;

/***********************************************************************
*
*  $FC Função: JOGO  -Criar jogo
*
*  $ED Descrição da função
*     Cria uma nova instância de um jogo de damas, incializando todos
*     os dados necessários com o estado de início de jogo.
*
*  $EP Parâmetros
*     nomeJogador1 - o nome do primeiro jogador.
*     nomeJogador2 - o nome do segundo jogador.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o jogo.
*     Se ocorreu algum erro, a função retornará NULL
*
***********************************************************************/

JOGO_tppJogo JOGO_CriarJogo (char *NomeJogador1, char *NomeJogador2);

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir jogo
*
*  $ED Descrição da função
*     Deleta o jogo, limpando o espaço de memória alocado para ele
*     o tabuleiro, e os jogadores
*
*  $EP Parâmetros
*     jogo - referência para o tipo jogo a ser deletado.
*
***********************************************************************/

void JOGO_DestruirJogo (JOGO_tppJogo jogo);

/***********************************************************************
*
*  $FC Função: JOGO  -Preencher tabuleiro
*
*  $ED Descrição da função
*     Carrega o tabuleiro de um arquivo e insere as peças no tabuleiro
*
*  $EP Parâmetros
*     jogo - Ponteiro para o jogo a ser impresso.
*     arqTabuleiro - string contedo o path (diretorio/nome) do arquivo.
*
*  $FV Valor retornado
*     CondRetArquivoInvalido - Caso o arquivo não seja aberto ou não exista.
*     CondRetJogoNaoInicializado - O jogo ainda não foi inicializado
*     CondRetArquivoCorrompido - Dados invalidos
*     CondRetOk - Tabuleiro foi preenchido com sucesso
*
***********************************************************************/

JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp);

/***********************************************************************
*
*  $FC Função: JOGO  -Imprimir tabuleiro
*
*  $ED Descrição da função
*     Imprime uma representação visual do tabuleiro em um console do
*     Windows
*
*  $EP Parâmetros
*     jogo - Ponteiro para o jogo a ser impresso.
*
*  $FV Valor retornado
*     CondRetJogoNaoInicializado - O jogo ainda não foi inicializado
*     CondRetOk - Tabuleiro foi impresso com sucesso
*
***********************************************************************/

JOGO_tpCondRet JOGO_ImprimirTabuleiro (JOGO_tppJogo jogo);

/***********************************************************************
*
*  $FC Função: JOGO  -Executar Jogada
*
*  $ED Descrição da função
*     Executa a jogada dada uma direção
*
*  $EP Parâmetros
*     jogo - Ponteiro para o jogo sendo jogado.
*     linhaOrigem - linha da peça que vai ser movida.
*     colunaOrigem - coluna da peça que vai ser movida.
*     linhaDestino - linha para onde a peça vai ser movida.
*     colunaDestino - coluna para onde a peça vai ser movida.
*
*  $FV Valor retornado
*     CondRetPosicaoInvalida - Uma das posições é inválida
*     CondRetTabuleiroVazio - O tabuleiro não existe
*     CondRetJogador1Ganhou - O jogador 1 ganhou
*     CondRetJogador2Ganhou - O jogador 2 ganhou
*     CondRetEmpate - Houve um empate
*     CondRetOk - A jogada foi executada sem problemas
*
***********************************************************************/

JOGO_tpCondRet JOGO_ExecutarJogada (JOGO_tppJogo jogo, short int linhaOrigem, short int colunaOrigem, short int linhaDestino, short int colunaDestino);

JOGO_tppJogador JOGO_ObterJogadorNaoDaVez (JOGO_tppJogo jogo);

void JOGO_ImprimirComCor(char* str, PECA_tpCor cor, int negrito);

#undef JOGO_EXT

/********** Fim do módulo de definição: JOGO  gerenciador do jogo de damas **********/

#else
#endif
