#include "glwidget.h"
#include "objeto.h"
#include "elipse.h"
#include "circulo.h"
#include "retangulo.h"
#include "ponto.h"
#include "vertice.h"

#include <iostream>
#include <cmath>

#define CONTROL 5

using namespace std;

Operacao op;
Objeto::Forma forma;

GLint espessuraLinha;
bool preenchido; // true  -> a forma está preenchida com alguma cor
bool desenha; // true = desenha. false = seleciona
bool desenhandoPolilinha = false;
/*Esta variável diz se uma polilinha está sendo desenhada;
 *É importante para diferenciar quando uma nova polilinha é criada,
 *e quando uma nova linha é adicionada à polilinha que está sendo desenhada
 */
// ---------------

Lista *init=NULL, *fim=NULL;

void insere(Objeto *ob) { // insere sempre no final da lista
    Lista *novo = new Lista;
    novo->objeto = ob;
    novo->next = NULL;
    novo->previous = fim;
    if (init==NULL && fim==NULL){
        init=novo;
    } else {
        fim->next = novo;
    }
    fim = novo;
}

void remove(Lista *re){
    if (re==init && re==fim){
        init=NULL;
        fim=NULL;
    } else if (re==init){
        init = re->next;
        (re->next)->previous = NULL; // re->next != NULL (certeza)

    } else if (re==fim){
        fim = re->previous;
        re->previous->next = NULL; //re->previous != NULL (certeza)
    } else {
        (re->previous)->next = re->next;
        (re->next)->previous = re->previous;
    }
}
// ---------------


/*
 * Lista encadeada de objetos (circulos, quadrilateros, elipes, polilinhas, e etc).
 * Não há necessidade de polimorfismo ou modelagem orientada a objeto. Por isso usamos struct
 */

GLfloat gfWrldSizeX = 800.f;
GLfloat gfWrldSizeY = 600.f;
GLfloat gfWrldSizeZ = 400.f;

Ponto clickCanvas;
GLdouble distanciaAB = 0; // guarda a distancia entre dois pontos quaisquer A e B
GLdouble anguloDeRotacao = M_PI/3; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)

/* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
 * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
 * opBotaoDireito=fase <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um ponto de controle
*/
bool opBotaoDireito = false;
bool onMouseClick = false;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);

    op = TRANSLACAO;
    forma = Objeto::ELIPSE;
    //---------------------

    desenha = true;
    espessuraLinha = 2;
}

void GLWidget::initializeGL() {}

void GLWidget::rotacionaObjeto(Objeto *ob){
    /* 1 Desloca os pontos para a origem
     * 2 Rotaciona
     * 3 Devolve as cordenadas iniciais
     */
    if (ob->selecionado){
        switch(ob->getTipo()){
            case Objeto::RETANGULO :
                {
                Retangulo *q = dynamic_cast <Retangulo *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                Ponto A = q->A.p;
                Ponto B = q->B.p;
                Ponto C = q->C.p;
                Ponto D = q->D.p;
                A.x = (A.x - clickCanvas.x);
                A.y = (A.y - clickCanvas.y);
                B.x = (B.x - clickCanvas.x);
                B.y = (B.y - clickCanvas.y);
                C.x = (C.x - clickCanvas.x);
                C.y = (C.y - clickCanvas.y);
                D.x = (D.x - clickCanvas.x);
                D.y = (D.y - clickCanvas.y);
                GLint aux = A.x;
                A.x = (aux*cos(anguloDeRotacao) - A.y*sin(anguloDeRotacao) + clickCanvas.x);
                A.y = (aux*sin(anguloDeRotacao) + A.y*cos(anguloDeRotacao) + clickCanvas.y);
                aux = B.x;
                B.x = (aux*cos(anguloDeRotacao) - B.y*sin(anguloDeRotacao) + clickCanvas.x);
                B.y = (aux*sin(anguloDeRotacao) + B.y*cos(anguloDeRotacao) + clickCanvas.y);
                aux = C.x;
                C.x = (aux*cos(anguloDeRotacao) - C.y*sin(anguloDeRotacao) + clickCanvas.x);
                C.y = (aux*sin(anguloDeRotacao) + C.y*cos(anguloDeRotacao) + clickCanvas.y);
                aux = D.x;
                D.x = (aux*cos(anguloDeRotacao) - D.y*sin(anguloDeRotacao) + clickCanvas.x);
                D.y = (aux*sin(anguloDeRotacao) + D.y*cos(anguloDeRotacao) + clickCanvas.y);
                q->A = (A);
                q->B = (B);
                q->C = (C);
                q->D = (D);}
                break;
            case Objeto::CIRCULO:
                {
                Circulo *c = dynamic_cast <Circulo *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                GLint xc = c->getXc()-clickCanvas.x;
                GLint yc = c->getYc()-clickCanvas.y;
                c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.x);
                c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.y);
                }
                break;
        case Objeto::ELIPSE:
                {
                    Elipse *e = dynamic_cast <Elipse *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    GLint xc = e->getCentro().p.x-clickCanvas.x;
                    GLint yc = e->getCentro().p.y-clickCanvas.y;
                    GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);
                    Ponto cen(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.x, xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.y );
                    GLint aux = e->getRaioHorizontal();
                    e->setRaioHorizontal(e->getRaioVertical());
                    e->setRaioVertical(aux);
                    e->setCentro(cen);
                    e->setControl(Ponto(e->getCentro().p.x+ax*(e->getRaioHorizontal()),e->getCentro().p.y+ay*(e->getRaioVertical())));
                    cout<<"ROTACAO ELISPE "<<endl;
                }
                break;
        case Objeto::POLILINHA:
        {
            /*
                Polilinha *pol = dynamic_cast <Polilinha *>(ob);
                Linha *linha = pol->getInit();
                while (linha!=NULL){
                    Ponto P0 = linha->getP0();
                    Ponto P1 = linha->getP1();
                    P0.x = (P0.x-clickCanvas.x);
                    P0.y = (P0.y-clickCanvas.y);
                    P1.x = (P1.x-clickCanvas.x);
                    P1.y = (P1.y-clickCanvas.y);
                    GLint aux = P0.x;
                    P0.x = (aux*cos(anguloDeRotacao) - P0.y*sin(anguloDeRotacao) + clickCanvas.x);
                    P0.y = (aux*sin(anguloDeRotacao) + P0.y*cos(anguloDeRotacao) + clickCanvas.y);
                    aux = P1.x;
                    P1.x = (aux*cos(anguloDeRotacao) - P1.y*sin(anguloDeRotacao) + clickCanvas.x);
                    P1.y = (aux*sin(anguloDeRotacao) + P1.y*cos(anguloDeRotacao) + clickCanvas.y);
                    linha->setP0(P0);
                    linha->setP1(P1);
                    linha = linha->getNext();
                }
                */
                break;
        }
            default:
                cout<<"DEFAUT"<<endl;
                break;

        }
    }
}

unsigned int CompOutCode(Ponto p, Ponto max, Ponto min){
    unsigned int code = 0;
    if (p.y>max.y){
        code |= 0x1;
    } else if (p.y<min.y){
        code |= 0x2;
    }
    if (p.x>max.x){
        code |= 0x4;
    } else if (p.x<min.x){
        code |= 0x8;
    }
    return code;
}

/*
void deslocaPontoPolilinha(Polilinha *pol, int x, int y){
    Linha *sel1 = pol->getLinhaSelecionada1(); // nunca é NULL
    Linha *sel2 = pol->getLinhaSelecionada2();
    GLint dy = y - pol->getYClick();
    GLint dx = x - pol->getXClick();
    if (sel1->getP0().isSelect()){ // linha 1, portanto sel2=NULL
        sel1->setP0(Ponto(sel1->getP0().x+dx, sel1->getP0().y+dy));
    } else if (sel1->getP1().isSelect()){
        sel1->setP1(Ponto(sel1->getP1().x+dx, sel1->getP1().y+dy));
        if (sel2!=NULL){
            sel2->setP0(Ponto(sel2->getP0().x+dx, sel2->getP0().y+dy));
        }
    }
    pol->setXClick(x);
    pol->setYClick(y);
}
*/

bool cohenClipping(Ponto P0, Ponto P1, Ponto max, Ponto min){
    enum {TOP=0x1, BOTTOM=0x2, RIGHT=0x4, LEFT=0x8};
    unsigned int outcode0, outcode1, outcodeOut;
    bool aceito = false, feito = false;
    outcode0 = CompOutCode(P0, max, min);
    outcode1 = CompOutCode(P1, max, min);
    do{
        if (!(outcode0|outcode1)){
            aceito = true;
            feito = true;
        } else if (outcode0 & outcode1){
            feito = true;
        } else {
            GLint x, y;
            outcodeOut = outcode0?outcode0:outcode1;

            if (outcodeOut & TOP){
                x = P0.x+(P1.x - P0.x) *(max.y - P0.y)/(P1.y-P0.y);
                y  =max.y;
            } else if (outcodeOut & BOTTOM){
                x = P0.x+(P1.x - P0.x) *(min.y - P0.y)/(P1.y-P0.y);
                y  =min.y;
            } else if (outcodeOut & RIGHT){
                y = P0.y + (P1.y - P0.y)*(max.x - P0.x)/(P1.x - P0.x);
                x = max.x;
            } else if (outcodeOut & LEFT){
                y = P0.y + (P1.y - P0.y)*(min.x - P0.x)/(P1.x - P0.x);
                x = min.x;
            }
            if (outcodeOut == outcode0){
                P0.x = (x);
                P0.y = (y);
                outcode0 = CompOutCode(P0, max, min);
            } else {
                P1.x = (x);
                P1.y = (y);
                outcode1 = CompOutCode(P1, max, min);
            }
        }
    } while (feito==false);
    return aceito;
}

void descelecionaALL(){
    Lista *aux = init;
    onMouseClick = false;

    do{
        if (aux!=NULL){
            aux->objeto->selecionado = (false);
            // o código abaixo "deseleciona" os pontos de controle da forma, caso a operação realizada tenha sido um deslocamento de pontos
            if (aux->objeto->getTipo() == Objeto::RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                q->A.selecionado = (false);
                q->B.selecionado = (false);
                q->C.selecionado = (false);
                q->D.selecionado = (false);
            } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                e->getPControl()->selecionado = (false);
            } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                // TODO
                // Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                // pol->deseleciona();
            }
            aux = aux->next;
        }
    } while (aux!=NULL);
}

void selecionaCirculo(Lista *aux, Circulo *c, Ponto click){
    aux->objeto->selecionado = (true);
    aux->objeto->setXClick(click.x -c->getXc());
    aux->objeto->setYClick(click.y - c->getYc());
    if (op==COPIA){
        GLfloat fill[4], line[4];
        c->getColorFill(fill);
        c->getColorLine(line);
        Circulo *novo = new Circulo(c->getRaio(), c->getXc(), c->getYc(), fill,line, c->getEspessuraLinha());
        insere (novo);
        aux->objeto->selecionado = (false);
        fim->objeto->selecionado = (true);
        fim->objeto->setXClick(click.x -c->getXc() );
        fim->objeto->setYClick(click.y- c->getYc());
    }
}

void selecionaQuadrilatero(Lista *aux, Retangulo *q, Ponto click){
    aux->objeto->selecionado = (true);
   // aux->objeto->setXClick(-q->A.p.x+ click.x);
   // aux->objeto->setYClick(-q->A.p.y+click.y );
    aux->objeto->setXClick(click.x);
    aux->objeto->setYClick(click.y );

    if (op== COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Retangulo *novo = new Retangulo(q->A, q->B, q->C, q->D, fill, line, espessuraLinha);
        insere(novo);
        aux->objeto->selecionado = (false);
        fim->objeto->selecionado = (true);
        fim->objeto->setXClick(click.x);
        fim->objeto->setYClick(click.y );
    }
}

//!
//! \brief getAreaClippingMouse define a área de click do mouse
//! @param xmouse e ymouse são as coordenadas do mouse
//! @return área de clike do mouse (abrangência do clike)
//! \return
//!
//!
Retangulo* GLWidget::getAreaClippingMouse(GLint xmouse, GLint ymouse){
    Ponto A(xmouse-CONTROL, ymouse+CONTROL);
    Ponto D(xmouse+CONTROL, ymouse-CONTROL);
    Ponto B(xmouse-CONTROL, ymouse-CONTROL); // (xmin, ymin)
    Ponto C(xmouse+CONTROL, ymouse+CONTROL); // (xmax, ymax)GLfloat colorFill[4] = {0, 0, 0, 0};
    GLfloat colorLine[4] = {0, 0, 0, 0};
    GLfloat colorFill[4] = {0, 0, 0, 0};
    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, espessuraLinha);
    return q;
}

//!
//! \brief clippingAreaVertice diz se um ponto está dentro ou fora de um região quadriculada em torno de um vértice
//! \param vertice é o vertice entorno do qual teremos a região quadrada
//! \param teste é o ponto de teste
//! \return true se o ponto está dentro
//!
bool clippingAreaVertice(Ponto vertice, Ponto teste){
    /*cout<<"Area.."<<endl;
    cout<<vertice.x+CONTROL<<", "<<vertice.y+CONTROL<<endl;
    cout<<vertice.x+CONTROL<<", "<<vertice.y-CONTROL<<endl;
    cout<<vertice.x-CONTROL<<", "<<vertice.y+CONTROL<<endl;
    cout<<vertice.x-CONTROL<<", "<<vertice.y-CONTROL<<endl;
    cout<<"Click..."<<endl;
    cout<<teste.x<<", "<<teste.y<<endl;*/
    return teste.x<=(vertice.x+CONTROL) && teste.y<=(vertice.y+CONTROL) && teste.x>=(vertice.x-CONTROL) && teste.y>=(vertice.y-CONTROL);
}

/*
void excluirPontoControlePolilinha(Polilinha *pol, int x, int y){
    if (clippingAreaVertice(pol->getInit()->getP0(), Ponto(x, y))){
        pol->remove(pol->getInit());
    } else {
        Linha *linha = pol->getInit();
         while (linha!=NULL){
             if (clippingAreaVertice(linha->getP1(), Ponto(x, y))){
                 if (linha->getNext()!=NULL){
                     linha->getNext()->setP0(linha->getP0());
                 }
                 pol->selecionado = (true);
                 pol->remove(linha);
                 linha = NULL;
             } else linha = linha->getNext();
         }
    }

}
*/

void GLWidget::resizeGL(int w, int h) {
    float vfAspect = 0.f;
        float viewDepth = gfWrldSizeZ/2.f;

        // Usar toda a janela.
        glViewport(0, 0, w, h);

        h = (h==0) ? 1 : h;
        w = (w==0) ? 1 : w;
        vfAspect = ( h < w ) ? (float)w/(float)h : (float)h/(float)w ;
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        glOrtho( 0, w, // * vfAspect,
                 0, h, // * vfAspect,
                -viewDepth, viewDepth );

        gfWrldSizeX = w;
        gfWrldSizeY = h;
        glMatrixMode( GL_MODELVIEW );

        glLoadIdentity( );
}

void GLWidget::paintGL() {
    glClearColor( 0.9f, 0.9f, 0.9f, 0.f );
        glClear( GL_COLOR_BUFFER_BIT );

        glColor3f( 1.f, 0.f, 0.f );

        glPushMatrix( );
        glPointSize(3);
        Lista *aux = init;
        if (op==ROTACAO && opBotaoDireito){ // se a opção se rotação foi selecionada e o botão direito foi pressionado, então desenhe uma cruzinha
            glPointSize(10);
            glColor3f( 0,0 , 0 );
            cout<<clickCanvas.x<<", "<<clickCanvas.y<<endl;
            glBegin(GL_LINES);
                glVertex2i(clickCanvas.x+CONTROL, clickCanvas.y);
                glVertex2i(clickCanvas.x-CONTROL, clickCanvas.y);
                glVertex2i(clickCanvas.x, clickCanvas.y+CONTROL);
                glVertex2i(clickCanvas.x, clickCanvas.y-CONTROL);
            glEnd();
        }
        while (aux!=NULL){
             Objeto *c = aux->objeto;
            c->desenha();
            if (aux->objeto->selecionado){
                if (aux->objeto->getTipo() == Objeto::CIRCULO){
                    Circulo *c = dynamic_cast <Circulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(8);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(c->getXc(), c->getYc());
                        glVertex2i(c->getXc()+c->getRaio(), c->getYc());
                      glEnd();
                } else if (aux->objeto->tipo == Objeto::RETANGULO){
                    Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(8);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(q->A.p.x, q->A.p.y);
                        glVertex2i(q->B.p.x, q->B.p.y);
                        glVertex2i(q->C.p.x, q->C.p.y);
                        glVertex2i(q->D.p.x, q->D.p.y);
                    glEnd();
                } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                    Elipse *e = dynamic_cast <Elipse *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0xAAAA);
                    glColor3f(0,0.5, 0);
                    glBegin(GL_LINES);
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x,  e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i( e->getRaioHorizontal()+e->getCentro().p.x,  e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i( e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x,  e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i( e->getRaioHorizontal()+e->getCentro().p.x,  e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i( e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);

                    glEnd();
                    glDisable(GL_LINE_STIPPLE);
                    glPointSize(8);
                    glBegin(GL_POINTS);
                        glVertex2i(e->getCentro().p.x,  e->getCentro().p.y);
                        glVertex2i(e->getControl().p.x, e->getControl().p.y);
                    glEnd();

                }
            } else {
                if (aux->objeto->getTipo() == Objeto::CIRCULO){
                    Circulo *c = dynamic_cast <Circulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(1);
                    glColor3f( 0,0, 0 );
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(c->getXc()+CONTROL, c->getYc()+CONTROL);
                        glVertex2i(c->getXc()-CONTROL, c->getYc()+CONTROL);
                        glVertex2i(c->getXc()-CONTROL, c->getYc()-CONTROL);
                        glVertex2i(c->getXc()+CONTROL, c->getYc()-CONTROL);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(c->getXc()+c->getRaio()+CONTROL, c->getYc()+CONTROL);
                        glVertex2i(c->getXc()+c->getRaio()-CONTROL, c->getYc()+CONTROL);
                        glVertex2i(c->getXc()+c->getRaio()-CONTROL, c->getYc()-CONTROL);
                        glVertex2i(c->getXc()+c->getRaio()+CONTROL, c->getYc()-CONTROL);
                    glEnd();
                } else if (aux->objeto->getTipo() == Objeto::RETANGULO){
                    Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
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
                } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                    Elipse *e = dynamic_cast <Elipse *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(1);
                    glColor3f( 0,0, 0 );
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(e->getControl().p.x+CONTROL, e->getControl().p.y+CONTROL);
                        glVertex2i(e->getControl().p.x-CONTROL, e->getControl().p.y+CONTROL);
                        glVertex2i(e->getControl().p.x-CONTROL, e->getControl().p.y-CONTROL);
                        glVertex2i(e->getControl().p.x+CONTROL, e->getControl().p.y-CONTROL);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(e->getCentro().p.x+CONTROL, e->getCentro().p.y+CONTROL);
                        glVertex2i(e->getCentro().p.x-CONTROL, e->getCentro().p.y+CONTROL);
                        glVertex2i(e->getCentro().p.x-CONTROL, e->getCentro().p.y-CONTROL);
                        glVertex2i(e->getCentro().p.x+CONTROL, e->getCentro().p.y-CONTROL);

                   glEnd();
                   glEnable(GL_LINE_STIPPLE);
                   glLineStipple(1, 0xAAAA);
                   glBegin(GL_LINES);
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x,e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);

                   glEnd();
                   glDisable(GL_LINE_STIPPLE);
                }
            }
            aux = aux->next;
        }
        glPopMatrix( );
        // Flush the pipeline, swap the buffers
        glFlush();

}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton){ // se o botão esquerdo for clicado, cria a forma
        cout<<"Esquerdo"<<endl;
        if (desenha){
            if (onMouseClick==false){
                if (forma == Objeto::CIRCULO){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Circulo *c = new Circulo(0, xc, yc, color, color, espessuraLinha);
                    insere (c);
                } else if (forma == Objeto::RETANGULO) {
                    Ponto A(event->x(), gfWrldSizeY-event->y()); // ponto de clik
                    Ponto D(event->x(), gfWrldSizeY-event->y());
                    Ponto B(event->x(), gfWrldSizeY-event->y());
                    Ponto C(event->x(), gfWrldSizeY-event->y());
                    GLfloat colorFill[4] = {1, 0, 0, 0};
                    GLfloat colorLine[4] = {1, 0, 0, 0};
                    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, espessuraLinha);
                    insere(q);
                } else if (forma == Objeto::ELIPSE){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Elipse *e = new Elipse(Ponto(xc, yc), 0, 0, color, color, espessuraLinha);
                    insere (e);
                } else if (forma == Objeto::POLILINHA){
                    /*
                    Ponto click(event->x(),gfWrldSizeY-event->y());
                    GLfloat color[4] = {1, 0, 0, 0};
                    Polilinha *pol = new Polilinha(color, color, espessuraLinha, Objeto::POLILINHA);
                    insere(pol);
                    pol->insert(click, click,NULL);
                    */
                }
            } else { // se o mouse foi clicado anteriormente, e estivermos desenhando uma polinha ...
                 if (forma == Objeto::POLILINHA){
                     /*
                     Ponto click(event->x(),gfWrldSizeY-event->y());
                     onMouseClick = false;
                    Polilinha *pol = dynamic_cast <Polilinha *>(fim->objeto);
                    pol->insert(pol->getFim()->getP1(), click, pol->getFim());
                    */

                }
            }

            onMouseClick = !onMouseClick;
        } else {
            if(onMouseClick==false){ // então seleciona a forma
                Lista *aux = init;
                Lista *auxFim = fim;
                do{
                    if (aux!=NULL){
                        if (aux->objeto->getTipo() == Objeto::CIRCULO){
                            Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                            Ponto click = Ponto(event->x(), gfWrldSizeY-event->y());
                            GLint distancia =sqrt(pow(event->x()-c->getXc(), 2)+pow(gfWrldSizeY-event->y()-c->getYc(), 2));
                            if (abs(c->getRaio()-distancia)<=CONTROL){
                                selecionaCirculo(aux, c, click);
                            } else {
                                aux->objeto->selecionado = (false);
                            }
                            if (c->selecionado == false && c->preenchido){
                                if(distancia<=c->getRaio()){
                                    selecionaCirculo(aux, c, Ponto(event->x(), gfWrldSizeY-event->y()));
                                }
                            }
                        } else if(aux->objeto->getTipo() == Objeto::ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                            /* caso a elipse não esteja preenchicda,
                                 * então a única possibilidade de selecioná-la será
                                 * através do ponto de controle ou clicando sobre seus limites
                                 * que formam o desenho (e não as linhas potilhadas)
                             */
                             Ponto click(event->x(),gfWrldSizeY-event->y());
                             GLint a = e->getRaioHorizontal(), b=e->getRaioVertical();
                             GLint x = e->getCentro().p.x - click.x, y = e->getCentro().p.y- click.y;

                             cout<<"a: "<<a<<", b: "<<b<<endl;
                             cout<<"X: "<<x<<", Y: "<<y<<endl;
                             GLint fxy = b*b*x*x + a*a*y*y - a*a*b*b;
                             //GLint fxy = (y/b)*(y/b) + (x/a)*(x/a) -1 ;
                             if (clippingAreaVertice(e->getControl().p, click)){  // clicou sobre o ponto de controle ?
                                 aux->objeto->selecionado = (true);
                                 aux->objeto->setXClick(e->getControl().p.x - e->getCentro().p.x);
                                 aux->objeto->setYClick(e->getControl().p.y - e->getCentro().p.y);
                                 e->getPControl()->selecionado = (true);
                             } else{
                                 cout<<fxy<<endl;
                                 if(fxy==0){ // clicou sobre os limites da elispe?
                                     aux->objeto->selecionado = (true);
                                     aux->objeto->setXClick(e->getControl().p.x-e->getCentro().p.x);
                                     aux->objeto->setYClick(e->getControl().p.y-e->getCentro().p.y);
                                 } else {
                                     aux->objeto->selecionado = (false);
                                 }
                             }
                            //} //if (Preenchida){
                              //se a forma estiver preenchida
                            //}
                             if (aux->objeto->selecionado){
                                 if (op==COPIA){
                                     GLfloat fill[4], line[4];
                                     e->getColorFill(fill);
                                     e->getColorLine(line);
                                     Elipse *novo = new Elipse(e->getCentro().p, e->getRaioHorizontal(), e->getRaioVertical(), fill, line, espessuraLinha);
                                     novo->setControl(e->getControl());
                                     insere (novo);
                                     aux->objeto->selecionado = (false);
                                     fim->objeto->selecionado = (true);

                                     fim->objeto->setXClick(aux->objeto->getXClick());
                                     fim->objeto->setYClick(aux->objeto->getYClick());
                                 }
                             }

                        }else if (aux->objeto->getTipo() == Objeto::RETANGULO){
                             Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                             Ponto click(event->x(),gfWrldSizeY-event->y());
                             if (clippingAreaVertice(q->A.p, click) || clippingAreaVertice(q->B.p, click) || clippingAreaVertice(q->C.p, click) || clippingAreaVertice(q->D.p, click)){
                                 selecionaQuadrilatero(aux, q, click);
                                 if(clippingAreaVertice(q->A.p, click)){
                                     q->A.selecionado = (true);
                                 } else if(clippingAreaVertice(q->B.p, click) ){
                                     q->B.selecionado = (true);
                                 } else if (clippingAreaVertice(q->C.p, click)){
                                     q->C.selecionado = (true);
                                 } else q->D.selecionado = (true);
                             } else { // testa se o click foi na linha do quadrado (não preenchido)
                                 Retangulo *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                                 if (cohenClipping(q->A.p, q->B.p,clik->C.p, clik->B.p) || cohenClipping(q->B.p, q->D.p,clik->C.p, clik->B.p) || cohenClipping(q->D.p, q->C.p,clik->C.p, clik->B.p) || cohenClipping(q->C.p, q->A.p,clik->C.p, clik->B.p)){
                                     selecionaQuadrilatero(aux, q, Ponto(event->x(),gfWrldSizeY-event->y()));
                                 } else {
                                    aux->objeto->selecionado = (false);
                                 }
                             }
                             if (q->selecionado == false && q->preenchido){// se o quadrilátero estiver preenchido
                                 if (event->x()<=q->D.p.x && event->x()>=q->A.p.x && (gfWrldSizeY-event->y())<=q->A.p.y && (gfWrldSizeY-event->y())>=q->D.p.y){
                                        selecionaQuadrilatero(aux, q, click);
                                 }
                            }
                        } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                            /*
                            Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                            Quadrilatero *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                            Linha *linha = pol->getInit();
                            pol->selecionado = (false);
                            while (linha!=NULL && pol->isSelect()==false){
                                if (cohenClipping(linha->getP0(), linha->getP1(),clik->C.p, clik->B.p)){
                                    pol->selecionado = (true);
                                    pol->setXClick(event->x());
                                    pol->setYClick(gfWrldSizeY-event->y());
                                    if (op==ESCALA){
                                        clickCanvas.setX(event->x());
                                        clickCanvas.setY(gfWrldSizeY-event->y());
                                    }
                                    pol->setLinhaSelecionada1(linha);
                                    pol->setLinhaSelecionada2(NULL);
                                    if (clippingAreaVertice(linha->getP0(), Ponto(event->x(),gfWrldSizeY-event->y()))){
                                        linha->getPP0()->selecionado = (true);
                                        cout<<"Seelcionado: P0 - Linha 1"<<endl;
                                    } else {
                                        if (clippingAreaVertice(linha->getP1(), Ponto(event->x(),gfWrldSizeY-event->y()))){
                                            linha->getPP1()->selecionado = (true);
                                            pol->setLinhaSelecionada2(linha->getNext());
                                            cout<<"Seelcionado: P1 - Linha"<<endl;
                                            if (linha!=pol->getFim()){
                                                linha->getNext()->getPP0()->selecionado = (true);

                                                cout<<"Selecionado: P0 - Linha->next()"<<endl;
                                            }
                                        }
                                    }

                                }
                                linha = linha->getNext();
                            }
                            if (pol->isSelect()){
                                if (op==COPIA){
                                    GLfloat fill[4], line[4];
                                    pol->getColorFill(fill);
                                    pol->getColorLine(line);
                                    Polilinha *novo = new Polilinha(fill, line, espessuraLinha, Objeto::POLILINHA);
                                    insere(novo);
                                    Linha *linha = pol->getInit();
                                    while (linha!=NULL){
                                        novo->insert(linha->getP0(), linha->getP1(), novo->getFim());
                                        linha = linha->getNext();
                                    }
                                    aux->objeto->selecionado = (false);
                                    fim->objeto->selecionado = (true);
                                    fim->objeto->setXClick(aux->objeto->getXClick());
                                    fim->objeto->setYClick(aux->objeto->getYClick());
                                } else if (op==INSERT_REMOVE_PONTO){ //insere um novo vértice;
                                    Ponto newP1(pol->getLinhaSelecionada1()->getP1().x, pol->getLinhaSelecionada1()->getP1().y);
                                    Ponto newP0(event->x(), gfWrldSizeY-event->y());
                                    pol->insert(newP0, newP1, pol->getLinhaSelecionada1());
                                    pol->getLinhaSelecionada1()->setP1(newP0);
                                    pol->getLinhaSelecionada1()->getPP1()->selecionado = (true);
                                    pol->setLinhaSelecionada2(pol->getLinhaSelecionada1()->getNext());
                                    pol->getLinhaSelecionada2()->getPP0()->selecionado = (true);

                                }
                            }
                            */
                        }
                        if (op == ROTACAO){
                            if (opBotaoDireito==false) { // se o usuário não clicou com o botão direito fora da área do quadrado
                                clickCanvas.x = (event->x());
                                clickCanvas.y = (gfWrldSizeY-event->y());
                            }
                           rotacionaObjeto(aux->objeto);
                        }
                        aux = aux->next;
                    }
                }while (aux!=auxFim->next);
                onMouseClick = true;
                //updateGL();

            } else { // deseleciona
                descelecionaALL();
            }

        }
        updateGL();
    } else if ((event->buttons() & Qt::RightButton) == Qt::RightButton){
        if (desenha){
            if (onMouseClick){ // se durante a rasterização da forma o botão direito for pressionado, então delete a forma [QUALQUER]
                onMouseClick = false;
                remove(fim);
                updateGL();
            }
        } else {
            if (op==ROTACAO){
                clickCanvas.x = (event->x());
                clickCanvas.y = (gfWrldSizeY-event->y());
                opBotaoDireito = true;
                updateGL();
            } else if (op == INSERT_REMOVE_PONTO){ // removemos o ponto selecionado (apenas para polilinha)
                /*
                Lista *aux = init;
                Lista *auxFim = fim;
                do{
                    if (aux->objeto->getTipo()==Objeto::POLILINHA){
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        excluirPontoControlePolilinha(pol, event->x(), gfWrldSizeY-event->y());
                        if (pol->getInit()==NULL){
                            remove(aux);
                        }
                    }
                    aux = aux->next;
                }while (aux!=auxFim->next);
                updateGL();
                */
            }
        }
        desenhandoPolilinha = false; // da especidicação: quando clicado com o botão do meio, finaliza o desenho da polilinha
        cout<<"Direito"<<endl;
    } else if ((event->buttons() & Qt::MidButton) == Qt::MidButton){
        cout<<"MEIO"<<endl;
        onMouseClick = false;
    }
}
void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    //if (desenha==false){
       // onMouseClik = false;
   // }

}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (desenha){
        if(onMouseClick){
            if (fim->objeto->getTipo()== Objeto::CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(fim->objeto);
                c->redimensionar(event->x(), gfWrldSizeY - event->y());
            } else if (fim->objeto->getTipo() == Objeto::RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(fim->objeto);
                q->D = (Ponto(event->x(), gfWrldSizeY-event->y()));
                q->C = (Ponto(q->D.p.x, q->A.p.y));
                q->B = (Ponto(q->A.p.x, q->D.p.y));
            } else if (fim->objeto->getTipo() == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(fim->objeto);
                e->setRaioHorizontal(abs(event->x()-e->getCentro().p.x));
                e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().p.y));
                e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
            } else if (fim->objeto->getTipo()== Objeto::POLILINHA){
                /*
                Polilinha *pol = dynamic_cast <Polilinha *>(fim->objeto);
                Ponto click(event->x(),gfWrldSizeY-event->y());
                pol->setFim(pol->getFim()->getP0(), click);
                */
            }
        }
    } else if (onMouseClick){
        Lista *aux = init;
        while (aux!=NULL){
            if (aux->objeto->selecionado){
                if (op==TRANSLACAO || op==COPIA){
                    if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->setXc(event->x()-aux->objeto->getXClick());
                        c->setYc(gfWrldSizeY-event->y()-aux->objeto->getYClick());
                    } else if (aux->objeto->getTipo() == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        GLint dy = gfWrldSizeY-event->y() - q->getYClick();
                        GLint dx = event->x() - q->getXClick();
                        q->A = (Ponto(q->A.p.x+dx, q->A.p.y+dy));
                        q->B = (Ponto(q->B.p.x+dx, q->B.p.y+dy));
                        q->C = (Ponto(q->C.p.x+dx, q->C.p.y+dy));
                        q->D = (Ponto(q->D.p.x+dx, q->D.p.y+dy));
                        q->setXClick(event->x());
                        q->setYClick(gfWrldSizeY-event->y());

                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);
                        e->setCentro(Ponto(event->x()-e->getXClick(), gfWrldSizeY-event->y()- e->getYClick()));

                        e->setControl(Ponto(e->getCentro().p.x+ax*(e->getRaioHorizontal()),e->getCentro().p.y+ay*(e->getRaioVertical())));
                    } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        /*
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        GLint dy = gfWrldSizeY-event->y() - pol->getYClick();
                        GLint dx = event->x() - pol->getXClick();
                        Linha *linha = pol->getInit();

                        while (linha!=NULL){
                            linha->setP0(Ponto(linha->getP0().x+dx, linha->getP0().y+dy));
                            linha->setP1(Ponto(linha->getP1().x+dx, linha->getP1().y+dy));
                            linha = linha->getNext();
                        }
                        pol->setXClick(event->x());
                        pol->setYClick(gfWrldSizeY-event->y());
                        */
                    }
                } else if (op==ESCALA){
                    if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        Ponto centro =  q->centro.p;
                        GLdouble aux = distanciaAB;
                        distanciaAB = sqrt(pow(event->x()-centro.x, 2)+pow(gfWrldSizeY-event->y()-centro.y, 2));
                        GLdouble fatorx = 2, fatory = 2;//abs(distanciaAB-(xclick-centro.x)), fatory = abs(distanciaAB-(yclick -centro.y));
                        if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                            q->escala(-1*fatorx, -1*fatory);
                        } else{ // "saindo"
                            q->escala(fatorx, fatory);
                        }
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){  // de qualquer ponto selecionado
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint rh = abs(event->x()-e->getCentro().p.x)+1;
                        GLint rv = abs(gfWrldSizeY-event->y()-e->getCentro().p.y)+1;
                        GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);

                        e->setControl(Ponto(e->getCentro().p.x+ax*(rh),e->getCentro().p.y+ay*(rv)));
                        e->setRaioHorizontal(rh);
                        e->setRaioVertical(rv);
                    } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        /*
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
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

                            if (dx>0){ //Se distanciando de X;
                                cout<<"Saindo X"<<endl;
                                auxx = fatorx;
                            } else if (dx<0){ //Se aproximando de X;
                                cout<<"Entrando X"<<endl;
                                auxx = 0.99;
                            } else {
                                auxx=1;
                            }
                            linha->getPP0()->setX(linha->getP0().x); // desloca para origem
                            linha->getPP0()->setY(linha->getP0().y); // desloca para origem
                            linha->getPP1()->setX(linha->getP1().x); // desloca para origem
                            linha->getPP1()->setY(linha->getP1().y); // desloca para origem

                            linha->getPP0()->setX(linha->getP0().x*auxx);
                            linha->getPP0()->setY(linha->getP0().y*auxy);
                            linha->getPP1()->setX(linha->getP1().x*auxx);
                            linha->getPP1()->setY(linha->getP1().y*auxy);
                            linha = linha->getNext();
                        }
                        clickCanvas.setX(event->x());
                        clickCanvas.setY(gfWrldSizeY-event->y());
                        */
                    }
                } else if (op==DESLOCARPONTOS){
                    if (aux->objeto->getTipo() == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        if (q->A.selecionado){
                            cout<<"<<A>>"<<endl;
                            q->A = (Ponto(event->x(), gfWrldSizeY-event->y())); //
                            q->B = (Ponto(q->A.p.x, q->B.p.y));
                            q->C = (Ponto(q->C.p.x,q->A.p.y));
                        } else if (q->B.selecionado){
                            cout<<"<<B>>"<<endl;
                            q->B = (Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->A = (Ponto(q->B.p.x, q->A.p.y));
                            q->D = (Ponto(q->D.p.x, q->B.p.y));
                        } else if (q->C.selecionado){
                            cout<<"<<C>>"<<endl;
                            q->C = (Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->A = (Ponto(q->A.p.x, q->C.p.y));
                            q->D = (Ponto(q->C.p.x, q->D.p.y));
                        } else if (q->D.selecionado){
                            cout<<"<<D>>"<<endl;
                            q->D = (Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->B = (Ponto(q->B.p.x, q->D.p.y));
                            q->C = (Ponto(q->D.p.x, q->C.p.y));
                        }
                    } else if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        cout<<e->getControl().selecionado<<endl;
                        if (e->getControl().selecionado){ // apenas por ponto de controle
                            e->setRaioHorizontal(abs(event->x()-e->getCentro().p.x));
                            e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().p.y));
                            e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
                        }
                    } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        /*
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        deslocaPontoPolilinha(pol, event->x(), gfWrldSizeY-event->y());
                        */
                    }
                } else if (op==INSERT_REMOVE_PONTO){
                    if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        /*
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        deslocaPontoPolilinha(pol, event->x(), gfWrldSizeY-event->y());
                        */
                        }
                }
            }
            aux = aux->next;
        }
    }
    updateGL();
}

void GLWidget::setOperacao(QAction* q) {
    // Encontrar maneira melhor
    // Depender do texto é péssimo
    if(q->text() == "Retângulo") {
        forma = Objeto::RETANGULO;
        desenha = true;
    } else if(q->text() == "Círculo") {
        forma = Objeto::CIRCULO;
        desenha = true;
    } else if(q->text() == "Polilinha") {
        forma = Objeto::POLILINHA;
        desenha = true;
    } else if(q->text() == "Elipse") {
        forma = Objeto::ELIPSE;

        desenha = true;
    } else if(q->text() == "Translação") {
        op = TRANSLACAO;
        cout<<"Translação"<<endl;
        desenha = false;
    } else if(q->text() == "Cópia") {
        op = COPIA;
        cout<<"COPIA"<<endl;
        desenha = false;
    } else if(q->text() == "Escalar") {
        op = ESCALA;
        cout<<"Escala"<<endl;
        desenha = false;
    } else if(q->text() == "Deslocar pontos") {
        op = DESLOCARPONTOS;
        cout<<"DeslocamentoDePontos"<<endl;
        desenha = false;
    } else if(q->text() == "Rotacionar") {
        op = ROTACAO;
        cout<<"Rotacao"<<endl;
        desenha = false;
    } else if (q->text() == "insertRemovePonto"){
        op = INSERT_REMOVE_PONTO;
        cout<<"Inserção e remoção de pontos"<<endl;
        desenha = false;
    }
    opBotaoDireito = false;
    descelecionaALL(); // sempre que uma opção (de desenho ou transformação) for escolhida, desselecione todos os objetos
}
