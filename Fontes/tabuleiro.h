#if !defined( TAB_ )
#define TAB_

/***************************************************************************
*
*  $MCD M�dulo de defini��o: TAB  tabuleiro de damas ou qualquer outro jogo
*
*  Arquivo gerado:              tabuleiro.h
*  Letras identificadoras:      TAB
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm)
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     4       hpg   13/jun/2014     Tornou ChecarPos publica
*     3       gmm   19/abr/2014     Mais implementa��o
*     2       gmm   18/abr/2014     Mais implementa��o
*     1       gmm   16/abr/2014     in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa um tabuleiro.
*     O tabuleiro tem um tamanho e oferece diversas fun��es de movimenta��o
*
***************************************************************************/

#if defined( TAB_OWN )
#define TAB_EXT
#else
#define TAB_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para um tabuleiro */

typedef struct TAB_tagTabuleiro *TAB_tppTabuleiro;

/* Tipo tamanho de um tabuleiro */

typedef struct TAB_tagTamanho
{
    unsigned short int colunas;
    unsigned short int linhas;
} TAB_tpTamanho;

/* Tipo posicao de um tabuleiro */

typedef struct TAB_tagPosicao
{
    short int coluna;
    short int linha;
} TAB_tpPosicao;

/***********************************************************************
*
*  $TC Tipo de dados: TAB Condi��es de retorno
*
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es do tabuleiro
*
***********************************************************************/

typedef enum
{
    TAB_CondRetOk = 0,
    TAB_CondRetTabuleiroVazio = 1,
    TAB_CondRetPosicaoInvalida = 2,
    TAB_CondRetEstrutura = 3
} TAB_tpCondRet;

/***********************************************************************
*
*  $FC Fun��o: TAB &Criar Tabuleiro
*
*  $ED Descri��o da fun��o
*     Cria um tabuleiro com o tamanho passado.
*     A fun��o ir� se encarregar de checar se h� espa�o suficiente para a cria��o do tabuleiro ou n�o.
*     N�o � incluso nenhuma pe�a no tabuleiro.
*
*  $EP Par�metros
*     Coluna - tamanho horizontal do tabuleiro.
*     Linha - tamanho vertical do tabuleiro.
*     DestruirPeca - ponteiro para uma fun��o de destruir pe�a.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o tabuleiro.
*     Este ponteiro ser� utilizado pelas fun��es que manipulem o tabuleiro.
*
*     Se ocorreu algum erro, por exemplo falta de mem�ria ou dados errados,
*     a fun��o retornar� NULL.
*     N�o ser� dada mais informa��o quanto ao problema ocorrido.
*
***********************************************************************/

TAB_tppTabuleiro TAB_CriarTabuleiro (short int colunas, short int linhas, void (* DestruirPeca) (void *pDado));

/***********************************************************************
*
*  $FC Fun��o: TAB  &Destruir tabuleiro
*
*  $ED Descri��o da fun��o
*     Destr�i o tabuleiro fornecidd.
*     O par�metro ponteiro para o tabuleiro n�o � modificado.
*     Se ocorrer algum erro durante a destrui��o, o tabuleiro resultar�
*     estruturalmente incorreta.
*     OBS. n�o existe previs�o para poss�veis falhas de execu��o.
*  $EP Par�metros
*     tab - ponteiro para um tabuleiro.
*
***********************************************************************/

void TAB_DestruirTabuleiro (TAB_tppTabuleiro tab);

/***********************************************************************
*
*  $FC Fun��o: TAB  &Obter tamanho de um tabuleiro
*
*  $ED Descri��o da fun��o
*     Obtem o tamanho de um tabuleiro.
*
*  $EP Par�metros
*     tab - ponteiro para um tabuleiro.
*
*  $FV Valor retornado
*     tamanho - Retorna o tamanho do tabuleiro do tipo TAB_tpTamanho
*
***********************************************************************/

TAB_tpTamanho TAB_ObterTamanho (TAB_tppTabuleiro tab) ;

/***********************************************************************
*
*  $FC Fun��o: TAB  &Incluir peca
*
*  $ED Descri��o da fun��o
*     Inclui qualquer tipo de peca em uma dada posicao do tabuleiro
*     Essa fun��o checa se a posicao passada � valida.
*
*  $EP Par�metros
*     tab - ponteiro para o tabuleiro
*     pPeca - ponteiro para a peca
*     pos   - posicao que se deseja incluir a peca
*
*  $FV Valor retornado
*     CondRetOk = Ocorreu tudo bem
*     CondRetPosicaoInvalida
*     CondRetTabuleiroVazio
*
***********************************************************************/

TAB_tpCondRet TAB_IncluirPeca (TAB_tppTabuleiro tab, void   *pPeca, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Fun��o: TAB  &Destruir peca
*
*  $ED Descri��o da fun��o
*     Destroi a peca de uma certa posicao do tabuleiro
*     O dado da posicao ter� o valor NULL
*
*  $EP Par�metros
*     tab - ponteiro para um tabuleiro
*     pos - posicao do tabuleiro
*
*  $FV Valor retornado
*     CondRetOk - Ocorreu tudo bem
*     CondRetTabuleiroVazio
*     CondRetPosicaoInvalida
*
***********************************************************************/

TAB_tpCondRet TAB_DestruirPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Fun��o: TAB  &Remover peca
*
*  $ED Descri��o da fun��o
*     Essa funcao n�o destroi a peca. Ela apenas retira a peca do tabuleiro na posicao passada
*     O dado da posicao ter� o valor NULL
*
*  $EP Par�metros
*     tab - ponteiro para um tabuleiro
*     pos - posicao do tabuleiro
*
*  $FV Valor retornado
*     Endereco da peca - Ocorreu tudo bem (O endereco pode ser NULL caso nao tenha nenhuma peca)
*     NULL - Tab vazio ou posicao invaloda
*
***********************************************************************/

void *TAB_RemoverPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Fun��o: TAB  &Obter peca
*
*  $ED Descri��o da fun��o
*     Obtem peca de uma posicao de um tabuleiro
*
*  $EP Par�metros
*     tab - ponteiro para um tabuleiro
*     pos - posicao do tabuleiro
*
*  $FV Valor retornado
*     void * - Retorna NULL se acontecer algum erro (pos invalido ou tab = NULL)
*              Retorna o endere�o da peca (Pode ser NULL tmb) se todos os parametros
*              forem validos
*
***********************************************************************/

void *TAB_ObterPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Fun��o: TAB  &Mover peca
*
*  $ED Descri��o da fun��o
*     Move uma peca de uma posicao para outra. No local de origem, o ponteiro para a peca � setado para NULL.
*     Essa funcao nao checa se o movimento est� de acordo com as regras, apenas se o movimento est� dentro do tabuleiro.
*
*  $EP Par�metros
*     tab - ponteiro para um tabuleiro
*     Original - posicao original da peca
*     Destino  - posicao destino
*
*  $FV Valor retornado
*     CondRetOk = Ocorreu tudo bem
*     CondRetPosicaoInvalida
*     CondRetTabuleiroVazio
*
***********************************************************************/

TAB_tpCondRet TAB_MoverPeca (TAB_tppTabuleiro tab, TAB_tpPosicao original, TAB_tpPosicao destino);

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

int TAB_ChecarPos (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

#ifdef _DEBUG

TAB_tpCondRet TAB_VerificarTabuleiro( TAB_tppTabuleiro tab );

#endif

#undef TAB_EXT

/********** Fim do m�dulo de defini��o: TAB  tabuleiro de damas ou qualquer outro jogo **********/

#else
#endif
