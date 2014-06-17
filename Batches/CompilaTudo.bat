ECHO compila todos os executaveis
REM Requer o ambiente (environment) inicializado para compilar com o VisualStudio Microsoft
cls

del ..\Produto\*.err
del ..\Objetos\*.obj

REM Compila teste do módulo lista
pushd .
nmake /F..\Composicao\TesteLista.make 
popd

REM Compila teste do módulo peca
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\TestePeca.make 
popd

REM Compila teste do módulo tabuleiro
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\TesteTabuleiro.make 
popd

REM Compila teste de auto-verificacao do módulo tabuleiro
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\TesteTabuleiroDeturpa.make 
popd

REM Compila jogo de damas
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\JogoDeDamas.make 
popd

REM Compila jogo de damas versão debug
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\JogoDeDamasDebug.make 
popd

del ..\Objetos\*.obj
popd

copy ..\Produto\*.err ..\Produto\tudo.err

notepad ..\Produto\tudo.err
