#if ! defined( PECA_ )
#define IO_

/***************************************************************************
*  $MCD Módulo de definição: IO  Gerenciador da comunicação com o usuário
*
*  Arquivo gerado:              io.c
*  Letras identificadoras:      IO
*
*  Projeto: INF 1301 / 1628 IO de Damas
*  Gestor:  LES/DI/PUC-Rio
*  Autores: Gustavo Marques Martins (gmm), Hugo Pedrotti Grochau (hpg)
*
*  $HA Histórico de evolução:
*     Versão  Autor     Data        Observações
*     1       hpg/gmm   09/jun/2014 início desenvolvimento
*
*  $ED Descrição do módulo
*     Lida com a entrada de dados do usuario e a resposta a ele.
*
***************************************************************************/

#if defined( IO_OWN )
#define IO_EXT
#else
#define IO_EXT extern
#endif

/***********************************************************************
*
*  $FC Função: PECA &Criar Peça
*
*  $ED Descrição da função
*     Cria uma peça com a cor especificada.
*
*  $EP Parâmetros
*     cor - A cor desejada.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para a peça.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL.
*
***********************************************************************/

void IO_ImprimirHeader ();