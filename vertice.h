#include "ponto.h"

#ifndef VERTICE_H
#define VERTICE_H

class Vertice {
    public:
        Vertice(Ponto p);
        Vertice(GLint x, GLint y);
        Ponto p;
        bool selecionado;
};

#endif // VERTICE_H
