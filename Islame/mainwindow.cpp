#include "mainwindow.h"
#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "Objeto.h"
using namespace std;

#define CONTROL 4 // largura, altura dos pontos de controle e da área de clip do mouse
/*
typedef struct _GLintPoint {
        GLint x;
        GLint y;
} GLintPoint ;


typedef struct l{
    GLintPoint p1, p2;
    l *next;
} GLlistaRetas;

*/


enum Transforcao {TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO};

/*
 * Lista encadeada de objetos (circulos, quadrilateros, elipes, polilinhas, e etc).
 * Não há necessidade de polimorfismo ou modelagem orientada a objeto. Por isso usamos struct
 */

typedef struct l{
    Objeto *objeto;
    l *next;
    l *previous;
}Lista;

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


GLfloat gfWrldSizeX = 800.f;
GLfloat gfWrldSizeY = 600.f;
GLfloat gfWrldSizeZ = 400.f;
GLint xmouse=0, ymouse=0;
bool onMouseClik = false;
bool desenha = true; // true = desenha. false = seleciona
GLint espessuraLinha = 2; //PEGAR ESTE VALOR DE UM SELETOR

Transforcao op = ROTACAO;
Forma forma = CIRCULO;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}

void GLWidget::initializeGL() {}

/*
GLintPoint p1, p2;

//! Algoritmo Bresenham
void Bresenham(GLintPoint p1, GLintPoint p2) {
    glPointSize(4); //especifica o diâmetro do ponto

    GLint x, y;
    if (p2.x!=p1.x && p2.y!=p1.y){
        GLfloat m = (GLfloat) (p2.y-p1.y) / (p2.x-p1.x);
        if (m>0  && m<1){

            GLint dx = p2.x - p1.x;
            GLint dy = p2.y - p1.y;
            GLint pk = 2*dy - dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;
            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.x<p2.x){
                while (x<p2.x){

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
                while (x>p2.x){

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
            GLint dx = p2.x - p1.x;
            GLint dy = - p2.y + p1.y;
            GLint pk = 2*dy + dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;

            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.x<p2.x){
                while (x<p2.x){
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
                while (x>p2.x){
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
            GLint dx = p2.x - p1.x;
            GLint dy = p2.y - p1.y;
            GLint pk = 2*dx + dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y<p2.y){
                while (y<p2.y){
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
                while (y>p2.y){
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
            GLint dx = p2.x - p1.x;
            GLint dy = -p2.y + p1.y;
            GLint pk = 2*dx - dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y>p2.y){
                while (y>p2.y){
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
                while (y<p2.y){
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

            x = p1.x;
            y = (GLfloat) p1.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y<p2.y){
                while (y<p2.y){
                    x++;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.y){
                    x--;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m==-1){
            x = p1.x;
            y = (GLfloat) p1.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y>p2.y){
                while (y>p2.y){
                    x++;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.y){
                    x--;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        }
    } else if (p2.y==p1.y){ //se a linha for horizontal:

        x = p1.x;
        y = (GLfloat) p1.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.x<p2.x){
            while (x<p2.x){
                x++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (x>p2.x){
                x--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    } else { // se a linha for vertical:
        x = p1.x;
        y = (GLfloat) p1.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.y<p2.y){
            while (y<p2.y){
                y++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (y>p2.y){
                y--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    }
}
*/



void selecionaQuadrilatero(Lista *aux, Quadrilatero *q, Ponto click){
    aux->objeto->setSelect(true);
    aux->objeto->setXClick(-q->getA().getX()+ click.getX());
    aux->objeto->setYClick(-q->getA().getY()+click.getY() );
    if (op==COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Quadrilatero *novo = new Quadrilatero(q->getA(), q->getB(), q->getC(), q->getD(), fill, line, espessuraLinha, QUADRILATERO);
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
Quadrilatero* getAreaClippingMouse(GLint xmouse, GLint ymouse){
    int size = 3;
    Ponto A(xmouse-CONTROL, ymouse+CONTROL);
    Ponto D(xmouse+CONTROL, ymouse-CONTROL);
    Ponto B(xmouse-CONTROL, ymouse-CONTROL); // (xmin, ymin)
    Ponto C(xmouse+CONTROL, ymouse+CONTROL); // (xmax, ymax)GLfloat colorFill[4] = {0, 0, 0, 0};
    GLfloat colorLine[4] = {0, 0, 0, 0};
    GLfloat colorFill[4] = {0, 0, 0, 0};
    Quadrilatero *q = new Quadrilatero(A, B, C, D, colorFill, colorLine, espessuraLinha, QUADRILATERO);
    return q;
}

//!
//! \brief clippingAreaVertice diz se um ponto está dentro ou fora de um região quadriculada em torno de um vértice
//! \param vertice é o vertice entorno do qual teremos a região quadrada
//! \param teste é o ponto de teste
//! \return true se o ponto está dentro
//!
bool clippingAreaVertice(Ponto vertice, Ponto teste){
    cout<<"Area.."<<endl;
    cout<<vertice.getX()+CONTROL<<", "<<vertice.getY()+CONTROL<<endl;
    cout<<vertice.getX()+CONTROL<<", "<<vertice.getY()-CONTROL<<endl;
    cout<<vertice.getX()-CONTROL<<", "<<vertice.getY()+CONTROL<<endl;
    cout<<vertice.getX()-CONTROL<<", "<<vertice.getY()-CONTROL<<endl;
    cout<<"Click..."<<endl;
    cout<<teste.getX()<<", "<<teste.getY()<<endl;
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
        while (aux!=NULL){
             Objeto *c = aux->objeto;
            c->desenha();
            if (aux->objeto->isSelect()){
                if (aux->objeto->getTipo()==CIRCULO){
                    Circulo *c = dynamic_cast <Circulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(6);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(c->getXc(), c->getYc());
                        glVertex2i(c->getXc()+c->getRaio(), c->getYc());
                      glEnd();
                } else if (aux->objeto->getTipo()==QUADRILATERO){
                    Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(6);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(q->getA().getX(), q->getA().getY());
                        glVertex2i(q->getB().getX(), q->getB().getY());
                        glVertex2i(q->getC().getX(), q->getC().getY());
                        glVertex2i(q->getD().getX(), q->getD().getY());
                    glEnd();
                }
            } else {
                if (aux->objeto->getTipo()==CIRCULO){
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
                } else if (aux->objeto->getTipo()==QUADRILATERO){
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
            if (onMouseClik==false){
                if (forma==CIRCULO){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Circulo *c = new Circulo(0, xc, yc, color, color, espessuraLinha, CIRCULO);
                    insere (c);
                } else if (forma==QUADRILATERO) {
                    Ponto A(event->x(), gfWrldSizeY-event->y()); // ponto de clik
                    Ponto D(event->x(), gfWrldSizeY-event->y());
                    Ponto B(event->x(), gfWrldSizeY-event->y());
                    Ponto C(event->x(), gfWrldSizeY-event->y());
                    GLfloat colorFill[4] = {1, 0, 0, 0};
                    GLfloat colorLine[4] = {1, 0, 0, 0};
                    Quadrilatero *q = new Quadrilatero(A, B, C, D, colorFill, colorLine, espessuraLinha, QUADRILATERO);
                    insere(q);
                }
            }
            onMouseClik = !onMouseClik;
        } else {
            if(onMouseClik==false){ // então seleciona a forma
                Lista *aux = init;
                Lista *auxFim = fim;
                do{
                    if (aux!=NULL){
                        if (aux->objeto->getTipo()==CIRCULO){
                            Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                            if (event->x()<=c->getXc()+c->getRaio() && (gfWrldSizeY-event->y())<c->getYc()+c->getRaio() && event->x()>c->getXc()-c->getRaio() && (gfWrldSizeY-event->y())>c->getYc()-c->getRaio()){
                                aux->objeto->setSelect(true);
                                aux->objeto->setXClick(event->x() -c->getXc());
                                aux->objeto->setYClick(gfWrldSizeY-event->y() - c->getYc());
                                if (op==COPIA){
                                    GLfloat fill[4], line[4];
                                    c->getColorFill(fill);
                                    c->getColorLine(line);
                                    Circulo *novo = new Circulo(c->getRaio(), c->getXc(), c->getYc(), fill,line, c->getEspessuraLinha(), CIRCULO);
                                    insere (novo);
                                    aux->objeto->setSelect(false);
                                    fim->objeto->setSelect(true);
                                    fim->objeto->setXClick(event->x() -c->getXc() );
                                    fim->objeto->setYClick(gfWrldSizeY-event->y()- c->getYc());
                                }
                            } else {
                                aux->objeto->setSelect(false);
                            }
                        } else if (aux->objeto->getTipo() == QUADRILATERO){
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
                        aux = aux->next;
                    }
                }while (aux!=auxFim->next);
                onMouseClik = true;
                //updateGL();

            } else { // deseleciona
                Lista *aux = init;
                onMouseClik = false;
                do{
                    if (aux!=NULL){
                        aux->objeto->setSelect(false);
                        if (aux->objeto->getTipo()==QUADRILATERO){
                            Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                            q->getPA()->setSelect(false);
                            q->getPB()->setSelect(false);
                            q->getPC()->setSelect(false);
                            q->getPD()->setSelect(false);
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
        if (onMouseClik){ // se durante a rasterização da forma o botão direito for pressionado, então delete a forma [QUALQUER]
            onMouseClik = false;
            remove(fim);
            updateGL();
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
        if (onMouseClik){
            if (fim->objeto->getTipo()==CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(fim->objeto);
                c->redimensionar(event->x(), gfWrldSizeY - event->y());
            } else if (fim->objeto->getTipo() == QUADRILATERO){
                Quadrilatero *q = dynamic_cast <Quadrilatero *>(fim->objeto);
                q->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
                q->setC(Ponto(q->getD().getX(), q->getA().getY()));
                q->setB(Ponto(q->getA().getX(), q->getD().getY()));
//                c->redimensionar(event->x(), gfWrldSizeY-event->y());
            }
        }
    } else if (onMouseClik){ // Primeiro Teste: translação
        Lista *aux = init;
        while (aux!=NULL){
            if (aux->objeto->isSelect()){
                if (op==TRANSLACAO || op==COPIA){
                    if (aux->objeto->getTipo() == CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->setXc(event->x()-aux->objeto->getXClick());
                        c->setYc(gfWrldSizeY-event->y()-aux->objeto->getYClick());
                    } else if (aux->objeto->getTipo() == QUADRILATERO){
                        Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                        GLint dx = + q->getD().getX() - q->getA().getX();
                        GLint dy = - q->getA().getY() + q->getD().getY();
                        Ponto A(event->x()-aux->objeto->getXClick(), gfWrldSizeY-event->y()-aux->objeto->getYClick()); // ponto de clik
                        Ponto D(A.getX()+dx, A.getY()+dy);
                        Ponto B(D.getX(), A.getY());
                        Ponto C(A.getX(), D.getY());
                        q->setA(A);
                        q->setB(B);
                        q->setC(C);
                        q->setD(D);
                    }
                } else if (op==ESCALA){
                    if (aux->objeto->getTipo() == CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == QUADRILATERO){

                    }
                } else if (op==DESLOCARPONTOS){
                    if (aux->objeto->getTipo()==QUADRILATERO){
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

void GLWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key()==83){ // se pressionar 's' ----> SUBSTITUIR INTERFACE
          desenha=false;
          onMouseClik = false;
    } else if (event->key()==68) { // se pressionar 'd'   ----> SUBSTITUIR INTERFACE
          desenha=true;
          Lista *aux=  init;
          while (aux!=NULL){
              aux->objeto->setSelect(false);
              aux = aux->next;
          }
          updateGL();
    } else if(event->key()==81){
           forma = QUADRILATERO;
    } else if (event->key()==67){
           forma = CIRCULO;
    }

}
