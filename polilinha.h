#ifndef POLILINHA_H
#define POLILINHA_H

#include "objeto.h"
#include "linha.h"
#include "vertice.h"
#include "ponto.h"

class Polilinha: public Objeto { // é uma lista encadeada de linhas
    public:
        Polilinha(GLfloat colorLine[4], GLint espessuraLinha);
        void desenhaLinha();
        void desenhaFill();
        void remove(Linha *l);
        void insert(Ponto p0, Ponto p1, Linha *depoisDe);
        void setFim(Ponto p0, Ponto p1);
        void setInit(Ponto p0, Ponto p1);
        Linha *getFim();
        Linha *getInit();
        void deseleciona();
        void setLinhaSelecionada1(Linha *sel);
        void setLinhaSelecionada2(Linha *sel);
        Linha* getLinhaSelecionada1();
        Linha* getLinhaSelecionada2();

    private:
        Linha *init, *fim;
        Linha *sel1, *sel2;
        /*indica as linhas selecionadas no momento do click.
         * É últil para deslocamento de pontos;
         * Regra:
         *        --> Se o usuário clicar na linha, apenas sel1 aponta para a linha selecionada. sel2=NULL
         *        --> Se o usuário clicar num ponto de controle, ele seleciona as linhas unidas por este ponto
         * É importante definir estes ponteiros. Se não o fizéssemos, então
         * no evento de movimento do mouse, teríamos que varrer toda a lista de linhas
         * para verificar qual(is) estava(m) selecionada(s)
        */
        //! Algortimo de rasterização da linha: Bresenham
};

#endif // POLILINHA_H
