#ifndef POLILINHA_H
#define POLILINHA_H

#include "objeto.h"
#include "vertice.h"
#include "ponto.h"

class Polilinha: public Objeto {
    public:
        Polilinha(Ponto inicial);
        Polilinha(Polilinha* p);

        void desenhaLinha();
        void desenhaFill();
        void desenhaControles();

        void translada(GLint mouseX, GLint mouseY);

        void desseleciona();

        Ponto boundsMax();
        Ponto boundsMin();

        void deslocarVertice(Vertice* v, GLint mouseX, GLint mouseY);
        void deslocarVerticeSelecionado(GLint mouseX, GLint mouseY);
        void inserirVertice(GLint mouseX, GLint mouseY);
        void inserirVertice(int pre, GLint mouseX, GLint mouseY);
        void removerVertice(int i);

        QList<Vertice*> vertices;
};

#endif // POLILINHA_H
