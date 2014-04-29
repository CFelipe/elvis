#include "polilinha.h"

#define CONTROL 5

Polilinha::Polilinha(GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, Objeto::POLILINHA) {
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

void Polilinha::desenha() {
    Linha *aux = init;

    if (selecionado){
        glPointSize(8);
        glBegin(GL_POINTS);
            glColor3f( 0,0.5 , 0 );
            glVertex2i(aux->getP0().p.x, aux->getP0().p.y);
        glEnd();
    } else {
        glPointSize(1);
        glColor3f( 0,0, 0 );
        glBegin(GL_LINE_LOOP);
            glVertex2i(aux->getP0().p.x+CONTROL, aux->getP0().p.y+CONTROL);
            glVertex2i(aux->getP0().p.x-CONTROL, aux->getP0().p.y+CONTROL);
            glVertex2i(aux->getP0().p.x-CONTROL, aux->getP0().p.y-CONTROL);
            glVertex2i(aux->getP0().p.x+CONTROL, aux->getP0().p.y-CONTROL);
        glEnd();
    }
    while (aux!=NULL){
        Bresenham(aux->getP0(), aux->getP1());
        if (selecionado){
            glPointSize(8);
            glBegin(GL_POINTS);
                glColor3f( 0,0.5 , 0 );
                glVertex2i(aux->getP1().p.x, aux->getP1().p.y);
            glEnd();
        } else {
            glPointSize(1);
            glColor3f( 0,0, 0 );
            glBegin(GL_LINE_LOOP);
                glVertex2i(aux->getP1().p.x+CONTROL, aux->getP1().p.y+CONTROL);
                glVertex2i(aux->getP1().p.x-CONTROL, aux->getP1().p.y+CONTROL);
                glVertex2i(aux->getP1().p.x-CONTROL, aux->getP1().p.y-CONTROL);
                glVertex2i(aux->getP1().p.x+CONTROL, aux->getP1().p.y-CONTROL);
            glEnd();
        }
        aux = aux->getNext();
    }
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
    fim->setP0(p0);
    fim->setP1(p1);
}
void Polilinha::setInit(Ponto p0, Ponto p1){
    init->setP0(p0);
    init->setP1(p1);
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
        linha->getPP0()->selecionado = (false);
        linha->getPP1()->selecionado = (false);
        linha = linha->getNext();
    }
}
void Polilinha::Bresenham(Vertice p1, Vertice p2) {
    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);

    // Especifica o diâmetro do Vertice
    glPointSize(getEspessuraLinha());

    GLint x, y;
    if (p2.p.x!=p1.p.x && p2.p.y!=p1.p.y){
        GLfloat m = (GLfloat) (p2.p.y-p1.p.y) / (p2.p.x-p1.p.x);
        if (m>0  && m<1){

            GLint dx = p2.p.x - p1.p.x;
            GLint dy = p2.p.y - p1.p.y;
            GLint pk = 2*dy - dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;
            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.x<p2.p.x){
                while (x<p2.p.x){

                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (x>p2.p.x){

                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m>-1 && m<0){
            printf("%f\n", m);
            GLint dx = p2.p.x - p1.p.x;
            GLint dy = - p2.p.y + p1.p.y;
            GLint pk = 2*dy + dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;

            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.x<p2.p.x){
                while (x<p2.p.x){
                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y--;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (x>p2.p.x){
                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y++;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m>1){
            GLint dx = p2.p.x - p1.p.x;
            GLint dy = p2.p.y - p1.p.y;
            GLint pk = 2*dx + dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y<p2.p.y){
                while (y<p2.p.y){
                    y++;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.p.y){
                    y--;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m<-1){
            GLint dx = p2.p.x - p1.p.x;
            GLint dy = -p2.p.y + p1.p.y;
            GLint pk = 2*dx - dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y>p2.p.y){
                while (y>p2.p.y){
                    y--;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.p.y){
                    y++;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }

        } else if (m==1){
            x = p1.p.x;
            y = (GLfloat) p1.p.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y<p2.p.y){
                while (y<p2.p.y){
                    x++;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.p.y){
                    x--;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m==-1){
            x = p1.p.x;
            y = (GLfloat) p1.p.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y>p2.p.y){
                while (y>p2.p.y){
                    x++;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.p.y){
                    x--;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        }
    } else if (p2.p.y==p1.p.y){ //se a linha for horizontal:

        x = p1.p.x;
        y = (GLfloat) p1.p.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.p.x<p2.p.x){
            while (x<p2.p.x){
                x++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (x>p2.p.x){
                x--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    } else { // se a linha for vertical:
        x = p1.p.x;
        y = (GLfloat) p1.p.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.p.y<p2.p.y){
            while (y<p2.p.y){
                y++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (y>p2.p.y){
                y--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    }
}

