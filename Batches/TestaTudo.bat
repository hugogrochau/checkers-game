rem testa todos os scripts

cls

if exist ..\Scripts\estatisticas.estat  del ..\Scripts\estatisticas.estat

..\Produto\TesteLista		/s..\Scripts\TesteLista		/l..\Produto\TesteLista		/a..\Scripts\estatisticas
if errorlevel 4 goto sai

..\Produto\TestePeca        /s..\Scripts\TestePeca		/l..\Produto\TestePeca		/a..\Scripts\estatisticas
if errorlevel 4 goto sai

..\Produto\TesteTabuleiro   /s..\Scripts\TesteTabuleiro /l..\Produto\TesteTabuleiro /a..\Scripts\estatisticas
if errorlevel 4 goto sai

..\Produto\TesteTabuleiroDeturpa   /s..\Scripts\TesteTabuleiroDeturpa /l..\Produto\TesteTabuleiroDeturpa /a..\Scripts\estatisticas
if errorlevel 4 goto sai

exbestat /e..\Scripts\estatisticas

:sai

