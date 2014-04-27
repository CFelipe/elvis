#include "vertice.h"

Vertice::Vertice(Ponto p) {
    this->p = p;
    selecionado = false;
}

Vertice::Vertice(GLint x, GLint y) {
    this->p.x = x;
    this->p.y = y;
    selecionado = false;
}
