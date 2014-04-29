#ifndef VERTICE_H
#define VERTICE_H

#include "ponto.h"

class Vertice {
    public:
        Vertice() {};
        Vertice(Ponto p);
        Vertice(GLint x, GLint y);
        Ponto p;
        bool selecionado;
};

#endif // VERTICE_H
