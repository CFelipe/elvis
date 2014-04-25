#include "glwidget.h"
#include "objeto.h"
#include <iostream>
#include <cmath>
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
        case Objeto::POLILINHA:
        {
                Polilinha *pol = dynamic_cast <Polilinha *>(ob);
                Linha *linha = pol->getInit();
                while (linha!=NULL){
                    Ponto P0 = linha->getP0();
                    Ponto P1 = linha->getP1();
                    P0.setX(P0.getX()-clickCanvas.getX());
                    P0.setY(P0.getY()-clickCanvas.getY());
                    P1.setX(P1.getX()-clickCanvas.getX());
                    P1.setY(P1.getY()-clickCanvas.getY());
                    GLint aux = P0.getX();
                    P0.setX(aux*cos(anguloDeRotacao) - P0.getY()*sin(anguloDeRotacao) + clickCanvas.getX());
                    P0.setY(aux*sin(anguloDeRotacao) + P0.getY()*cos(anguloDeRotacao) + clickCanvas.getY());
                    aux = P1.getX();
                    P1.setX(aux*cos(anguloDeRotacao) - P1.getY()*sin(anguloDeRotacao) + clickCanvas.getX());
                    P1.setY(aux*sin(anguloDeRotacao) + P1.getY()*cos(anguloDeRotacao) + clickCanvas.getY());
                    linha->setP0(P0);
                    linha->setP1(P1);
                    linha = linha->getNext();
                }
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
    if (p.getY()>max.getY()){
        code |= 0x1;
    } else if (p.getY()<min.getY()){
        code |= 0x2;
    }
    if (p.getX()>max.getX()){
        code |= 0x4;
    } else if (p.getX()<min.getX()){
        code |= 0x8;
    }
    return code;
}

void deslocaPontoPolilinha(Polilinha *pol, int x, int y){
    Linha *sel1 = pol->getLinhaSelecionada1(); // nunca é NULL
    Linha *sel2 = pol->getLinhaSelecionada2();
    GLint dy = y - pol->getYClick();
    GLint dx = x - pol->getXClick();
    if (sel1->getP0().isSelect()){ // linha 1, portanto sel2=NULL
        sel1->setP0(Ponto(sel1->getP0().getX()+dx, sel1->getP0().getY()+dy));
    } else if (sel1->getP1().isSelect()){
        sel1->setP1(Ponto(sel1->getP1().getX()+dx, sel1->getP1().getY()+dy));
        if (sel2!=NULL){
            sel2->setP0(Ponto(sel2->getP0().getX()+dx, sel2->getP0().getY()+dy));
        }
    }
    pol->setXClick(x);
    pol->setYClick(y);
}

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
                x = P0.getX()+(P1.getX() - P0.getX()) *(max.getY() - P0.getY())/(P1.getY()-P0.getY());
                y  =max.getY();
            } else if (outcodeOut & BOTTOM){
                x = P0.getX()+(P1.getX() - P0.getX()) *(min.getY() - P0.getY())/(P1.getY()-P0.getY());
                y  =min.getY();
            } else if (outcodeOut & RIGHT){
                y = P0.getY() + (P1.getY() - P0.getY())*(max.getX() - P0.getX())/(P1.getX() - P0.getX());
                x = max.getX();
            } else if (outcodeOut & LEFT){
                y = P0.getY() + (P1.getY() - P0.getY())*(min.getX() - P0.getX())/(P1.getX() - P0.getX());
                x = min.getX();
            }
            if (outcodeOut == outcode0){
                P0.setX(x);
                P0.setY(y);
                outcode0 = CompOutCode(P0, max, min);
            } else {
                P1.setX(x);
                P1.setY(y);
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
            } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                pol->deseleciona();
            }
            aux = aux->next;
        }
    } while (aux!=NULL);
}

void selecionaCirculo(Lista *aux, Circulo *c, Ponto click){
    aux->objeto->setSelect(true);
    aux->objeto->setXClick(click.getX() -c->getXc());
    aux->objeto->setYClick(click.getY() - c->getYc());
    if (op==COPIA){
        GLfloat fill[4], line[4];
        c->getColorFill(fill);
        c->getColorLine(line);
        Circulo *novo = new Circulo(c->getRaio(), c->getXc(), c->getYc(), fill,line, c->getEspessuraLinha(), Objeto::CIRCULO);
        insere (novo);
        aux->objeto->setSelect(false);
        fim->objeto->setSelect(true);
        fim->objeto->setXClick(click.getX() -c->getXc() );
        fim->objeto->setYClick(click.getY()- c->getYc());
    }
}

void selecionaQuadrilatero(Lista *aux, Quadrilatero *q, Ponto click){
    aux->objeto->setSelect(true);
   // aux->objeto->setXClick(-q->getA().getX()+ click.getX());
   // aux->objeto->setYClick(-q->getA().getY()+click.getY() );
    aux->objeto->setXClick(click.getX());
    aux->objeto->setYClick(click.getY() );

    if (op== COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Quadrilatero *novo = new Quadrilatero(q->getA(), q->getB(), q->getC(), q->getD(), fill, line, espessuraLinha, Objeto::QUADRILATERO);
        insere(novo);
        aux->objeto->setSelect(false);
        fim->objeto->setSelect(true);
        fim->objeto->setXClick(click.getX());
        fim->objeto->setYClick(click.getY() );
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
                 pol->setSelect(true);
                 pol->remove(linha);
                 linha = NULL;
             } else linha = linha->getNext();
         }
    }

}

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
                    glPointSize(8);
                    glBegin(GL_POINTS);
                        glColor3f( 0,0.5 , 0 );
                        glVertex2i(c->getXc(), c->getYc());
                        glVertex2i(c->getXc()+c->getRaio(), c->getYc());
                      glEnd();
                } else if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                    Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                    glPointSize(8);
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
                    glPointSize(8);
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
        cout<<"Esquerdo"<<endl;
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
                } else if (forma == Objeto::POLILINHA){
                    Ponto click(event->x(),gfWrldSizeY-event->y());
                    GLfloat color[4] = {1, 0, 0, 0};
                    Polilinha *pol = new Polilinha(color, color, espessuraLinha, Objeto::POLILINHA);
                    insere(pol);
                    pol->insert(click, click,NULL);

                }
            } else { // se o mouse foi clicado anteriormente, e estivermos desenhando uma polinha ...
                 if (forma == Objeto::POLILINHA){
                     Ponto click(event->x(),gfWrldSizeY-event->y());
                     onMouseClick = false;
                    Polilinha *pol = dynamic_cast <Polilinha *>(fim->objeto);
                    pol->insert(pol->getFim()->getP1(), click, pol->getFim());

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
                                aux->objeto->setSelect(false);
                            }
                            if (c->isSelect()==false && c->isPreenchido()){
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
                             GLint x = e->getCentro().getX() - click.getX(), y = e->getCentro().getY()- click.getY();

                             cout<<"a: "<<a<<", b: "<<b<<endl;
                             cout<<"X: "<<x<<", Y: "<<y<<endl;
                             GLint fxy = b*b*x*x + a*a*y*y - a*a*b*b;
                             //GLint fxy = (y/b)*(y/b) + (x/a)*(x/a) -1 ;
                             if (clippingAreaVertice(e->getControl(), click)){  // clicou sobre o ponto de controle ?
                                 aux->objeto->setSelect(true);
                                 aux->objeto->setXClick(e->getControl().getX()-e->getCentro().getX());
                                 aux->objeto->setYClick(e->getControl().getY()-e->getCentro().getY());
                                 e->getPControl()->setSelect(true);
                             } else{
                                 cout<<fxy<<endl;
                                 if(fxy==0){ // clicou sobre os limites da elispe?
                                     aux->objeto->setSelect(true);
                                     aux->objeto->setXClick(e->getControl().getX()-e->getCentro().getX());
                                     aux->objeto->setYClick(e->getControl().getY()-e->getCentro().getY());
                                 } else {
                                     aux->objeto->setSelect(false);
                                 }
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
                             } else { // testa se o click foi na linha do quadrado (não preenchido)
                                 Quadrilatero *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                                 if (cohenClipping(q->getA(), q->getB(),clik->getC(), clik->getB()) || cohenClipping(q->getB(), q->getD(),clik->getC(), clik->getB()) || cohenClipping(q->getD(), q->getC(),clik->getC(), clik->getB()) || cohenClipping(q->getC(), q->getA(),clik->getC(), clik->getB())){
                                     selecionaQuadrilatero(aux, q, Ponto(event->x(),gfWrldSizeY-event->y()));
                                 } else {
                                    aux->objeto->setSelect(false);
                                 }
                             }
                             if (q->isSelect()== false && q->isPreenchido()){// se o quadrilátero estiver preenchido
                                 if (event->x()<=q->getD().getX() && event->x()>=q->getA().getX() && (gfWrldSizeY-event->y())<=q->getA().getY() && (gfWrldSizeY-event->y())>=q->getD().getY()){
                                        selecionaQuadrilatero(aux, q, click);
                                 }
                            }
                        } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                            Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                            Quadrilatero *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                            Linha *linha = pol->getInit();
                            pol->setSelect(false);
                            while (linha!=NULL && pol->isSelect()==false){
                                if (cohenClipping(linha->getP0(), linha->getP1(),clik->getC(), clik->getB())){
                                    pol->setSelect(true);
                                    pol->setXClick(event->x());
                                    pol->setYClick(gfWrldSizeY-event->y());
                                    if (op==ESCALA){
                                        clickCanvas.setX(event->x());
                                        clickCanvas.setY(gfWrldSizeY-event->y());
                                    }
                                    pol->setLinhaSelecionada1(linha);
                                    pol->setLinhaSelecionada2(NULL);
                                    if (clippingAreaVertice(linha->getP0(), Ponto(event->x(),gfWrldSizeY-event->y()))){
                                        linha->getPP0()->setSelect(true);
                                        cout<<"Seelcionado: P0 - Linha 1"<<endl;
                                    } else {
                                        if (clippingAreaVertice(linha->getP1(), Ponto(event->x(),gfWrldSizeY-event->y()))){
                                            linha->getPP1()->setSelect(true);
                                            pol->setLinhaSelecionada2(linha->getNext());
                                            cout<<"Seelcionado: P1 - Linha"<<endl;
                                            if (linha!=pol->getFim()){
                                                linha->getNext()->getPP0()->setSelect(true);

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
                                    aux->objeto->setSelect(false);
                                    fim->objeto->setSelect(true);
                                    fim->objeto->setXClick(aux->objeto->getXClick());
                                    fim->objeto->setYClick(aux->objeto->getYClick());
                                } else if (op==INSERT_REMOVE_PONTO){ //insere um novo vértice;
                                    Ponto newP1(pol->getLinhaSelecionada1()->getP1().getX(), pol->getLinhaSelecionada1()->getP1().getY());
                                    Ponto newP0(event->x(), gfWrldSizeY-event->y());
                                    pol->insert(newP0, newP1, pol->getLinhaSelecionada1());
                                    pol->getLinhaSelecionada1()->setP1(newP0);
                                    pol->getLinhaSelecionada1()->getPP1()->setSelect(true);
                                    pol->setLinhaSelecionada2(pol->getLinhaSelecionada1()->getNext());
                                    pol->getLinhaSelecionada2()->getPP0()->setSelect(true);

                                }
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
                clickCanvas.setX(event->x());
                clickCanvas.setY(gfWrldSizeY-event->y());
                opBotaoDireito = true;
                updateGL();
            } else if (op==INSERT_REMOVE_PONTO){ // removemos o ponto selecionado (apenas para polilinha)
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
            } else if (fim->objeto->getTipo()== Objeto::POLILINHA){
                Polilinha *pol = dynamic_cast <Polilinha *>(fim->objeto);
                Ponto click(event->x(),gfWrldSizeY-event->y());
                pol->setFim(pol->getFim()->getP0(), click);

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
                        GLint dy = gfWrldSizeY-event->y() - q->getYClick();
                        GLint dx = event->x() - q->getXClick();
                        q->setA(Ponto(q->getA().getX()+dx, q->getA().getY()+dy));
                        q->setB(Ponto(q->getB().getX()+dx, q->getB().getY()+dy));
                        q->setC(Ponto(q->getC().getX()+dx, q->getC().getY()+dy));
                        q->setD(Ponto(q->getD().getX()+dx, q->getD().getY()+dy));
                        q->setXClick(event->x());
                        q->setYClick(gfWrldSizeY-event->y());

                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);
                        e->setCentro(Ponto(event->x()-e->getXClick(), gfWrldSizeY-event->y()- e->getYClick()));

                        e->setControl(Ponto(e->getCentro().getX()+ax*(e->getRaioHorizontal()),e->getCentro().getY()+ay*(e->getRaioVertical())));
                    } else if (aux->objeto->getTipo() == Objeto::POLILINHA){

                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        GLint dy = gfWrldSizeY-event->y() - pol->getYClick();
                        GLint dx = event->x() - pol->getXClick();
                        Linha *linha = pol->getInit();

                        while (linha!=NULL){
                            linha->setP0(Ponto(linha->getP0().getX()+dx, linha->getP0().getY()+dy));
                            linha->setP1(Ponto(linha->getP1().getX()+dx, linha->getP1().getY()+dy));
                            linha = linha->getNext();
                        }
                        pol->setXClick(event->x());
                        pol->setYClick(gfWrldSizeY-event->y());
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
                        GLdouble fatorx = 2, fatory = 2;//abs(distanciaAB-(xclick-centro.getX())), fatory = abs(distanciaAB-(yclick -centro.getY()));
                        if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                            q->escala(-1*fatorx, -1*fatory);
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
                    } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        /* centro do canvas = (gfWrldSizeX/2, gfWrldSizeY/2)
                        */
                        GLint dy = abs((gfWrldSizeY-event->y()-gfWrldSizeY/2)) - abs((clickCanvas.getY()-gfWrldSizeY/2));
                        GLint dx = abs((event->x() - gfWrldSizeX/2))-abs((clickCanvas.getX()- gfWrldSizeX/2));
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
                            linha->getPP0()->setX(linha->getP0().getX()); // desloca para origem
                            linha->getPP0()->setY(linha->getP0().getY()); // desloca para origem
                            linha->getPP1()->setX(linha->getP1().getX()); // desloca para origem
                            linha->getPP1()->setY(linha->getP1().getY()); // desloca para origem

                            linha->getPP0()->setX(linha->getP0().getX()*auxx);
                            linha->getPP0()->setY(linha->getP0().getY()*auxy);
                            linha->getPP1()->setX(linha->getP1().getX()*auxx);
                            linha->getPP1()->setY(linha->getP1().getY()*auxy);
                            linha = linha->getNext();
                        }
                        clickCanvas.setX(event->x());
                        clickCanvas.setY(gfWrldSizeY-event->y());
                    }
                } else if (op==DESLOCARPONTOS){
                    if (aux->objeto->getTipo() == Objeto::QUADRILATERO){
                        Quadrilatero *q = dynamic_cast <Quadrilatero *>(aux->objeto);
                        if (q->getA().isSelect()){
                            cout<<"<<A>>"<<endl;
                            q->setA(Ponto(event->x(), gfWrldSizeY-event->y())); //
                            q->setB(Ponto(q->getA().getX(), q->getB().getY()));
                            q->setC(Ponto(q->getC().getX(),q->getA().getY()));
                        } else if (q->getB().isSelect()){
                            cout<<"<<B>>"<<endl;
                            q->setB(Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->setA(Ponto(q->getB().getX(), q->getA().getY()));
                            q->setD(Ponto(q->getD().getX(), q->getB().getY()));
                        } else if (q->getC().isSelect()){
                            cout<<"<<C>>"<<endl;
                            q->setC(Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->setA(Ponto(q->getA().getX(), q->getC().getY()));
                            q->setD(Ponto(q->getC().getX(), q->getD().getY()));
                        } else if (q->getD().isSelect()){
                            cout<<"<<D>>"<<endl;
                            q->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
                            q->setB(Ponto(q->getB().getX(), q->getD().getY()));
                            q->setC(Ponto(q->getD().getX(), q->getC().getY()));
                        }
                    } else if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        cout<<e->getControl().isSelect()<<endl;
                        if (e->getControl().isSelect()){ // apenas por ponto de controle
                            e->setRaioHorizontal(abs(event->x()-e->getCentro().getX()));
                            e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().getY()));
                            e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
                        }
                    } else if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        deslocaPontoPolilinha(pol, event->x(), gfWrldSizeY-event->y());
                    }
                } else if (op==INSERT_REMOVE_PONTO){
                    if (aux->objeto->getTipo() == Objeto::POLILINHA){
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux->objeto);
                        deslocaPontoPolilinha(pol, event->x(), gfWrldSizeY-event->y());
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
        forma = Objeto::QUADRILATERO;
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
