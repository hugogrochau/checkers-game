#if ! defined( JOGO_ )
#define JOGO_

/***************************************************************************
*  $MCI M�dulo de defini��o: JOGO  gerenciador do jogo de damas
*
*  Arquivo gerado:              jogo.c
*  Letras identificadoras:      JOGO
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor     Data        Observa��es
*     1       hpg/gmm   09/jun/2014 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa a l�gica e funcionamento geral do jogo de damas.
*
***************************************************************************/

#if defined( JOGO_OWN )
#define JOGO_EXT
#else
#define JOGO_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para o jogo */
typedef JOGO_tpJogo *JOGO_tppJogo;

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Inicializar jogo
*
*  $ED Descri��o da fun��o
*      Destroi por completo uma lista. Destroi os elementos e dps a cabe�a da lista
*
***********************************************************************/

JOGO_tppJogo JOGO_CriarJogo (char *NomeJogador1, char *NomeJogador2);

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Destruir jogo
*
*  $ED Descri��o da fun��o
*      Deleta o jogo, limpado o espa�o de mem�ria alocado para ele
*      o tabuleiro, e os jogadores
*
*  $EP Par�metros
*     jogo - refer�ncia para o tipo jogo a ser deletado.
*
***********************************************************************/

void JOGO_DestruirJogo (JOGO_tppJogo jogo);

/***********************************************************************
*
*  $FC Fun��o: JOGO  -Preencher tabuleiro
*
*  $ED Descri��o da fun��o
*      Carrega o tabuleiro de um arquivo e insere as pe�as no tabuleiro
*
*  $EP Par�metros
*     jogo - Ponteiro para o tipo jogo.
*     arqTabuleiro - string contedo o path (diretorio/nome) do arquivo.
*
*  $FV Valor retornado
*     CondRetArquivoInvalido - Caso o arquivo n�o seja aberto ou n�o exista.
*     CondRetJogoNaoInicializado - O jogo ainda nao foi inicializado
*     CondRetArquivoCorrompido - Dados invalidos
*     CondRetOk - Tabuleiro foi preenchido com sucesso
*
***********************************************************************/

JOGO_tpCondRet JOGO_PreencherTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro );


JOGO_tpCondRet JOGO_SalvarTabuleiro (JOGO_tppJogo jogo, FILE *fp arqTabuleiro );

#undef JOGO_EXT

/********** Fim do m�dulo de defini��o: PECA  Pe�a de tabuleiro de damas **********/

#else
#endif
