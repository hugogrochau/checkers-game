ECHO compila todos os programas exemplo
REM Requer o ambiente (environment) inicializado para compilar com o VisualStudio Microsoft
cls

del ..\Produto\*.err
del ..\Objetos\*.obj

REM compila para producao sem otimizacoes
pushd .
nmake /F..\Composicao\TesteLista.make 
popd

REM compila para producao sem otimizacoes
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\TestePeca.make 
popd

REM compila para producao sem otimizacoes
del ..\Objetos\*.obj
pushd .
nmake /F..\Composicao\TesteTabuleiro.make 
del ..\Objetos\*.obj

popd

copy ..\Produto\*.err ..\Produto\tudo.err

notepad ..\Produto\tudo.err
