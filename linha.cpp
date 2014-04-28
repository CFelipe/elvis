#include "linha.h"

Linha::Linha(Vertice p0, Vertice p1){
    this->p0 = p0;
    this->p1 = p1;
    next = NULL;
    previous = NULL;
}

Linha::Linha(Ponto p0, Ponto p1){
    this->p0 = Vertice(p0.x, p0.y);
    this->p1 = Vertice(p1.x, p1.y);
    next = NULL;
    previous = NULL;
}

Vertice Linha::getP0(){
    return p0;
}

Vertice Linha::getP1(){
    return p1;
}

Vertice* Linha::getPP0(){
    return &p0;
}

Vertice* Linha::getPP1(){
    return &p1;
}

void Linha::setP1(Vertice p1){
    p1.selecionado = (this->p1.selecionado);
    this->p1 = p1;
}

void Linha::setP0(Vertice p0){
    p0.selecionado = (this->p0.selecionado);
    this->p0 = p0;
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
