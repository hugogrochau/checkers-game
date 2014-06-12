#if ! defined( JOGO_ )
#define JOGO_

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

/* Tipo referência para o jogo */
typedef JOGO_tpJogo *JOGO_tppJogo;

/***********************************************************************
*
*  $FC Função: JOGO  -Inicializar jogo
*
*  $ED Descrição da função
*      Destroi por completo uma lista. Destroi os elementos e dps a cabeça da lista
*
***********************************************************************/

JOGO_tppJogo JOGO_CriarJogo (char *NomeJogador1, char *NomeJogador2);

/***********************************************************************
*
*  $FC Função: JOGO  -Destruir jogo
*
*  $ED Descrição da função
*      Deleta o jogo, limpado o espaço de memória alocado para ele
*      o tabuleiro, e os jogadores
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
*      Carrega o tabuleiro de um arquivo e insere as peças no tabuleiro
*
*  $EP Parâmetros
*     jogo - Ponteiro para o tipo jogo.
*     arqTabuleiro - string contedo o path (diretorio/nome) do arquivo.
*
*  $FV Valor retornado
*     CondRetArquivoInvalido - Caso o arquivo não seja aberto ou não exista.
*     CondRetJogoNaoInicializado - O jogo ainda nao foi inicializado
*     CondRetArquivoCorrompido - Dados invalidos
*     CondRetOk - Tabuleiro foi preenchido com sucesso
*
***********************************************************************/

JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro );


JOGO_tpCondRet JOGO_SalvarTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro );

#undef JOGO_EXT

/********** Fim do módulo de definição: PECA  Peça de tabuleiro de damas **********/

#else
#endif
