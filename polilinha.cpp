#include "polilinha.h"

#define CONTROL 5

Polilinha::Polilinha(Ponto inicial) : Objeto(POLILINHA) {
    vertices.append(new Vertice(inicial));
    vertices.append(new Vertice(inicial));
    this->preenchido = false;
    this->selecionado = true;
}

Polilinha::Polilinha(Polilinha* p) : Objeto(POLILINHA) {
    int i;
    for(i = 0; i < p->vertices.size(); i++) {
        vertices.append(new Vertice(p->vertices.at(i)->p));
    }

    this->colorLine[0] = p->colorLine[0];
    this->colorLine[1] = p->colorLine[1];
    this->colorLine[2] = p->colorLine[2];
    this->espessuraLinha = p->espessuraLinha;
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
    this->selecionado = false;
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
        if(vertices.at(i)->selecionado) {
            glPointSize(8);
            glBegin(GL_POINTS);
                glColor3f( 0,0.5 , 0 );
                glVertex2i(vertices.at(i)->p.x, vertices.at(i)->p.y);
            glEnd();
        } else {
            glLineWidth(1);
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_LOOP);
                glVertex2i(vertices.at(i)->p.x + 5 , vertices.at(i)->p.y + 5);
                glVertex2i(vertices.at(i)->p.x - 5 , vertices.at(i)->p.y + 5);
                glVertex2i(vertices.at(i)->p.x - 5 , vertices.at(i)->p.y - 5);
                glVertex2i(vertices.at(i)->p.x + 5 , vertices.at(i)->p.y - 5);
            glEnd();
        }

        /* bounds
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
        */

    }
}

void Polilinha::deslocarVertice(Vertice* v, GLint mouseX, GLint mouseY) {
    v->p.x = mouseX;
    v->p.y = mouseY;
}

void Polilinha::deslocarVerticeSelecionado(GLint mouseX, GLint mouseY) {
    int i;
    for(i = 0; i < vertices.size(); i++) {
        if(vertices.at(i)->selecionado) {
            vertices.at(i)->p.x = mouseX;
            vertices.at(i)->p.y = mouseY;
            return;
        }
    }
}

void Polilinha::removerVertice(int i) {
    qDebug() << "remover vertice";
    if(vertices.size() > 2) {
        vertices.removeAt(i);
    }
    desseleciona();
}

void Polilinha::inserirVertice(GLint mouseX, GLint mouseY) {
    vertices.append(new Vertice(mouseX, mouseY));
}

void Polilinha::inserirVertice(int pre, GLint mouseX, GLint mouseY) {
    qDebug() << "entrou";
    desseleciona();
    Vertice *novo = new Vertice(mouseX, mouseY);
    novo->selecionado = true;
    selecionado = true;
    vertices.insert(pre + 1, novo);
}

/*
void Polilinha::remove(Linha *l) {
}
*/
