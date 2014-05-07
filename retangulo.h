#ifndef RETANGULO_H
#define RETANGULO_H

#include "objeto.h"
#include "vertice.h"
#include "ponto.h"

class Retangulo : public Objeto {
    public:
        Retangulo(Vertice A, Vertice B, Vertice C, Vertice D, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, bool linha, bool preenchido);
        Vertice A, B, C, D, centro;
        Ponto max, min;
        void desenhaLinha();
        void desenhaFill();
        void escala(GLdouble fatorx, GLdouble fatory);
        void Bresenham(Vertice p1, Vertice p2);
        void atualizaMINMAX();

        void setA(Vertice A){
           A.selecionado = (this->A.selecionado);
           this->A = A;
           atualizaMINMAX();
       }

       void setB(Vertice B){
           B.selecionado = (this->B.selecionado);
           this->B = B;
           atualizaMINMAX();

       }
       void setC(Vertice C){
           C.selecionado = (this->C.selecionado);
           this->C = C;
           atualizaMINMAX();

       }
       void setD(Vertice D){
           D.selecionado = (this->D.selecionado);
           this->D = D;
           atualizaMINMAX();
       }
};

#endif // RETANGULO_H
