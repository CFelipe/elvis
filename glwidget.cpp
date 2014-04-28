#include "glwidget.h"
#include "objeto.h"
#include "vertice.h"
#include "retangulo.h"
#include "circulo.h"
#include "elipse.h"

#include <iostream>
#include <cmath>
using namespace std;

#define CONTROL 5 // largura, altura dos Vertices de controle e da área de clip do mouse

Operacao op;
Objeto::Forma forma;

GLint espessuraLinha;
bool preenchido; // true  -> a forma está preenchida com alguma cor
bool desenha; // true = desenha. false = seleciona

GLfloat gfWrldSizeX = 800.f;
GLfloat gfWrldSizeY = 600.f;
GLfloat gfWrldSizeZ = 400.f;

Ponto clickCanvas = Ponto(0, 0);
GLdouble distanciaAB = 0; // guarda a distancia entre dois Vertices quaisquer A e B
GLdouble anguloDeRotacao = M_PI/2; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)

/* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
 * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
 * opBotaoDireito=fase <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um Vertice de controle
*/
bool opBotaoDireito = false;
bool onMouseClick = false;

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

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);

    op = TRANSLACAO;
    forma = Objeto::ELIPSE;
    //---------------------

    desenha = true;
    espessuraLinha = 2;
}

void GLWidget::initializeGL() {}

/*
void GLWidget::rotacionaObjeto(Objeto *ob){
    // 1 Desloca os Vertices para a origem
    // 2 Rotaciona
    // 3 Devolve as cordenadas iniciais

    if (ob->selecionado){
        switch(ob->tipo){
            case Objeto::RETANGULO:
                Retangulo *q = dynamic_cast <Retangulo *>(ob);
                Ponto A = q->A;
                Ponto B = q->B;
                Ponto C = q->C;
                Ponto D = q->D;
                A.x -= -clickCanvas.x;
                A.y -= -clickCanvas.y;
                B.setX(B.x-clickCanvas.x);
                B.setY(B.y-clickCanvas.y);
                C.setX(C.x-clickCanvas.x);
                C.setY(C.y-clickCanvas.y);
                D.setX(D.x-clickCanvas.x);
                D.setY(D.y-clickCanvas.y);
                GLint aux = A.x;
                A.setX(aux*cos(anguloDeRotacao) - A.y*sin(anguloDeRotacao) + clickCanvas.x);
                A.setY(aux*sin(anguloDeRotacao) + A.y*cos(anguloDeRotacao) + clickCanvas.y);
                aux = B.x;
                B.setX(aux*cos(anguloDeRotacao) - B.y*sin(anguloDeRotacao) + clickCanvas.x);
                B.setY(aux*sin(anguloDeRotacao) + B.y*cos(anguloDeRotacao) + clickCanvas.y);
                aux = C.x;
                C.setX(aux*cos(anguloDeRotacao) - C.y*sin(anguloDeRotacao) + clickCanvas.x);
                C.setY(aux*sin(anguloDeRotacao) + C.y*cos(anguloDeRotacao) + clickCanvas.y);
                aux = D.x;
                D.setX(aux*cos(anguloDeRotacao) - D.y*sin(anguloDeRotacao) + clickCanvas.x);
                D.setY(aux*sin(anguloDeRotacao) + D.y*cos(anguloDeRotacao) + clickCanvas.y);
                q->setA(A);
                q->setB(B);
                q->setC(C);
                q->setD(D);}
                break;
            case Objeto::CIRCULO:
                Circulo *c = dynamic_cast <Circulo *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                GLint xc = c->getXc() - clickCanvas.x;
                GLint yc = c->getYc() - clickCanvas.y;
                c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.x);
                c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.y);
                break;
            case Objeto::ELIPSE:
            default:
                cout<<"DEFAUT"<<endl;
                break;
    }
}
*/

/*
void selecionaRetangulo(Lista *aux, Retangulo *q, Vertice click) {
    aux->objeto->selecionado = true;
    aux->objeto->setXClick(-q->A.x+ click.x);
    aux->objeto->setYClick(-q->A.y+click.y );
    if (op== COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Retangulo *novo = new Retangulo(q->A, q->B, q->C, q->D, fill, line, espessuraLinha, Objeto::RETANGULO);
        insere(novo);
        aux->objeto->selecionado = false;
        fim->objeto->selecionado = true;
        fim->objeto->setXClick(-q->A.x + click.x);
        fim->objeto->setYClick(-q->A.y + click.y );
    }
}
*/

//!
//! \brief getAreaClippingMouse define a área de click do mouse
//! @param xmouse e ymouse são as coordenadas do mouse
//! @return área de clike do mouse (abrangência do clike)
//! \return
//!
//!
Retangulo* GLWidget::getAreaClippingMouse(GLint xmouse, GLint ymouse){
    Vertice A(xmouse-CONTROL, ymouse+CONTROL);
    Vertice D(xmouse+CONTROL, ymouse-CONTROL);
    Vertice B(xmouse-CONTROL, ymouse-CONTROL); // (xmin, ymin)
    Vertice C(xmouse+CONTROL, ymouse+CONTROL); // (xmax, ymax)GLfloat colorFill[4] = {0, 0, 0, 0};
    GLfloat colorLine[4] = {0, 0, 0, 0};
    GLfloat colorFill[4] = {0, 0, 0, 0};
    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, espessuraLinha);
    return q;
}

//!
//! \brief clippingAreaVertice diz se um Vertice está dentro ou fora de um região quadriculada em torno de um vértice
//! \param vertice é o vertice entorno do qual teremos a região quadrada
//! \param teste é o Vertice de teste
//! \return true se o Vertice está dentro
//!
bool clippingAreaVertice(Vertice vertice, Vertice teste){
    /*cout<<"Area.."<<endl;
    cout<<vertice.x+CONTROL<<", "<<vertice.y+CONTROL<<endl;
    cout<<vertice.x+CONTROL<<", "<<vertice.y-CONTROL<<endl;
    cout<<vertice.x-CONTROL<<", "<<vertice.y+CONTROL<<endl;
    cout<<vertice.x-CONTROL<<", "<<vertice.y-CONTROL<<endl;
    cout<<"Click..."<<endl;
    cout<<teste.x<<", "<<teste.y<<endl;*/
    return teste.p.x <= (vertice.p.x + CONTROL) &&
           teste.p.y <= (vertice.p.y + CONTROL) &&
           teste.p.x >= (vertice.p.x - CONTROL) &&
           teste.p.y >= (vertice.p.y - CONTROL);
}

void GLWidget::resizeGL(int w, int h) {
    float vfAspect = 0.f;
    float viewDepth = gfWrldSizeZ/2.f;

    // Usar toda a janela.
    glViewport(0, 0, w, h);

    h = (h==0) ? 1 : h;
    w = (w==0) ? 1 : w;
    vfAspect = ( h < w ) ? (float)w/(float)h : (float)h/(float)w ;
    cout<<w<<", "<<h<<endl;
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
            glVertex2i(clickCanvas.x + CONTROL, clickCanvas.y);
            glVertex2i(clickCanvas.x -CONTROL, clickCanvas.y);
            glVertex2i(clickCanvas.x, clickCanvas.y+CONTROL);
            glVertex2i(clickCanvas.x, clickCanvas.y-CONTROL);
        glEnd();
    }
    while (aux!=NULL){
         Objeto *c = aux->objeto;
        c->desenha();
        if (aux->objeto->selecionado){
            if (aux->objeto->tipo == Objeto::CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                glPointSize(6);
                glBegin(GL_POINTS);
                    glColor3f( 0,0.5 , 0 );
                    glVertex2i(c->getXc(), c->getYc());
                    glVertex2i(c->getXc()+c->getRaio(), c->getYc());
                  glEnd();
            } else if (aux->objeto->tipo == Objeto::RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                glPointSize(6);
                glBegin(GL_POINTS);
                    glColor3f( 0,0.5 , 0 );
                    glVertex2i(q->A.p.x, q->A.p.y);
                    glVertex2i(q->B.p.x, q->B.p.y);
                    glVertex2i(q->C.p.x, q->C.p.y);
                    glVertex2i(q->D.p.x, q->D.p.y);
                glEnd();
            } else if (aux->objeto->tipo == Objeto::ELIPSE) {
                Elipse *e = dynamic_cast <Elipse *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(1, 0xAAAA);
                glColor3f(0,0.5, 0);
                glBegin(GL_LINES);
                     glVertex2i(-e->getRaioHorizontal() + e->getCentro().p.x,  e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i(-e->getRaioHorizontal() + e->getCentro().p.x, -e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i( e->getRaioHorizontal() + e->getCentro().p.x,  e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i( e->getRaioHorizontal() + e->getCentro().p.x, -e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i(-e->getRaioHorizontal() + e->getCentro().p.x,  e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i( e->getRaioHorizontal() + e->getCentro().p.x,  e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i( e->getRaioHorizontal() + e->getCentro().p.x, -e->getRaioVertical() + e->getCentro().p.y);
                     glVertex2i(-e->getRaioHorizontal() + e->getCentro().p.x, -e->getRaioVertical() + e->getCentro().p.y);

                glEnd();
                glDisable(GL_LINE_STIPPLE);
                glPointSize(6);
                glBegin(GL_POINTS);
                    glVertex2i(e->getCentro().p.x,  e->getCentro().p.y);
                    glVertex2i(e->getControl().p.x, e->getControl().p.y);
                glEnd();

            }
        } else {
            if (aux->objeto->tipo == Objeto::CIRCULO){
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
            } else if (aux->objeto->tipo == Objeto::RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                glPointSize(1);
                glColor3f( 0,0, 0 );
                glBegin(GL_LINE_LOOP);
                    glVertex2i(q->A.p.x + CONTROL, q->A.p.y + CONTROL);
                    glVertex2i(q->A.p.x - CONTROL, q->A.p.y + CONTROL);
                    glVertex2i(q->A.p.x - CONTROL, q->A.p.y - CONTROL);
                    glVertex2i(q->A.p.x + CONTROL, q->A.p.y - CONTROL);
                glEnd();
                glBegin(GL_LINE_LOOP);
                    glVertex2i(q->B.p.x + CONTROL, q->B.p.y + CONTROL);
                    glVertex2i(q->B.p.x - CONTROL, q->B.p.y + CONTROL);
                    glVertex2i(q->B.p.x - CONTROL, q->B.p.y - CONTROL);
                    glVertex2i(q->B.p.x + CONTROL, q->B.p.y - CONTROL);
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
            } else if (aux->objeto->tipo == Objeto::ELIPSE){
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
        if (desenha){
            if (onMouseClick==false){
                if (forma == Objeto::CIRCULO){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Circulo *c = new Circulo(0, xc, yc, color, color, espessuraLinha);
                    insere (c);
                } else if (forma == Objeto::RETANGULO) {
                    Vertice A(Ponto(event->x(), gfWrldSizeY-event->y())); // Vertice de clique
                    Vertice D(Ponto(event->x(), gfWrldSizeY-event->y()));
                    Vertice B(Ponto(event->x(), gfWrldSizeY-event->y()));
                    Vertice C(Ponto(event->x(), gfWrldSizeY-event->y()));
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
                }
            }
            onMouseClick = !onMouseClick;
        } else {
            if(onMouseClick==false){ // então seleciona a forma
                Lista *aux = init;
                Lista *auxFim = fim;
                do{
                    if (aux!=NULL){
                        if (aux->objeto->tipo == Objeto::CIRCULO){
                            Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                            if (event->x()<=c->getXc()+c->getRaio() && (gfWrldSizeY-event->y())<c->getYc()+c->getRaio() && event->x()>c->getXc()-c->getRaio() && (gfWrldSizeY-event->y())>c->getYc()-c->getRaio()){
                                aux->objeto->selecionado = true;
                                aux->objeto->setXClick(event->x() -c->getXc());
                                aux->objeto->setYClick(gfWrldSizeY-event->y() - c->getYc());
                                if (op==COPIA){
                                    GLfloat fill[4], line[4];
                                    c->getColorFill(fill);
                                    c->getColorLine(line);
                                    Circulo *novo = new Circulo(c->getRaio(), c->getXc(), c->getYc(), fill,line, c->getEspessuraLinha());
                                    insere (novo);
                                    aux->objeto->selecionado = false;
                                    fim->objeto->selecionado = true;
                                    fim->objeto->setXClick(event->x() -c->getXc() );
                                    fim->objeto->setYClick(gfWrldSizeY-event->y()- c->getYc());
                                }
                            } else {
                                aux->objeto->selecionado = false;
                            }
                        } else if(aux->objeto->tipo == Objeto::ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                            /* caso a elipse não esteja preenchicda,
                                 * então a única possibilidade de selecioná-la será
                                 * através do Vertice de controle ou clicando sobre seus limites
                                 * que formam o desenho (e não as linhas potilhadas)
                             */
                             Vertice click(event->x(),gfWrldSizeY-event->y());
                             if (clippingAreaVertice(e->getControl(), click)){
                                 aux->objeto->selecionado = true;
                                 aux->objeto->setXClick(e->getControl().p.x-e->getCentro().p.x);
                                 aux->objeto->setYClick(e->getControl().p.y-e->getCentro().p.y);
                                 e->getPControl()->selecionado = true;
                            //} else {
                                 //sobre os limites da elipse rasterizada
                             } else {
                                  aux->objeto->selecionado = false;
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
                                     aux->objeto->selecionado = false;
                                     fim->objeto->selecionado = true;

                                     fim->objeto->setXClick(aux->objeto->getXClick());
                                     fim->objeto->setYClick(aux->objeto->getYClick());
                                 }
                             }

                        } else if (aux->objeto->tipo == Objeto::RETANGULO){
                             Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                             Vertice click(event->x(),gfWrldSizeY-event->y());
                             if (clippingAreaVertice(q->A, click) || clippingAreaVertice(q->B, click) || clippingAreaVertice(q->C, click) || clippingAreaVertice(q->D, click)){
                                 // selecionaRetangulo(aux, q, click); ???
                                 if(clippingAreaVertice(q->A, click)){
                                     q->A.selecionado = true;
                                 } else if(clippingAreaVertice(q->B, click) ){
                                     q->B.selecionado = true;
                                 } else if (clippingAreaVertice(q->C, click)){
                                     q->C.selecionado = true;
                                 } else {
                                     q->D.selecionado = true;
                                 }
                             } else if (q->preenchido) {// se o quadrilátero estiver preenchido
                                 if (event->x() <= q->D.p.x &&
                                     event->x() >= q->A.p.x &&
                                     (gfWrldSizeY-event->y()) <= q->A.p.y &&
                                     (gfWrldSizeY-event->y()) >= q->D.p.y) {
                                    //selecionaRetangulo(aux, q, click); ??????
                                }
                                 /*
                                  * else {
                            // mais uma possibilidade:
                                // clicar na linha
                        }
                                  */
                            } else {
                                 aux->objeto->selecionado = false;
                             }
                        }
                        if (op == ROTACAO){
                            if (opBotaoDireito==false) { // se o usuário não clicou com o botão direito fora da área do quadrado
                                clickCanvas.x = (event->x());
                                clickCanvas.y = (gfWrldSizeY - event->y());
                            }
                           //rotacionaObjeto(aux->objeto); ??????
                        }
                        aux = aux->next;
                    }
                }while (aux!=auxFim->next);
                onMouseClick = true;
                //updateGL();

            } else { // deseleciona
                Lista *aux = init;
                onMouseClick = false;
                do{
                    if (aux!=NULL){
                        aux->objeto->selecionado = false;
                        // o código abaixo "deseleciona" os Vertices de controle da forma, caso a operação realizada tenha sido um deslocamento de Vertices
                        if (aux->objeto->tipo == Objeto::RETANGULO){
                            Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                            q->A.selecionado = false;
                            q->B.selecionado = false;
                            q->C.selecionado = false;
                            q->D.selecionado = false;
                        } else if (aux->objeto->tipo == Objeto::ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                            e->getPControl()->selecionado = false;
                        }
                    }
                    aux = aux->next;
                } while (aux!=NULL);
            }

        }
        updateGL();
       /* GLintPoint p1, p2;
        p1.x = event->x();
        p1.y = gfWrldSizeY-event->y();
        p2.x = event->x();
        p2.y = gfWrldSizeY-event->y();
        insere(p1, p2);
        onMouseClik = !onMouseClik;*/
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
            }
        }
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
            if (fim->objeto->tipo== Objeto::CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(fim->objeto);
                // c->redimensionar(event->x(), gfWrldSizeY - event->y()); ??????
            } else if (fim->objeto->tipo == Objeto::RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(fim->objeto);
                q->D = Vertice(event->x(), gfWrldSizeY-event->y());
                q->C = Vertice(q->D.p.x, q->A.p.y);
                q->B = Vertice(q->A.p.x, q->D.p.y);
            } else if (fim->objeto->tipo == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(fim->objeto);
                e->setRaioHorizontal(abs(event->x()-e->getCentro().p.x));
                e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().p.y));
                e->setControl(Vertice(event->x(), gfWrldSizeY-event->y()));
            }
        }
    } else if (onMouseClick){
        Lista *aux = init;
        while (aux!=NULL){
            if (aux->objeto->selecionado){
                if (op==TRANSLACAO || op==COPIA){
                    if (aux->objeto->tipo == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->setXc(event->x()-aux->objeto->getXClick());
                        c->setYc(gfWrldSizeY-event->y()-aux->objeto->getYClick());
                    } else if (aux->objeto->tipo == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        GLint dx = + q->D.p.x - q->A.p.x;
                        GLint dy = - q->A.p.y + q->D.p.y;
                        Vertice A(event->x()-aux->objeto->getXClick(), gfWrldSizeY-event->y()-aux->objeto->getYClick()); // Vertice de clik
                        Vertice D(A.p.x + dx, A.p.y + dy);
                        Vertice C(D.p.x, A.p.y);
                        Vertice B(A.p.x, D.p.y);
                        q->A = A;
                        q->B = B;
                        q->C = C;
                        q->D = D;
                    } else if (aux->objeto->tipo == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);
                        e->setCentro(Vertice(event->x()-e->getXClick(), gfWrldSizeY-event->y()- e->getYClick()));

                        e->setControl(Vertice(e->getCentro().p.x+ax*(e->getRaioHorizontal()),e->getCentro().p.y+ay*(e->getRaioVertical())));

                    }
                } else if (op==ESCALA){
                    if (aux->objeto->tipo == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        //c->redimensionar(event->x(), gfWrldSizeY-event->y()); ?????
                    } else if (aux->objeto->tipo == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        Vertice centro = q->centro;
                        GLdouble aux = distanciaAB;
                        distanciaAB = sqrt(pow(event->x()-centro.p.x, 2)+pow(gfWrldSizeY-event->y()-centro.p.y, 2));
                        GLint fatorx = 3, fatory = 3;//abs(distanciaAB-(xclick-centro.x)), fatory = abs(distanciaAB-(yclick -centro.y));
                        if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                            //q->escala((-1)*fatorx, (-1)*fatory); ?????
                        } else{ // "saindo"
                            //q->escala(fatorx, fatory); ??????
                        }
                    } else if (aux->objeto->tipo == Objeto::ELIPSE){  // de qualquer Vertice selecionado
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint rh = abs(event->x()-e->getCentro().p.x)+1;
                        GLint rv = abs(gfWrldSizeY-event->y()-e->getCentro().p.y)+1;
                        GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);

                        e->setControl(Vertice(e->getCentro().p.x+ax*(rh),e->getCentro().p.y+ay*(rv)));
                        e->setRaioHorizontal(rh);
                        e->setRaioVertical(rv);
                        //cout<<rh<<","<<rv<<endl;
                    }
                } else if (op == DESLOCARPONTOS){
                    if (aux->objeto->tipo == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        if (q->A.selecionado){
                            q->A = Vertice(event->x(), gfWrldSizeY-event->y()); //
                            q->B = Vertice(q->A.p.x, q->B.p.y);
                            q->C = Vertice(q->C.p.x, q->A.p.y);
                        } else if (q->B.selecionado){
                            q->B = Vertice(event->x(), gfWrldSizeY-event->y());
                            q->A = Vertice(q->B.p.x, q->A.p.y);
                            q->D = Vertice(q->D.p.x, q->B.p.y);
                        } else if (q->C.selecionado){
                            q->C = Vertice(event->x(), gfWrldSizeY-event->y());
                            q->A = Vertice(q->A.p.x, q->C.p.y);
                            q->D = Vertice(q->C.p.x, q->D.p.y);
                        } else if (q->D.selecionado){
                            q->D = Vertice(event->x(), gfWrldSizeY-event->y());
                            q->B = Vertice(q->B.p.x, q->D.p.y);
                            q->C = Vertice(q->D.p.x, q->C.p.y);
                        }
                    } else if (aux->objeto->tipo == Objeto::CIRCULO){
                        // fazer primeiro a elipse;
                    } else if (aux->objeto->tipo == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        cout<<e->getControl().selecionado<<endl;
                        if (e->getControl().selecionado){ // apenas por Vertice de controle
                            e->setRaioHorizontal(abs(event->x()-e->getCentro().p.x));
                            e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().p.y));
                            e->setControl(Vertice(event->x(), gfWrldSizeY-event->y()));
                        }
                    }
                }
            }
            aux = aux->next;
        }
    }
    updateGL();
    //if (onMouseClik){

        /*fim->p2.x = event->x();
        fim->p2.y = gfWrldSizeY-event->y();
        updateGL();*/
    //}
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
        //this->forma = Objeto::POLILINHA;
        qDebug() << "Não implementado";
    } else if(q->text() == "Elipse") {
        forma = Objeto::ELIPSE;
        desenha = true;
    } else if(q->text() == "Translação") {
        op = TRANSLACAO;
        desenha = false;
    } else if(q->text() == "Cópia") {
        op = COPIA;
        desenha = false;
    } else if(q->text() == "Escala") {
        op = ESCALA;
        desenha = false;
    } else if(q->text() == "Deslocar Vertices") {
        op = DESLOCARPONTOS;
        desenha = false;
    } else if(q->text() == "Rotação") {
        op = ROTACAO;
        desenha = false;
    }
}
