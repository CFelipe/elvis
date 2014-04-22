#include "glwidget.h"
#include "objeto.h"
#include <iostream>
#include <cmath>
using namespace std;

#define CONTROL 5 // largura, altura dos pontos de controle e da área de clip do mouse

Operacao op;
Objeto::Forma forma;

GLint espessuraLinha;
bool preenchido; // true  -> a forma está preenchida com alguma cor
bool desenha; // true = desenha. false = seleciona

// ---------------
// Lista encadeada
typedef struct l {
    Objeto *objeto;
    l *next;
    l *previous;
} Lista;

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
GLdouble anguloDeRotacao = M_PI/2; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)

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
    if (ob->isSelect()){
        switch(ob->getTipo()){
            case Objeto::QUADRILATERO :
                {
                Quadrilatero *q = dynamic_cast <Quadrilatero *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                Ponto A = q->getA();
                Ponto B = q->getB();
                Ponto C = q->getC();
                Ponto D = q->getD();
                A.setX(A.getX()-clickCanvas.getX());
                A.setY(A.getY()-clickCanvas.getY());
                B.setX(B.getX()-clickCanvas.getX());
                B.setY(B.getY()-clickCanvas.getY());
                C.setX(C.getX()-clickCanvas.getX());
                C.setY(C.getY()-clickCanvas.getY());
                D.setX(D.getX()-clickCanvas.getX());
                D.setY(D.getY()-clickCanvas.getY());
                GLint aux = A.getX();
                A.setX(aux*cos(anguloDeRotacao) - A.getY()*sin(anguloDeRotacao) + clickCanvas.getX());
                A.setY(aux*sin(anguloDeRotacao) + A.getY()*cos(anguloDeRotacao) + clickCanvas.getY());
                aux = B.getX();
                B.setX(aux*cos(anguloDeRotacao) - B.getY()*sin(anguloDeRotacao) + clickCanvas.getX());
                B.setY(aux*sin(anguloDeRotacao) + B.getY()*cos(anguloDeRotacao) + clickCanvas.getY());
                aux = C.getX();
                C.setX(aux*cos(anguloDeRotacao) - C.getY()*sin(anguloDeRotacao) + clickCanvas.getX());
                C.setY(aux*sin(anguloDeRotacao) + C.getY()*cos(anguloDeRotacao) + clickCanvas.getY());
                aux = D.getX();
                D.setX(aux*cos(anguloDeRotacao) - D.getY()*sin(anguloDeRotacao) + clickCanvas.getX());
                D.setY(aux*sin(anguloDeRotacao) + D.getY()*cos(anguloDeRotacao) + clickCanvas.getY());
                q->setA(A);
                q->setB(B);
                q->setC(C);
                q->setD(D);}
                break;
            case Objeto::CIRCULO:
                {
                Circulo *c = dynamic_cast <Circulo *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                GLint xc = c->getXc()-clickCanvas.getX();
                GLint yc = c->getYc()-clickCanvas.getY();
                c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.getX());
                c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.getY());
                }
                break;
        case Objeto::ELIPSE:
                {

                }
            default:
                cout<<"DEFAUT"<<endl;
                break;

        }
    }
}

void selecionaQuadrilatero(Lista *aux, Quadrilatero *q, Ponto click){
    aux->objeto->setSelect(true);
    aux->objeto->setXClick(-q->getA().getX()+ click.getX());
    aux->objeto->setYClick(-q->getA().getY()+click.getY() );
    if (op== COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Quadrilatero *novo = new Quadrilatero(q->getA(), q->getB(), q->getC(), q->getD(), fill, line, espessuraLinha, Objeto::QUADRILATERO);
        insere(novo);
        aux->objeto->setSelect(false);
        fim->objeto->setSelect(true);
        fim->objeto->setXClick(-q->getA().getX() + click.getX());
        fim->objeto->setYClick(-q->getA().getY() + click.getY() );
    }
}

//!
//! \brief getAreaClippingMouse define a área de click do mouse
//! @param xmouse e ymouse são as coordenadas do mouse
//! @return área de clike do mouse (abrangência do clike)
//! \return
//!
//!
Quadrilatero* GLWidget::getAreaClippingMouse(GLint xmouse, GLint ymouse){
    Ponto A(xmouse-CONTROL, ymouse+CONTROL);
    Ponto D(xmouse+CONTROL, ymouse-CONTROL);
    Ponto B(xmouse-CONTROL, ymouse-CONTROL); // (xmin, ymin)
    Ponto C(xmouse+CONTROL, ymouse+CONTROL); // (xmax, ymax)GLfloat colorFill[4] = {0, 0, 0, 0};
    GLfloat colorLine[4] = {0, 0, 0, 0};
    GLfloat colorFill[4] = {0, 0, 0, 0};
    Quadrilatero *q = new Quadrilatero(A, B, C, D, colorFill, colorLine, espessuraLinha, Objeto::QUADRILATERO);
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
    cout<<vertice.getX()+CONTROL<<", "<<vertice.getY()+CONTROL<<endl;
    cout<<vertice.getX()+CONTROL<<", "<<vertice.getY()-CONTROL<<endl;
    cout<<vertice.getX()-CONTROL<<", "<<vertice.getY()+CONTROL<<endl;
    cout<<vertice.getX()-CONTROL<<", "<<vertice.getY()-CONTROL<<endl;
    cout<<"Click..."<<endl;
    cout<<teste.getX()<<", "<<teste.getY()<<endl;*/
    return teste.getX()<=(vertice.getX()+CONTROL) && teste.getY()<=(vertice.getY()+CONTROL) && teste.getX()>=(vertice.getX()-CONTROL) && teste.getY()>=(vertice.getY()-CONTROL);
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
            cout<<clickCanvas.getX()<<", "<<clickCanvas.getY()<<endl;
            glBegin(GL_LINES);
                glVertex2i(clickCanvas.getX()+CONTROL, clickCanvas.getY());
                glVertex2i(clickCanvas.getX()-CONTROL, clickCanvas.getY());
                glVertex2i(clickCanvas.getX(), clickCanvas.getY()+CONTROL);
                glVertex2i(clickCanvas.getX(), clickCanvas.getY()-CONTROL);
            glEnd();
        }
        while (aux!=NULL){
             Objeto *c = aux->objeto;
            c->desenha();
            if (aux->objeto->isSelect()){
                if (aux->objeto->getTipo() == Objeto::CIRCULO){
                    Circulo *c = dynamic_cast <Circulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(6);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(c->getXc(), c->getYc());
                        glVertex2i(c->getXc()+c->getRaio(), c->getYc());
                      glEnd();
                } else if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                    Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(6);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(q->getA().getX(), q->getA().getY());
                        glVertex2i(q->getB().getX(), q->getB().getY());
                        glVertex2i(q->getC().getX(), q->getC().getY());
                        glVertex2i(q->getD().getX(), q->getD().getY());
                    glEnd();
                } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                    Elipse *e = dynamic_cast <Elipse *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glEnable(GL_LINE_STIPPLE);
                    glLineStipple(1, 0xAAAA);
                    glColor3f(0,0.5, 0);
                    glBegin(GL_LINES);
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(), e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(),e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());
                         glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());

                    glEnd();
                    glDisable(GL_LINE_STIPPLE);
                    glPointSize(6);
                    glBegin(GL_POINTS);
                        glVertex2i(e->getCentro().getX(), e->getCentro().getY());
                        glVertex2i(e->getControl().getX(), e->getControl().getY());
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
                } else if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                    Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(1);
                    glColor3f( 0,0, 0 );
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(q->getA().getX()+CONTROL, q->getA().getY()+CONTROL);
                        glVertex2i(q->getA().getX()-CONTROL, q->getA().getY()+CONTROL);
                        glVertex2i(q->getA().getX()-CONTROL, q->getA().getY()-CONTROL);
                        glVertex2i(q->getA().getX()+CONTROL, q->getA().getY()-CONTROL);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(q->getB().getX()+CONTROL, q->getB().getY()+CONTROL);
                        glVertex2i(q->getB().getX()-CONTROL, q->getB().getY()+CONTROL);
                        glVertex2i(q->getB().getX()-CONTROL, q->getB().getY()-CONTROL);
                        glVertex2i(q->getB().getX()+CONTROL, q->getB().getY()-CONTROL);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(q->getC().getX()+CONTROL, q->getC().getY()+CONTROL);
                        glVertex2i(q->getC().getX()-CONTROL, q->getC().getY()+CONTROL);
                        glVertex2i(q->getC().getX()-CONTROL, q->getC().getY()-CONTROL);
                        glVertex2i(q->getC().getX()+CONTROL, q->getC().getY()-CONTROL);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(q->getD().getX()+CONTROL, q->getD().getY()+CONTROL);
                        glVertex2i(q->getD().getX()-CONTROL, q->getD().getY()+CONTROL);
                        glVertex2i(q->getD().getX()-CONTROL, q->getD().getY()-CONTROL);
                        glVertex2i(q->getD().getX()+CONTROL, q->getD().getY()-CONTROL);
                    glEnd();
                } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                    Elipse *e = dynamic_cast <Elipse *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(1);
                    glColor3f( 0,0, 0 );
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(e->getControl().getX()+CONTROL, e->getControl().getY()+CONTROL);
                        glVertex2i(e->getControl().getX()-CONTROL, e->getControl().getY()+CONTROL);
                        glVertex2i(e->getControl().getX()-CONTROL, e->getControl().getY()-CONTROL);
                        glVertex2i(e->getControl().getX()+CONTROL, e->getControl().getY()-CONTROL);
                    glEnd();
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(e->getCentro().getX()+CONTROL, e->getCentro().getY()+CONTROL);
                        glVertex2i(e->getCentro().getX()-CONTROL, e->getCentro().getY()+CONTROL);
                        glVertex2i(e->getCentro().getX()-CONTROL, e->getCentro().getY()-CONTROL);
                        glVertex2i(e->getCentro().getX()+CONTROL, e->getCentro().getY()-CONTROL);

                   glEnd();
                   glEnable(GL_LINE_STIPPLE);
                   glLineStipple(1, 0xAAAA);
                   glBegin(GL_LINES);
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(), e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(),e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());
                        glVertex2i(-e->getRaioHorizontal()+e->getCentro().getX(), -e->getRaioVertical()+e->getCentro().getY());

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
                    Circulo *c = new Circulo(0, xc, yc, color, color, espessuraLinha, Objeto::CIRCULO);
                    insere (c);
                } else if (forma == Objeto::QUADRILATERO) {
                    Ponto A(event->x(), gfWrldSizeY-event->y()); // ponto de clik
                    Ponto D(event->x(), gfWrldSizeY-event->y());
                    Ponto B(event->x(), gfWrldSizeY-event->y());
                    Ponto C(event->x(), gfWrldSizeY-event->y());
                    GLfloat colorFill[4] = {1, 0, 0, 0};
                    GLfloat colorLine[4] = {1, 0, 0, 0};
                    Quadrilatero *q = new Quadrilatero(A, B, C, D, colorFill, colorLine, espessuraLinha, Objeto::QUADRILATERO);
                    insere(q);
                } else if (forma == Objeto::ELIPSE){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Elipse *e = new Elipse(Ponto(xc, yc), 0, 0, color, color, espessuraLinha, Objeto::ELIPSE);
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
                        if (aux->objeto->getTipo() == Objeto::CIRCULO){
                            Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                            if (event->x()<=c->getXc()+c->getRaio() && (gfWrldSizeY-event->y())<c->getYc()+c->getRaio() && event->x()>c->getXc()-c->getRaio() && (gfWrldSizeY-event->y())>c->getYc()-c->getRaio()){
                                aux->objeto->setSelect(true);
                                aux->objeto->setXClick(event->x() -c->getXc());
                                aux->objeto->setYClick(gfWrldSizeY-event->y() - c->getYc());
                                if (op==COPIA){
                                    GLfloat fill[4], line[4];
                                    c->getColorFill(fill);
                                    c->getColorLine(line);
                                    Circulo *novo = new Circulo(c->getRaio(), c->getXc(), c->getYc(), fill,line, c->getEspessuraLinha(), Objeto::CIRCULO);
                                    insere (novo);
                                    aux->objeto->setSelect(false);
                                    fim->objeto->setSelect(true);
                                    fim->objeto->setXClick(event->x() -c->getXc() );
                                    fim->objeto->setYClick(gfWrldSizeY-event->y()- c->getYc());
                                }
                            } else {
                                aux->objeto->setSelect(false);
                            }
                        } else if(aux->objeto->getTipo() == Objeto::ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                            /* caso a elipse não esteja preenchicda,
                                 * então a única possibilidade de selecioná-la será
                                 * através do ponto de controle ou clicando sobre seus limites
                                 * que formam o desenho (e não as linhas potilhadas)
                             */
                             Ponto click(event->x(),gfWrldSizeY-event->y());
                             if (clippingAreaVertice(e->getControl(), click)){
                                 aux->objeto->setSelect(true);
                                 aux->objeto->setXClick(e->getControl().getX()-e->getCentro().getX());
                                 aux->objeto->setYClick(e->getControl().getY()-e->getCentro().getY());
                                 e->getPControl()->setSelect(true);
                            //} else {
                                 //sobre os limites da elipse rasterizada
                             } else {
                                  aux->objeto->setSelect(false);
                             }
                            //} //if (Preenchida){
                              //se a forma estiver preenchida
                            //}
                             if (aux->objeto->isSelect()){
                                 if (op==COPIA){
                                     GLfloat fill[4], line[4];
                                     e->getColorFill(fill);
                                     e->getColorLine(line);
                                     Elipse *novo = new Elipse(e->getCentro(), e->getRaioHorizontal(), e->getRaioVertical(), fill, line, espessuraLinha, Objeto::ELIPSE);
                                     novo->setControl(e->getControl());
                                     insere (novo);
                                     aux->objeto->setSelect(false);
                                     fim->objeto->setSelect(true);

                                     fim->objeto->setXClick(aux->objeto->getXClick());
                                     fim->objeto->setYClick(aux->objeto->getYClick());
                                 }
                             }

                        }else if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                             Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                             Ponto click(event->x(),gfWrldSizeY-event->y());
                             if (clippingAreaVertice(q->getA(), click) || clippingAreaVertice(q->getB(), click) || clippingAreaVertice(q->getC(), click) || clippingAreaVertice(q->getD(), click)){
                                 selecionaQuadrilatero(aux, q, click);
                                 if(clippingAreaVertice(q->getA(), click)){
                                     q->getPA()->setSelect(true);
                                 } else if(clippingAreaVertice(q->getB(), click) ){
                                     q->getPB()->setSelect(true);
                                 } else if (clippingAreaVertice(q->getC(), click)){
                                     q->getPC()->setSelect(true);
                                 } else q->getPD()->setSelect(true);
                             } else if (q->isPreenchido()){// se o quadrilátero estiver preenchido
                                 if (event->x()<=q->getD().getX() && event->x()>=q->getA().getX() && (gfWrldSizeY-event->y())<=q->getA().getY() && (gfWrldSizeY-event->y())>=q->getD().getY()){
                                        selecionaQuadrilatero(aux, q, click);
                                }
                                 /*
                                  * else {
                            // mais uma possibilidade:
                                // clicar na linha
                        }
                                  */
                            } else {
                                 aux->objeto->setSelect(false);
                             }
                        }
                        if (op == ROTACAO){
                            if (opBotaoDireito==false) { // se o usuário não clicou com o botão direito fora da área do quadrado
                                clickCanvas.setX(event->x());
                                clickCanvas.setY(gfWrldSizeY-event->y());
                            }
                           rotacionaObjeto(aux->objeto);
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
                        aux->objeto->setSelect(false);
                        // o código abaixo "deseleciona" os pontos de controle da forma, caso a operação realizada tenha sido um deslocamento de pontos
                        if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                            Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                            q->getPA()->setSelect(false);
                            q->getPB()->setSelect(false);
                            q->getPC()->setSelect(false);
                            q->getPD()->setSelect(false);
                        } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                            e->getPControl()->setSelect(false);
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
                clickCanvas.setX(event->x());
                clickCanvas.setY(gfWrldSizeY-event->y());
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
            if (fim->objeto->getTipo()== Objeto::CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(fim->objeto);
                c->redimensionar(event->x(), gfWrldSizeY - event->y());
            } else if (fim->objeto->getTipo() == Objeto::QUADRILATERO){
                Quadrilatero *q = dynamic_cast <Quadrilatero *>(fim->objeto);
                q->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
                q->setC(Ponto(q->getD().getX(), q->getA().getY()));
                q->setB(Ponto(q->getA().getX(), q->getD().getY()));
            } else if (fim->objeto->getTipo() == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(fim->objeto);
                e->setRaioHorizontal(abs(event->x()-e->getCentro().getX()));
                e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().getY()));
                e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
            }
        }
    } else if (onMouseClick){
        Lista *aux = init;
        while (aux!=NULL){
            if (aux->objeto->isSelect()){
                if (op==TRANSLACAO || op==COPIA){
                    if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->setXc(event->x()-aux->objeto->getXClick());
                        c->setYc(gfWrldSizeY-event->y()-aux->objeto->getYClick());
                    } else if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                        Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                        GLint dx = + q->getD().getX() - q->getA().getX();
                        GLint dy = - q->getA().getY() + q->getD().getY();
                        Ponto A(event->x()-aux->objeto->getXClick(), gfWrldSizeY-event->y()-aux->objeto->getYClick()); // ponto de clik
                        Ponto D(A.getX()+dx, A.getY()+dy);
                        Ponto C(D.getX(), A.getY());
                        Ponto B(A.getX(), D.getY());
                        q->setA(A);
                        q->setB(B);
                        q->setC(C);
                        q->setD(D);
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);
                        e->setCentro(Ponto(event->x()-e->getXClick(), gfWrldSizeY-event->y()- e->getYClick()));

                        e->setControl(Ponto(e->getCentro().getX()+ax*(e->getRaioHorizontal()),e->getCentro().getY()+ay*(e->getRaioVertical())));

                    }
                } else if (op==ESCALA){
                    if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                        Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                        Ponto centro = q->getCentro();
                        GLdouble aux = distanciaAB;
                        distanciaAB = sqrt(pow(event->x()-centro.getX(), 2)+pow(gfWrldSizeY-event->y()-centro.getY(), 2));
                        GLint fatorx = 3, fatory = 3;//abs(distanciaAB-(xclick-centro.getX())), fatory = abs(distanciaAB-(yclick -centro.getY()));
                        if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                            q->escala((-1)*fatorx, (-1)*fatory);
                        } else{ // "saindo"
                            q->escala(fatorx, fatory);
                        }
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){  // de qualquer ponto selecionado
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint rh = abs(event->x()-e->getCentro().getX())+1;
                        GLint rv = abs(gfWrldSizeY-event->y()-e->getCentro().getY())+1;
                        GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);

                        e->setControl(Ponto(e->getCentro().getX()+ax*(rh),e->getCentro().getY()+ay*(rv)));
                        e->setRaioHorizontal(rh);
                        e->setRaioVertical(rv);
                        //cout<<rh<<","<<rv<<endl;
                    }
                } else if (op==DESLOCARPONTOS){
                    if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                        Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                        if (q->getA().isSelect()){
                            q->setA(Ponto(event->x(), gfWrldSizeY-event->y())); //
                            q->setB(Ponto(q->getA().getX(), q->getB().getY()));
                            q->setC(Ponto(q->getC().getX(),q->getA().getY()));
                        } else if (q->getB().isSelect()){
                            q->setB(Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->setA(Ponto(q->getB().getX(), q->getA().getY()));
                            q->setD(Ponto(q->getD().getX(), q->getB().getY()));
                        } else if (q->getC().isSelect()){
                            q->setC(Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->setA(Ponto(q->getA().getX(), q->getC().getY()));
                            q->setD(Ponto(q->getC().getX(), q->getD().getY()));
                        } else if (q->getD().isSelect()){
                            q->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->setB(Ponto(q->getB().getX(), q->getD().getY()));
                            q->setC(Ponto(q->getD().getX(), q->getC().getY()));
                        }
                    } else if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        // fazer primeiro a elipse;
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        cout<<e->getControl().isSelect()<<endl;
                        if (e->getControl().isSelect()){ // apenas por ponto de controle
                            e->setRaioHorizontal(abs(event->x()-e->getCentro().getX()));
                            e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().getY()));
                            e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
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
        forma = Objeto::QUADRILATERO;
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
    } else if(q->text() == "Deslocar pontos") {
        op = DESLOCARPONTOS;
        desenha = false;
    } else if(q->text() == "Rotação") {
        op = ROTACAO;
        desenha = false;
    }
}
