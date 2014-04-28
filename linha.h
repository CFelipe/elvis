#ifndef LINHA_H
#define LINHA_H

#include "vertice.h"

class Linha{
    private:
        Vertice p0, p1;
        Linha *next, *previous;
    public:
        Linha(Vertice p0, Vertice p1);
        Linha(Ponto p0, Ponto p1);

        Vertice getP0();
        Vertice getP1();
        Vertice *getPP0();
        Vertice *getPP1();
        void setP1(Vertice p1);
        void setP0(Vertice p0);
        Linha * getNext();
        Linha *getPrevious();
        void setNext(Linha *n);
        void setPrevious(Linha *p);
};

#endif // LINHA_H
