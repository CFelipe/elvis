#ifndef POLILINHA_H
#define POLILINHA_H

#include "objeto.h"
#include "vertice.h"
#include "ponto.h"

class Polilinha: public Objeto {
    public:
        Polilinha(GLfloat colorLine[4], GLint espessuraLinha);

        void desenhaLinha();
        void desenhaFill();
        void desenhaControles();

        void translada(GLint xmouse, GLint ymouse);

        void desseleciona();

        Ponto boundsMax();
        Ponto boundsMin();

        QList<Vertice*> vertices;
};

#endif // POLILINHA_H
