/***************************************************************************
*  $MCI Módulo de implementação: IO  Gerenciador da comunicação com o usuário
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
*      Lida com a entrada de dados do usuario e a resposta a ele
*
***************************************************************************/

#include <stdio.h>

#define TITULO(STR) "================" STR "================\n"

void IO_ImprimirHeader ()
{
    printf( TITULO("JOGO DE DAMAS")
           "DESENVOLVIDO POR {\n"
           "GUSTAVO MARQUES MARTINS (gmm)\n"
           "HUGO PEDROTTI GROCHAU (hpg)\n"
           "LUCAS REIS (lr)\n"
           TITULO("JOGO DE DAMAS"));
}

int IO_Menu()
{
    int menu;
    printf( TITULO("MENU")
           "1- Inicializar Jogo\n"
           "2- Creditos\n"
           "3- Sair\n"
           "\n"
           "Opcao : ");
    scanf("%d",&menu);
    while (menu < 1 || menu > 3)
    {
        printf("OPCAO INVALIDA!\n")
        printf( TITULO("MENU")
           "1- Inicializar Jogo\n"
           "2- Creditos\n"
           "3- Sair\n"
           "\n"
           "Opcao : ");
        scanf("%d",&menu);
    }
    return menu;
}
