#include "polilinha.h"

#define CONTROL 5

Polilinha::Polilinha(GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorLine, colorLine, espessuraLinha, true, false, Objeto::POLILINHA) {
    init = NULL;
    fim = NULL;
    this->selecionado = true;
}

void Polilinha::translada(GLint xmouse, GLint ymouse) {
    /*
    Polilinha *pol = dynamic_cast <Polilinha *>(aux);
    // centro do canvas = (gfWrldSizeX/2, gfWrldSizeY/2)
    GLint dy = abs((gfWrldSizeY-event->y()-gfWrldSizeY/2)) - abs((clickCanvas.y-gfWrldSizeY/2));
    GLint dx = abs((event->x() - gfWrldSizeX/2))-abs((clickCanvas.x- gfWrldSizeX/2));
    Linha *linha = pol->getInit();
    GLdouble fatorx = 1.01, fatory=1.01, auxx, auxy;
    while (linha!=NULL){
        if (dy>0){ //Se disnanciando de Y;
            auxy = fatory;
            cout<<"Saindo Y"<<endl;
        } else if (dy<0){ //Se aproximando de Y;
            auxy = 0.99;
            cout<<"Entrando Y"<<endl;
        } else{
            auxy=1;
        }

        while (linha!=NULL){
            linha->setV0(Ponto(linha->getV0().p.x+dx, linha->getV0().p.y+dy));
            linha->setV1(Ponto(linha->getV1().p.x+dx, linha->getV1().p.y+dy));
            linha = linha->getNext();
        }
        pol->atualizaMINMAX();
        pol->setXClick(event->x());
        pol->setYClick(gfWrldSizeY-event->y());
    }
    */
}

Ponto Polilinha::boundsMax() {
    return max;
}

Ponto Polilinha::boundsMin() {
    return min;
}

void Polilinha::desseleciona(){
    Linha *linha = init;
    sel1 = NULL;
    sel2 = NULL;
    selecionado = false;
    while (linha!=NULL){
        linha->getVV0()->selecionado = (false);
        linha->getVV1()->selecionado = (false);
        linha = linha->getNext();
    }
}

void Polilinha::desenhaLinha() {
    Linha *aux = init;

    while (aux!=NULL){
        Objeto::Bresenham(aux->getV0().p, aux->getV1().p);
        aux = aux->getNext();
    }
}

void Polilinha::desenhaFill() {
    // Não tem fill aqui
}

void Polilinha::desenhaControles() {
    Linha *aux = init;

    while (aux!=NULL){
        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 0,0.5 , 0 );
            glVertex2i(aux->getV0().p.x, aux->getV0().p.y);
        glEnd();

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
    aux = aux->getNext();
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

Ponto Polilinha::getCentro(){
    return centro;
}

void Polilinha::setCentro(Ponto centro){
    this->centro = centro;
}

Ponto Polilinha::getMax() {
    return max;
}

Ponto Polilinha::getMin() {
    return min;
}

void Polilinha::insert(Ponto p0, Ponto p1, Linha *depoisDe){
    Linha *l = new Linha(p0, p1);
    if (init==NULL && fim==NULL){
        init = l;
        fim = l;
        l->setNext(NULL);
        l->setPrevious(NULL);
        if (p1.x>p0.x){
            max.x = p1.x;
            min.x = p0.x;
        } else {
            max.x = p0.x;
            min.x = p1.x;
        }
        if (p1.y>p0.y){
            max.y = p1.y;
            min.y = p0.y;
        } else {
            max.y = p0.y;
            min.y = p1.y;
        }
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
        atualizaMINMAX();
    }
}

void Polilinha::atualizaMINMAX(){
    Linha *linha = init;
    //max = init->getV0().p;
    //min = init->getV0().p;
    while (linha!=NULL){
        if (linha->getV0().p.x>max.x)  max.x = linha->getV0().p.x;
        else if (linha->getV0().p.x<min.x) min.x = linha->getV0().p.x;

        if (linha->getV1().p.x>max.x)  max.x = linha->getV1().p.x;
        else if (linha->getV1().p.x<min.x) min.x = linha->getV1().p.x;


        if (linha->getV0().p.y>max.y)  max.y = linha->getV0().p.y;
        else if (linha->getV0().p.y<min.y) min.y = linha->getV0().p.y;

        if (linha->getV1().p.y>max.y)  max.y = linha->getV1().p.y;
        else if (linha->getV1().p.y<min.y) min.y = linha->getV1().p.y;
        linha = linha->getNext();
    }

    GLint xc = this->getMin().x+ abs((this->getMin().x-this->getMax().x)/2);
    GLint yc = this->getMin().y+ abs((this->getMin().y-this->getMax().y)/2);
    centro.x = xc;
    centro.y = yc;

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
    atualizaMINMAX();
}
void Polilinha::setInit(Ponto p0, Ponto p1){
    init->setV0(p0);
    init->setV1(p1);
    atualizaMINMAX();
}
Linha* Polilinha::getFim(){
    return fim;
}
Linha* Polilinha::getInit(){
    return init;
}

Ponto *Polilinha::getPMax(){
    return &max;
}
Ponto *Polilinha::getPMin(){
    return &min;
}
void Polilinha::setMax(Ponto max){
    this->max = max;
}

void Polilinha::setMin(Ponto min){
    this->min = min;
}

void Polilinha::escala(Ponto mouse) {
}

