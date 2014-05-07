#ifndef LINHA_H
#define LINHA_H

#include "vertice.h"

class Linha{
    private:
        Vertice v0, v1;
        Linha *next, *previous;
    public:
        Linha(Vertice v0, Vertice v1);
        Linha(Ponto v0, Ponto v1);

        Vertice getV0();
        Vertice getV1();
        Vertice *getVV0();
        Vertice *getVV1();
        void setV1(Vertice v1);
        void setV0(Vertice v0);
        Linha * getNext();
        Linha *getPrevious();
        void setNext(Linha *n);
        void setPrevious(Linha *p);
};

#endif // LINHA_H
