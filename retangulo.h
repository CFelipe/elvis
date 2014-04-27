#include "objeto.h"
#include "vertice.h"

#ifndef RETANGULO_H
#define RETANGULO_H

class Retangulo : public Objeto {
    public:
        Retangulo(Vertice A, Vertice B, Vertice C, Vertice D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha);
        Vertice A, B, C, D, centro, max, min;
        void desenha();

    private:
        void Bresenham(Ponto p1, Ponto p2);
};

#endif // RETANGULO_H
