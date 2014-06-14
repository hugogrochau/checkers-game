#if !defined( TAB_ )
#define TAB_

/***************************************************************************
*
*  $MCD Módulo de definição: TAB  tabuleiro de damas ou qualquer outro jogo
*
*  Arquivo gerado:              tabuleiro.h
*  Letras identificadoras:      TAB
*
*  Projeto: INF 1301 / 1628 Jogo de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm)
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     4       hpg   13/jun/2014     Tornou ChecarPos publica
*     3       gmm   19/abr/2014     Mais implementação
*     2       gmm   18/abr/2014     Mais implementação
*     1       gmm   16/abr/2014     início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa um tabuleiro.
*     O tabuleiro tem um tamanho e oferece diversas funções de movimentação
*
***************************************************************************/

#if defined( TAB_OWN )
#define TAB_EXT
#else
#define TAB_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um tabuleiro */

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
*  $TC Tipo de dados: TAB Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções do tabuleiro
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
*  $FC Função: TAB &Criar Tabuleiro
*
*  $ED Descrição da função
*     Cria um tabuleiro com o tamanho passado.
*     A função irá se encarregar de checar se há espaço suficiente para a criação do tabuleiro ou não.
*     Não é incluso nenhuma peça no tabuleiro.
*
*  $EP Parâmetros
*     Coluna - tamanho horizontal do tabuleiro.
*     Linha - tamanho vertical do tabuleiro.
*     DestruirPeca - ponteiro para uma função de destruir peça.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o tabuleiro.
*     Este ponteiro será utilizado pelas funções que manipulem o tabuleiro.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL.
*     Não será dada mais informação quanto ao problema ocorrido.
*
***********************************************************************/

TAB_tppTabuleiro TAB_CriarTabuleiro (short int colunas, short int linhas, void (* DestruirPeca) (void *pDado));

/***********************************************************************
*
*  $FC Função: TAB  &Destruir tabuleiro
*
*  $ED Descrição da função
*     Destrói o tabuleiro fornecidd.
*     O parâmetro ponteiro para o tabuleiro não é modificado.
*     Se ocorrer algum erro durante a destruição, o tabuleiro resultará
*     estruturalmente incorreta.
*     OBS. não existe previsão para possíveis falhas de execução.
*  $EP Parâmetros
*     tab - ponteiro para um tabuleiro.
*
***********************************************************************/

void TAB_DestruirTabuleiro (TAB_tppTabuleiro tab);

/***********************************************************************
*
*  $FC Função: TAB  &Obter tamanho de um tabuleiro
*
*  $ED Descrição da função
*     Obtem o tamanho de um tabuleiro.
*
*  $EP Parâmetros
*     tab - ponteiro para um tabuleiro.
*
*  $FV Valor retornado
*     tamanho - Retorna o tamanho do tabuleiro do tipo TAB_tpTamanho
*
***********************************************************************/

TAB_tpTamanho TAB_ObterTamanho (TAB_tppTabuleiro tab) ;

/***********************************************************************
*
*  $FC Função: TAB  &Incluir peca
*
*  $ED Descrição da função
*     Inclui qualquer tipo de peca em uma dada posicao do tabuleiro
*     Essa função checa se a posicao passada é valida.
*
*  $EP Parâmetros
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
*  $FC Função: TAB  &Destruir peca
*
*  $ED Descrição da função
*     Destroi a peca de uma certa posicao do tabuleiro
*     O dado da posicao terá o valor NULL
*
*  $EP Parâmetros
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
*  $FC Função: TAB  &Remover peca
*
*  $ED Descrição da função
*     Essa funcao não destroi a peca. Ela apenas retira a peca do tabuleiro na posicao passada
*     O dado da posicao terá o valor NULL
*
*  $EP Parâmetros
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
*  $FC Função: TAB  &Obter peca
*
*  $ED Descrição da função
*     Obtem peca de uma posicao de um tabuleiro
*
*  $EP Parâmetros
*     tab - ponteiro para um tabuleiro
*     pos - posicao do tabuleiro
*
*  $FV Valor retornado
*     void * - Retorna NULL se acontecer algum erro (pos invalido ou tab = NULL)
*              Retorna o endereço da peca (Pode ser NULL tmb) se todos os parametros
*              forem validos
*
***********************************************************************/

void *TAB_ObterPeca (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

/***********************************************************************
*
*  $FC Função: TAB  &Mover peca
*
*  $ED Descrição da função
*     Move uma peca de uma posicao para outra. No local de origem, o ponteiro para a peca é setado para NULL.
*     Essa funcao nao checa se o movimento está de acordo com as regras, apenas se o movimento está dentro do tabuleiro.
*
*  $EP Parâmetros
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
*  $FC Função: TAB  -Checar posição
*
*  $ED Descrição da função
*      Checa se a posição é invalida ou nao. Para ser invalida,
*      POS < 0 ou POS >= tabuleiro.tamanho
*      Se a posicao for valida retorna 1, se for invaliad retorna 0
*
***********************************************************************/

int TAB_ChecarPos (TAB_tppTabuleiro tab, TAB_tpPosicao pos);

#ifdef _DEBUG

TAB_tpCondRet TAB_VerificarTabuleiro( TAB_tppTabuleiro tab );

#endif

#undef TAB_EXT

/********** Fim do módulo de definição: TAB  tabuleiro de damas ou qualquer outro jogo **********/

#else
#endif
