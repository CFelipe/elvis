#include "retangulo.h"
#include "vertice.h"
#include "objeto.h"

Retangulo::Retangulo(Vertice A, Vertice B, Vertice C, Vertice D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, bool linha = true, bool preenchido = true) : Objeto(colorfill, colorLine, espessuraLinha, linha, preenchido, Objeto::RETANGULO) {
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
    this->isVisible = true;
    this->isSeletor = false;
    this->selecionado = true;
}

void Retangulo::translada(GLint xmouse, GLint ymouse) {
    GLint dx = xmouse - xclick;
    GLint dy = ymouse - yclick;
    setA(Ponto(A.p.x+dx, A.p.y+dy));
    setB(Ponto(B.p.x+dx, B.p.y+dy));
    setC(Ponto(C.p.x+dx, C.p.y+dy));
    setD(Ponto(D.p.x+dx, D.p.y+dy));
    xclick = xmouse;
    yclick = ymouse;
}

void Retangulo::desseleciona() {
    this->selecionado = false;
    this->A.selecionado = false;
    this->B.selecionado = false;
    this->C.selecionado = false;
    this->D.selecionado = false;
}

/* No momento isso não é verdade! Lembrar de mudar desenho */
Ponto Retangulo::boundsMax() {
    return D.p;
}

Ponto Retangulo::boundsMin() {
    return B.p;
}

void Retangulo::desenhaLinha() {
    Objeto::Bresenham(A.p, B.p);
    Objeto::Bresenham(B.p, D.p);
    Objeto::Bresenham(D.p, C.p);
    Objeto::Bresenham(C.p, A.p);
}

void Retangulo::desenhaFill() {
    GLint y, x_max, x_min, y_max, y_min;

    if(A.p.y < B.p.y) {
        y_min = A.p.y;
        y_max = B.p.y;
    } else {
        y_min = B.p.y;
        y_max = A.p.y;
    }

    if(A.p.x < D.p.x) {
        x_min = A.p.x;
        x_max = D.p.x;
    } else {
        x_min = D.p.x;
        x_max = A.p.x;
    }

    for(y = y_min; y <= y_max; y++) {
        Objeto::linhaFill(Ponto(x_min, y), Ponto(x_max, y));
    }
}

void Retangulo::desenhaControles() {
    glPointSize(8);
    glBegin(GL_POINTS);
        glColor3f( 0,0.5 , 0 );
        glVertex2i(A.p.x, A.p.y);
        glVertex2i(B.p.x, B.p.y);
        glVertex2i(C.p.x, C.p.y);
        glVertex2i(D.p.x, D.p.y);
    glEnd();

    /*
    glPointSize(1);
    glColor3f( 0,0, 0 );
    glBegin(GL_LINE_LOOP);
        glVertex2i(q->A.p.x+CONTROL, q->A.p.y+CONTROL);
        glVertex2i(q->A.p.x-CONTROL, q->A.p.y+CONTROL);
        glVertex2i(q->A.p.x-CONTROL, q->A.p.y-CONTROL);
        glVertex2i(q->A.p.x+CONTROL, q->A.p.y-CONTROL);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2i(q->B.p.x+CONTROL, q->B.p.y+CONTROL);
        glVertex2i(q->B.p.x-CONTROL, q->B.p.y+CONTROL);
        glVertex2i(q->B.p.x-CONTROL, q->B.p.y-CONTROL);
        glVertex2i(q->B.p.x+CONTROL, q->B.p.y-CONTROL);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2i(q->C.p.x+CONTROL, q->C.p.y+CONTROL);
        glVertex2i(q->C.p.x-CONTROL, q->C.p.y+CONTROL);
        glVertex2i(q->C.p.x-CONTROL, q->C.p.y-CONTROL);
        glVertex2i(q->C.p.x+CONTROL, q->C.p.y-CONTROL);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2i(q->D.p.x+CONTROL, q->D.p.y+CONTROL);
        glVertex2i(q->D.p.x-CONTROL, q->D.p.y+CONTROL);
        glVertex2i(q->D.p.x-CONTROL, q->D.p.y-CONTROL);
        glVertex2i(q->D.p.x+CONTROL, q->D.p.y-CONTROL);
    glEnd();
    */
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
