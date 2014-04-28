#ifndef RETANGULO_H
#define RETANGULO_H

#include "objeto.h"
#include "vertice.h"
#include "ponto.h"

class Retangulo : public Objeto {
    public:
        Retangulo(Vertice A, Vertice B, Vertice C, Vertice D, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha);
        Vertice A, B, C, D, centro;
        Ponto max, min;
        void desenha();
        void escala(GLdouble fatorx, GLdouble fatory);
        void Bresenham(Vertice p1, Vertice p2);
        void atualizaMINMAX();
};

#endif // RETANGULO_H
