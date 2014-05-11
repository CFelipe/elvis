#include "polilinha.h"

#define CONTROL 5

Polilinha::Polilinha(GLfloat colorLine[4], GLint espessuraLinha, Ponto inicial) : Objeto(colorLine, colorLine, espessuraLinha, true, false, Objeto::POLILINHA) {
    vertices.append(new Vertice(inicial));
    vertices.append(new Vertice(inicial));
    this->selecionado = true;
}

void Polilinha::translada(GLint mouseX, GLint mouseY) {
    int i;
    GLint dx = mouseX - xclick;
    GLint dy = mouseY - yclick;
    for(i = 0; i < vertices.size(); i++) {
        vertices.at(i)->p.x += dx;
        vertices.at(i)->p.y += dy;
    }

    xclick = mouseX;
    yclick = mouseY;
}

Ponto Polilinha::boundsMax() {
    int i;
    GLint maxX, maxY;
    maxX = vertices.first()->p.x;
    maxY = vertices.first()->p.y;
    for(i = 1; i < vertices.size(); i++) {
        if(vertices.at(i)->p.x > maxX) {
            maxX = vertices.at(i)->p.x;
        }

        if(vertices.at(i)->p.y > maxY) {
            maxY = vertices.at(i)->p.y;
        }
    }

    return Ponto(maxX, maxY);
}

Ponto Polilinha::boundsMin() {
    int i;
    GLint minX, minY;
    minX = vertices.first()->p.x;
    minY = vertices.first()->p.y;
    for(i = 1; i < vertices.size(); i++) {
        if(vertices.at(i)->p.x < minX) {
            minX = vertices.at(i)->p.x;
        }

        if(vertices.at(i)->p.y < minY) {
            minY = vertices.at(i)->p.y;
        }
    }

    return Ponto(minX, minY);
}

void Polilinha::desseleciona(){
    int i;
    for(i = 0; i < vertices.size(); i++) {
        vertices.at(i)->selecionado = false;
    }
}

void Polilinha::desenhaLinha() {
    int i;
    for(i = 0; i < vertices.size(); i++) {
        if(i + 1 < vertices.size()) {
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

    int i;
    for(i = 0; i < vertices.size(); i++) {
        /* linhas
        glPointSize(1);
        glColor3f( 0,0, 0 );
        glBegin(GL_LINE_LOOP);
            glVertex2i(vertices.at(i)->p.x + 5 , vertices.at(i)->p.y + 5);
            glVertex2i(vertices.at(i)->p.x - 5 , vertices.at(i)->p.y + 5);
            glVertex2i(vertices.at(i)->p.x - 5 , vertices.at(i)->p.y - 5);
            glVertex2i(vertices.at(i)->p.x + 5 , vertices.at(i)->p.y - 5);
        glEnd();
        */

        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 0,0.5 , 0 );
            glVertex2i(vertices.at(i)->p.x, vertices.at(i)->p.y);
        glEnd();

        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 1 , 1 , 0 );
            glVertex2i(boundsMax().x, boundsMax().y);
        glEnd();

        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 1 , 0 , 1 );
            glVertex2i(boundsMin().x, boundsMin().y);
        glEnd();
    }
}

void Polilinha::deslocarVertice(Vertice* v, GLint mouseX, GLint mouseY) {
    v->p.x = mouseX;
    v->p.y = mouseY;
}

void Polilinha::inserirVertice(GLint mouseX, GLint mouseY) {
    vertices.append(new Vertice(mouseX, mouseY));
}

/*
void Polilinha::remove(Linha *l) {
}
*/
