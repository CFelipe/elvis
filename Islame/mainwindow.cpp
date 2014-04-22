#include "mainwindow.h"
#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
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
bool onMouseClik = false;
bool desenha = true; // true = desenha. false = seleciona
GLint espessuraLinha = 2; //PEGAR ESTE VALOR DE UM SELETOR

Transforcao op = ROTACAO;
Forma forma = ELIPSE;
GLdouble anguloDeRotacao = M_PI/2; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)
bool opBotaoDireito = false;
/* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
 * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
 * opBotaoDireito=fase <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um ponto de controle
*/
Ponto clickCanvas;

GLdouble distanciaAB = 0; // guarda a distancia entre dois pontos quaisquer A e B


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


void retacionaObjeto(Objeto *ob){
    /* 1 ª Desloca os pontos para a origem
     * 2 ª rotaciona
     * 3 ª Devolve às cordenadas iniciais
     */
    if (ob->isSelect()){
        switch(ob->getTipo()){
            case QUADRILATERO :
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
            case CIRCULO:
                {
                Circulo *c = dynamic_cast <Circulo *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                GLint xc = c->getXc()-clickCanvas.getX();
                GLint yc = c->getYc()-clickCanvas.getY();
                c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.getX());
                c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.getY());
                }
                break;
            case ELIPSE:
                {
                Elipse *e = dynamic_cast <Elipse *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                GLint xc = e->getCentro().getX()-clickCanvas.getX();
                GLint yc = e->getCentro().getY()-clickCanvas.getY();
                GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);
                Ponto cen(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.getX(), xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.getY() );
                GLint aux = e->getRaioHorizontal();
                e->setRaioHorizontal(e->getRaioVertical());
                e->setRaioVertical(aux);
                e->setCentro(cen);
                e->setControl(Ponto(e->getCentro().getX()+ax*(e->getRaioHorizontal()),e->getCentro().getY()+ay*(e->getRaioVertical())));

                cout<<"ROTACAO ELISPE "<<endl;
                }
                break;
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
                } else if (aux->objeto->getTipo() == ELIPSE){
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
                } else if (aux->objeto->getTipo() == ELIPSE){
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
                } else if (forma==ELIPSE){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Elipse *e = new Elipse(Ponto(xc, yc), 0, 0, color, color, espessuraLinha, ELIPSE);
                    insere (e);
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
                        } else if(aux->objeto->getTipo() == ELIPSE){
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
                                     Elipse *novo = new Elipse(e->getCentro(), e->getRaioHorizontal(), e->getRaioVertical(), fill, line, espessuraLinha, ELIPSE);
                                     novo->setControl(e->getControl());
                                     insere (novo);
                                     aux->objeto->setSelect(false);
                                     fim->objeto->setSelect(true);

                                     fim->objeto->setXClick(aux->objeto->getXClick());
                                     fim->objeto->setYClick(aux->objeto->getYClick());
                                 }
                             }

                        }else if (aux->objeto->getTipo() == QUADRILATERO){
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
                        if (op==ROTACAO){
                            if (opBotaoDireito==false) { // se o usuário não clicou com o botão direito fora da área do quadrado
                                clickCanvas.setX(event->x());
                                clickCanvas.setY(gfWrldSizeY-event->y());
                            }
                           retacionaObjeto(aux->objeto);
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
                        // o código abaixo "deseleciona" os pontos de controle da forma, caso a operação realizada tenha sido um deslocamento de pontos
                        if (aux->objeto->getTipo()==QUADRILATERO){
                            Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                            q->getPA()->setSelect(false);
                            q->getPB()->setSelect(false);
                            q->getPC()->setSelect(false);
                            q->getPD()->setSelect(false);
                        } else if (aux->objeto->getTipo()==ELIPSE){
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
            if (onMouseClik){ // se durante a rasterização da forma o botão direito for pressionado, então delete a forma [QUALQUER]
                onMouseClik = false;
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
        if (onMouseClik){
            if (fim->objeto->getTipo()==CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(fim->objeto);
                c->redimensionar(event->x(), gfWrldSizeY - event->y());
            } else if (fim->objeto->getTipo() == QUADRILATERO){
                Quadrilatero *q = dynamic_cast <Quadrilatero *>(fim->objeto);
                q->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
                q->setC(Ponto(q->getD().getX(), q->getA().getY()));
                q->setB(Ponto(q->getA().getX(), q->getD().getY()));
            } else if (fim->objeto->getTipo()==ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(fim->objeto);
                e->setRaioHorizontal(abs(event->x()-e->getCentro().getX()));
                e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().getY()));
                e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
            }
        }
    } else if (onMouseClik){
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
                        Ponto C(D.getX(), A.getY());
                        Ponto B(A.getX(), D.getY());
                        q->setA(A);
                        q->setB(B);
                        q->setC(C);
                        q->setD(D);
                    } else if (aux->objeto->getTipo()==ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);
                        e->setCentro(Ponto(event->x()-e->getXClick(), gfWrldSizeY-event->y()- e->getYClick()));

                        e->setControl(Ponto(e->getCentro().getX()+ax*(e->getRaioHorizontal()),e->getCentro().getY()+ay*(e->getRaioVertical())));

                    }
                } else if (op==ESCALA){
                    if (aux->objeto->getTipo() == CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == QUADRILATERO){
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
                    } else if (aux->objeto->getTipo() == ELIPSE){  // de qualquer ponto selecionado
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
                    } else if (aux->objeto->getTipo() == CIRCULO){
                        // fazer primeiro a elipse;
                    } else if (aux->objeto->getTipo() == ELIPSE){
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
          opBotaoDireito = false;
          updateGL();
    } else if(event->key()==81){ //q
           forma = QUADRILATERO;
    } else if (event->key()==67){ //c
           forma = CIRCULO;
    } else if (event->key()==69){ //e
        forma = ELIPSE;
    } else if (Qt::Key_Escape){
        opBotaoDireito = false;
    }
    cout<<event->key()<<endl;
}
