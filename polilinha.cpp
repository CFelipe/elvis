#include "polilinha.h"

#define CONTROL 5

Polilinha::Polilinha(GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorLine, colorLine, espessuraLinha, true, false, Objeto::POLILINHA) {
    /*
    init = NULL;
    fim = NULL;
    this->selecionado = true;
    */
}

void Polilinha::translada(GLint xmouse, GLint ymouse) {
}

Ponto Polilinha::boundsMax() {
}

Ponto Polilinha::boundsMin() {
}

void Polilinha::desseleciona(){
}

void Polilinha::desenhaLinha() {
    int i;
    for(i = 0; i < vertices.size(); i++) {
        if(vertices.size() <= i + 1) {
            Objeto::Bresenham(vertices.at(i)->p, vertices.at(i + 1)->p);
        }
    }
}

void Polilinha::desenhaFill() {
    // Não tem fill aqui
}

void Polilinha::desenhaControles() {
    /*
    Linha *aux = init;

    while (aux!=NULL){
        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 0,0.5 , 0 );
            glVertex2i(aux->getV0().p.x, aux->getV0().p.y);
        glEnd();
    */

        /*
        glPointSize(1);
        glColor3f( 0,0, 0 );
        glBegin(GL_LINE_LOOP);
            glVertex2i(aux->getV1().p.x+CONTROL, aux->getV1().p.y+CONTROL);
            glVertex2i(aux->getV1().p.x-CONTROL, aux->getV1().p.y+CONTROL);
            glVertex2i(aux->getV1().p.x-CONTROL, aux->getV1().p.y-CONTROL);
            glVertex2i(aux->getV1().p.x+CONTROL, aux->getV1().p.y-CONTROL);
        glEnd();
        */
}


/*
void Polilinha::remove(Linha *l) {
}

void Polilinha::insert(Ponto p, Vertice pos){
}
*/
