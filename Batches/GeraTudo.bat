@ECHO  OFF
REM  Gera todos os scripts de make

cls

pushd  .

gmake  /b..\Composicao /cTesteLista
gmake  /b..\Composicao /cTestePeca
gmake  /b..\Composicao /cTesteTabuleiro
gmake  /b..\Composicao /cTesteTabuleiroDeturpa
gmake  /b..\Composicao /cJogoDeDamas
gmake  /b..\Composicao /cJogoDeDamasDebug

popd
