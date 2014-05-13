#include "retangulo.h"
#include "vertice.h"
#include "objeto.h"
#include "window.h"

Retangulo::Retangulo(Vertice A, Vertice B, Vertice C, Vertice D) : Objeto(RETANGULO) {
    this->A = Vertice(A.p.x, A.p.y);
    this->B = Vertice(B.p.x, B.p.y);
    this->C = Vertice(C.p.x, C.p.y);
    this->D = Vertice(D.p.x, D.p.y);
    Vertice m(A.p.x, A.p.y);
    min = m.p;
    max = m.p;
    Vertice c((max.x-min.x)/2 + min.x, (max.y- min.y)/2 + min.y);
    this->centro = c;
    this->selecionado = true;
}

Retangulo::Retangulo(Retangulo* r) : Objeto(RETANGULO) {
    this->A = r->A;
    this->B = r->B;
    this->C = r->C;
    this->D = r->D;
    this->centro = r->centro;

    this->colorfill[0] = r->colorfill[0];
    this->colorfill[1] = r->colorfill[1];
    this->colorfill[2] = r->colorfill[2];
    this->colorLine[0] = r->colorLine[0];
    this->colorLine[1] = r->colorLine[1];
    this->colorLine[2] = r->colorLine[2];
    this->espessuraLinha = r->espessuraLinha;
}

void Retangulo::translada(GLint mouseX, GLint mouseY) {
    GLint dx = mouseX - xclick;
    GLint dy = mouseY - yclick;
    setA(Ponto(A.p.x+dx, A.p.y+dy));
    setB(Ponto(B.p.x+dx, B.p.y+dy));
    setC(Ponto(C.p.x+dx, C.p.y+dy));
    setD(Ponto(D.p.x+dx, D.p.y+dy));
    xclick = mouseX;
    yclick = mouseY;
}

void Retangulo::desseleciona() {
    this->selecionado = false;
    this->A.selecionado = false;
    this->B.selecionado = false;
    this->C.selecionado = false;
    this->D.selecionado = false;
}

Ponto Retangulo::boundsMax() {
    GLint x_max, y_max;

    if(A.p.y < B.p.y) {
        y_max = B.p.y;
    } else {
        y_max = A.p.y;
    }

    if(A.p.x < D.p.x) {
        x_max = D.p.x;
    } else {
        x_max = A.p.x;
    }

    return Ponto(x_max, y_max);
}

Ponto Retangulo::boundsMin() {
    GLint x_min, y_min;

    if(A.p.y < B.p.y) {
        y_min = A.p.y;
    } else {
        y_min = B.p.y;
    }

    if(A.p.x < D.p.x) {
        x_min = A.p.x;
    } else {
        x_min = D.p.x;
    }

    return Ponto(x_min, y_min);
}

void Retangulo::desenhaLinha() {
    Objeto::Bresenham(Objeto::pView(A.p), Objeto::pView(B.p));
    Objeto::Bresenham(Objeto::pView(B.p), Objeto::pView(D.p));
    Objeto::Bresenham(Objeto::pView(D.p), Objeto::pView(C.p));
    Objeto::Bresenham(Objeto::pView(C.p), Objeto::pView(A.p));
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
        Objeto::linhaFill(Objeto::pView(Ponto(x_min, y)), Objeto::pView(Ponto(x_max, y)));
    }
}

void Retangulo::desenhaControles() {
    glPointSize(8);
    glBegin(GL_POINTS);
        glColor3f( 0,0.5 , 0 );
        glVertex2i(Objeto::intXView(A.p.x), Objeto::intYView(A.p.y));
        glVertex2i(Objeto::intXView(B.p.x), Objeto::intYView(B.p.y));
        glVertex2i(Objeto::intXView(C.p.x), Objeto::intYView(C.p.y));
        glVertex2i(Objeto::intXView(D.p.x), Objeto::intYView(D.p.y));
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

int Retangulo::w() {
    return boundsMax().x - boundsMin().x;
}

int Retangulo::h() {
    return boundsMax().y - boundsMin().y;
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
