#include "retangulo.h"
#include "vertice.h"
#include "objeto.h"

Retangulo::Retangulo(Vertice A, Vertice B, Vertice C, Vertice D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, Objeto::RETANGULO) {
    this->A = Vertice(A.p.x, A.p.y);
    this->B = Vertice(B.p.x, B.p.y);
    this->C = Vertice(C.p.x, C.p.y);
    this->D = Vertice(D.p.x, D.p.y);
    Vertice m(A.p.x, A.p.y);
    min = m.p;
    max = m.p;
    Vertice c((max.x-min.x)/2 + min.x, (max.y- min.y)/2 + min.y);
    this->centro = c;
    this->tipo = Objeto::RETANGULO;
}

void Retangulo::desenha() {
    Objeto::Bresenham(A.p, B.p);
    Objeto::Bresenham(B.p, D.p);
    Objeto::Bresenham(D.p, C.p);
    Objeto::Bresenham(C.p, A.p);
}

void Retangulo::escala(GLdouble fatorx, GLdouble fatory){
   Vertice *v[4] = {&A,&B, &C, &D};
    for (int i=0; i<4; i++){
        if (v[i]->p.x==max.x){
            v[i]->p.x = (v[i]->p.x+fatorx);
        } else if (v[i]->p.x==min.x){
            v[i]->p.x = (v[i]->p.x-fatorx);
        }
        if (v[i]->p.y == max.y){
            v[i]->p.y = (v[i]->p.y + fatory);
        } else if (v[i]->p.y == min.y){
            v[i]->p.y = (v[i]->p.y - fatory);
        }
    }

    atualizaMINMAX();
}

void Retangulo::atualizaMINMAX(){
    Vertice v[4] = {A, B, C, D};
    GLint maxx=A.p.x, maxy=A.p.y, minx = A.p.x, miny = A.p.y;
    for (int i=0; i<4; i++){
      if (v[i].p.x>maxx) maxx = v[i].p.x;
      if (v[i].p.y>maxy) maxy = v[i].p.y;
      if (v[i].p.x<minx) minx = v[i].p.x;
      if (v[i].p.y<miny) miny = v[i].p.y;
    }
    min.x = (minx);
    min.y = (miny);
    max.x = (maxx);
    max.y = (maxy);
    Ponto c((max.x-min.x)/2 + min.x, (max.y- min.y)/2 + min.y);
    this->centro = c;
}
