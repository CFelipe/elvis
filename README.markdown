Elvis
=====

Desenvolvido por Íslame Felipe da Costa Fernandes e Felipe Cortez de Sá

Compilando
----------
Inclui um Makefile gerado pelo Qt.

Criação de formas
-----------------
As formas podem ser criadas selecionando-se uma das ferramentas de criação, e
então clicando-se uma vez para determinar o ponto inicial e outra vez para o
ponto final. No caso da polilinha, pontos são adicionados a cada clique,
adicionando o último ponto com o clique do meio.

Seleção de objetos
------------------
A ferramenta de seleção (cursor preto) permite clicar em objetos,
selecionando-os e, posteriormente, transladando-os (clicando e arrastando) ou
deletando-os (apertando "Delete" no teclado). No caso da elipse, apenas o ponto
de controle no canto inferior direito movimenta o objeto.

Seleção de pontos
-----------------
A ferramenta de seleção de pontos (cursor branco) permite clicar em pontos de
controle, alterando a forma do objeto de maneira semelhante à criação. Também
neste modo, é possível deslocar pontos na polilinha clicando e arrastando-os,
adicionar novos pontos (Ctrl+Clique em cima da linha) ou remover pontos
(Ctrl+Clique em cima de um ponto de controle)

Pan
---
O viewport pode ser movimentado selecionando essa ferramenta e posteriormente
clicando e arrastando qualquer parte da área de desenho.

Cópia
-----
Objetos podem ser copiados clicando e arrastando para criar um novo objeto.

Rotação
-------
Objetos podem ser rotacionados por ângulos de 90 graus clicando em cima de um
ponto qualquer neles.

Escala
------
Objetos podem ser escalados clicando e arrastando em cima deles.

Características do objeto
-------------------------
É possível alterar as cores (tanto da linha quanto do preenchimento), a
espessura de linha e estilo de linha de cada objeto. Ctrl+Clique no botão das
cores abilita e desabilita a linha e o preenchimento. Um clique normal
seleciona uma cor. A espessura da linha pode ser escolhida, e o estilo de linha
é configurável escrevendo uma string com "-" para um pixel preenchido e "."
para um espaço em branco. Um estilo de linha com quatro pixels sim, quatro
pixels não é dado pela string "----....".

Exportação para SVG
-------------------
É possível exportar objetos para .SVG através do menu "Arquivo -> Exportar para
SVG" ou simplesmente Ctrl+E.

Grade
-----
É possível mostrar uma grade a que os objetos podem ser alinhados no momento da
criação clicando-se no botão de grade no canto inferior direito.
