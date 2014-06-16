##################################################
###
### Diretivas de MAKE para o construto: JogoDeDamas
### Gerado a partir de: JogoDeDamas.comp
###
### ----- Arquivo gerado, NÃO EDITE!!! -----
###
##################################################

### Nomes globais

NOME            = JogoDeDamas


### Nomes de paths

Ph                   = ..\Fontes
Pobj                 = ..\Objetos
Perr                 = ..\Produto
PDEFAULT             = ..\Fontes
Pc                   = ..\Fontes

### Nomes de diretórios para geração

Fh                   = ..\Fontes
Fobj                 = ..\Objetos
Ferr                 = ..\Produto
FDEFAULT             = ..\Fontes
Fc                   = ..\Fontes

### Macros da plataforma


O       = /D_CRT_SECURE_NO_DEPRECATE
OD      = /Zi /Od /D_CRT_SECURE_NO_DEPRECATE
L       = /SUBSYSTEM:CONSOLE
LD      = /DEBUG /DEBUGTYPE:CV

!IFDEF PRD
O       = /Ox /D_CRT_SECURE_NO_DEPRECATE
!ENDIF

OPT     = /c /J /W4 /nologo
INCLUDE = $(INCLUDE);$(PDEFAULT)


### Regras de geração

all : limpa \
   $(Fobj)\Princip.obj   $(Fobj)\Jogo.obj   $(Fobj)\Tabuleiro.obj \
   $(Fobj)\Lista.obj   $(Fobj)\Peca.obj \
   Construto

### Limpar arquivos

limpa :
   if exist $(Ferr)\$(NOME).err  del $(Ferr)\$(NOME).err


### Dependências de módulos objeto a compilar

$(Fobj)\Princip.obj :  {$(Pc)}\Princip.c \
    {$(Ph)}jogo.h               {$(Ph)}peca.h               {$(Ph)}tabuleiro.h         
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err

$(Fobj)\Jogo.obj :  {$(Pc)}\Jogo.c \
    {$(Ph)}jogo.h               {$(Ph)}peca.h               {$(Ph)}tabuleiro.h         
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err

$(Fobj)\Tabuleiro.obj :  {$(Pc)}\Tabuleiro.c \
    ..\\tabelas\\TiposEspacosTabuleiro.def {$(Ph)}TST_Espc.h           {$(Ph)}cespdin.h            \
    {$(Ph)}conta.h              {$(Ph)}generico.h           {$(Ph)}lista.h              \
    {$(Ph)}tabuleiro.h         
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err

$(Fobj)\Lista.obj :  {$(Pc)}\Lista.c \
    {$(Ph)}TST_Espc.h           {$(Ph)}cespdin.h            {$(Ph)}generico.h           \
    {$(Ph)}lista.h             
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err

$(Fobj)\Peca.obj :  {$(Pc)}\Peca.c \
    {$(Ph)}peca.h              
   cl $(O) $(OPT) /Fo$(Fobj)\ $(Fc)\$(@B).c >> $(Ferr)\$(NOME).err


### Terminação

Construto : \
   $(Fobj)\Princip.obj   $(Fobj)\Jogo.obj   $(Fobj)\Tabuleiro.obj \
   $(Fobj)\Lista.obj   $(Fobj)\Peca.obj
    cd $(Fobj)
    LINK $(L) @$(NOME).build >> $(Ferr)\$(NOME).err

##################################################
###
### Fim de diretivas MAKE para o construto: JogoDeDamas
###
##################################################

