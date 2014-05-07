#include "polilinha.h"

#define CONTROL 5

Polilinha::Polilinha(GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorLine, colorLine, espessuraLinha, true, false, Objeto::POLILINHA) {
    init = NULL;
    fim = NULL;
}

void Polilinha::remove(Linha *l) {
    if (fim==init){
        init=NULL;
        fim = NULL;
    } else {
        if (l==init){
            init = l->getNext();
        } else { //l->getPrevious()!=NULL
            l->getPrevious()->setNext(l->getNext());
        }
        if (l==fim){
            fim = l->getPrevious();
        } else { // l->getNext()!=NULL;
            l->getNext()->setPrevious(l->getPrevious());
        }
    }
    delete l;
}

void Polilinha::insert(Ponto p0, Ponto p1, Linha *depoisDe){
    Linha *l = new Linha(p0, p1);
    if (init==NULL && fim==NULL){
        init = l;
        fim = l;
        l->setNext(NULL);
        l->setPrevious(NULL);
    } else { // init!=NULL sse fim!=NULL
        if (depoisDe==NULL){ // inserimos uma nova linha inicial (init)
            l->setNext(init);
            init->setPrevious(l);
            l->setPrevious(NULL);
            init = l;
        } else {
            l->setNext(depoisDe->getNext());
            if (depoisDe==fim){
                fim = l;
            } else {
                depoisDe->getNext()->setPrevious(l);
            }
            l->setPrevious(depoisDe);
            depoisDe->setNext(l);
        }
    }
}

void Polilinha::desenhaLinha() {
    Linha *aux = init;

    if (selecionado){
        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 0,0.5 , 0 );
            glVertex2i(aux->getV0().p.x, aux->getV0().p.y);
        glEnd();
    } else {
        glPointSize(1);
        glColor3f( 0,0, 0 );
        glBegin(GL_LINE_LOOP);
            glVertex2i(aux->getV0().p.x+CONTROL, aux->getV0().p.y+CONTROL);
            glVertex2i(aux->getV0().p.x-CONTROL, aux->getV0().p.y+CONTROL);
            glVertex2i(aux->getV0().p.x-CONTROL, aux->getV0().p.y-CONTROL);
            glVertex2i(aux->getV0().p.x+CONTROL, aux->getV0().p.y-CONTROL);
        glEnd();
    }
    while (aux!=NULL){
        Objeto::Bresenham(aux->getV0().p, aux->getV1().p);
        if (selecionado){
            glPointSize(8);
            glBegin(GL_POINTS);
                glColor3f( 0,0.5 , 0 );
                glVertex2i(aux->getV1().p.x, aux->getV1().p.y);
            glEnd();
        } else {
            glPointSize(1);
            glColor3f( 0,0, 0 );
            glBegin(GL_LINE_LOOP);
                glVertex2i(aux->getV1().p.x+CONTROL, aux->getV1().p.y+CONTROL);
                glVertex2i(aux->getV1().p.x-CONTROL, aux->getV1().p.y+CONTROL);
                glVertex2i(aux->getV1().p.x-CONTROL, aux->getV1().p.y-CONTROL);
                glVertex2i(aux->getV1().p.x+CONTROL, aux->getV1().p.y-CONTROL);
            glEnd();
        }
        aux = aux->getNext();
    }
}

void Polilinha::desenhaFill() {
    // Não tem fill aqui
}

void Polilinha::setLinhaSelecionada1(Linha *sel){
    sel1 = sel;
}
void Polilinha::setLinhaSelecionada2(Linha *sel){
    sel2 = sel;
}
Linha* Polilinha::getLinhaSelecionada1() {
    return sel1;
}
Linha* Polilinha::getLinhaSelecionada2() {
    return sel2;
}
void Polilinha::setFim(Ponto p0, Ponto p1){
    fim->setV0(p0);
    fim->setV1(p1);
}
void Polilinha::setInit(Ponto p0, Ponto p1){
    init->setV0(p0);
    init->setV1(p1);
}
Linha* Polilinha::getFim(){
    return fim;
}
Linha* Polilinha::getInit(){
    return init;
}
void Polilinha::deseleciona(){
    Linha *linha = init;
    sel1 = NULL;
    sel2 = NULL;
    selecionado = (false);
    while (linha!=NULL){
        linha->getVV0()->selecionado = (false);
        linha->getVV1()->selecionado = (false);
        linha = linha->getNext();
    }
}
