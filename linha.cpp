#include "linha.h"

Linha::Linha(Vertice v0, Vertice v1){
    this->v0 = v0;
    this->v1 = v1;
    next = NULL;
    previous = NULL;
}

Linha::Linha(Ponto p0, Ponto p1){
    this->v0 = Vertice(p0.x, p0.y);
    this->v1 = Vertice(p1.x, p1.y);
    next = NULL;
    previous = NULL;
}

Vertice Linha::getV0(){
    return v0;
}

Vertice Linha::getV1(){
    return v1;
}

Vertice* Linha::getVV0(){
    return &v0;
}

Vertice* Linha::getVV1(){
    return &v1;
}

void Linha::setV1(Vertice v1){
    v1.selecionado = (this->v1.selecionado);
    this->v1 = v1;
}

void Linha::setV0(Vertice v0){
    v0.selecionado = (this->v0.selecionado);
    this->v0 = v0;
}

Linha* Linha::getNext() {
    return next;
}

Linha* Linha::getPrevious(){
    return previous;
}

void Linha::setNext(Linha *n){
    next = n;
}

void Linha::setPrevious(Linha *p){
    previous = p;
}
