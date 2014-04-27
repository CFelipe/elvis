#include "glwidget.h"
#include "objeto.h"
#include "vertice.h"
#include "retangulo.h"
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

Ponto clickCanvas;
GLdouble distanciaAB = 0; // guarda a distancia entre dois Vertices quaisquer A e B
GLdouble anguloDeRotacao = M_PI/2; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)

/* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
 * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
 * opBotaoDireito=fase <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um Vertice de controle
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
    /* 1 Desloca os Vertices para a origem
     * 2 Rotaciona
     * 3 Devolve as cordenadas iniciais
     */
    if (ob->isSelect()){
        switch(ob->getTipo()){
            case Objeto::RETANGULO:
                Retangulo *q = dynamic_cast <Retangulo *>(ob);
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
                Circulo *c = dynamic_cast <Circulo *>(ob); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
                GLint xc = c->getXc()-clickCanvas.getX();
                GLint yc = c->getYc()-clickCanvas.getY();
                c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.getX());
                c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.getY());
                break;
            case Objeto::ELIPSE:
            default:
                cout<<"DEFAUT"<<endl;
                break;

        }
    }
}

void selecionaRetangulo(Lista *aux, Retangulo *q, Vertice click){
    aux->objeto->setSelect(true);
    aux->objeto->setXClick(-q->getA().getX()+ click.getX());
    aux->objeto->setYClick(-q->getA().getY()+click.getY() );
    if (op== COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Retangulo *novo = new Retangulo(q->getA(), q->getB(), q->getC(), q->getD(), fill, line, espessuraLinha, Objeto::Retangulo);
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
Retangulo* GLWidget::getAreaClippingMouse(GLint xmouse, GLint ymouse){
    Vertice A(xmouse-CONTROL, ymouse+CONTROL);
    Vertice D(xmouse+CONTROL, ymouse-CONTROL);
    Vertice B(xmouse-CONTROL, ymouse-CONTROL); // (xmin, ymin)
    Vertice C(xmouse+CONTROL, ymouse+CONTROL); // (xmax, ymax)GLfloat colorFill[4] = {0, 0, 0, 0};
    GLfloat colorLine[4] = {0, 0, 0, 0};
    GLfloat colorFill[4] = {0, 0, 0, 0};
    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, espessuraLinha, Objeto::Retangulo);
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
                } else if (aux->objeto->getTipo() == Objeto::Retangulo){
                    Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
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
                } else if (aux->objeto->getTipo() == Objeto::Retangulo){
                    Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto); // EIS O PROBLEMA COM O QUAL PASSAMOS UMA TARDE QUEBRANDO A CABEÇA!!!!!!!!!!!!!!
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
                } else if (forma == Objeto::Retangulo) {
                    Vertice A(event->x(), gfWrldSizeY-event->y()); // Vertice de clik
                    Vertice D(event->x(), gfWrldSizeY-event->y());
                    Vertice B(event->x(), gfWrldSizeY-event->y());
                    Vertice C(event->x(), gfWrldSizeY-event->y());
                    GLfloat colorFill[4] = {1, 0, 0, 0};
                    GLfloat colorLine[4] = {1, 0, 0, 0};
                    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, espessuraLinha, Objeto::Retangulo);
                    insere(q);
                } else if (forma == Objeto::ELIPSE){
                    GLint xc =event->x();
                    GLint yc = gfWrldSizeY-event->y();
                    GLfloat color[4] = {1, 0, 0, 0};
                    Elipse *e = new Elipse(Vertice(xc, yc), 0, 0, color, color, espessuraLinha, Objeto::ELIPSE);
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
                                 * através do Vertice de controle ou clicando sobre seus limites
                                 * que formam o desenho (e não as linhas potilhadas)
                             */
                             Vertice click(event->x(),gfWrldSizeY-event->y());
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

                        }else if (aux->objeto->getTipo() == Objeto::Retangulo){
                             Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                             Vertice click(event->x(),gfWrldSizeY-event->y());
                             if (clippingAreaVertice(q->getA(), click) || clippingAreaVertice(q->getB(), click) || clippingAreaVertice(q->getC(), click) || clippingAreaVertice(q->getD(), click)){
                                 selecionaRetangulo(aux, q, click);
                                 if(clippingAreaVertice(q->getA(), click)){
                                     q->getPA()->setSelect(true);
                                 } else if(clippingAreaVertice(q->getB(), click) ){
                                     q->getPB()->setSelect(true);
                                 } else if (clippingAreaVertice(q->getC(), click)){
                                     q->getPC()->setSelect(true);
                                 } else q->getPD()->setSelect(true);
                             } else if (q->isPreenchido()){// se o quadrilátero estiver preenchido
                                 if (event->x()<=q->getD().getX() && event->x()>=q->getA().getX() && (gfWrldSizeY-event->y())<=q->getA().getY() && (gfWrldSizeY-event->y())>=q->getD().getY()){
                                        selecionaRetangulo(aux, q, click);
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
                                clickCanvas.setY(gfWrldSizeY - event->y());
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
                        // o código abaixo "deseleciona" os Vertices de controle da forma, caso a operação realizada tenha sido um deslocamento de Vertices
                        if (aux->objeto->getTipo() == Objeto::Retangulo){
                            Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
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
            } else if (fim->objeto->getTipo() == Objeto::Retangulo){
                Retangulo *q = dynamic_cast <Retangulo *>(fim->objeto);
                q->setD(Vertice(event->x(), gfWrldSizeY-event->y()));
                q->setC(Vertice(q->getD().getX(), q->getA().getY()));
                q->setB(Vertice(q->getA().getX(), q->getD().getY()));
            } else if (fim->objeto->getTipo() == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(fim->objeto);
                e->setRaioHorizontal(abs(event->x()-e->getCentro().getX()));
                e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().getY()));
                e->setControl(Vertice(event->x(), gfWrldSizeY-event->y()));
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
                    } else if (aux->objeto->getTipo() == Objeto::Retangulo){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        GLint dx = + q->getD().getX() - q->getA().getX();
                        GLint dy = - q->getA().getY() + q->getD().getY();
                        Vertice A(event->x()-aux->objeto->getXClick(), gfWrldSizeY-event->y()-aux->objeto->getYClick()); // Vertice de clik
                        Vertice D(A.getX()+dx, A.getY()+dy);
                        Vertice C(D.getX(), A.getY());
                        Vertice B(A.getX(), D.getY());
                        q->setA(A);
                        q->setB(B);
                        q->setC(C);
                        q->setD(D);
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);
                        e->setCentro(Vertice(event->x()-e->getXClick(), gfWrldSizeY-event->y()- e->getYClick()));

                        e->setControl(Vertice(e->getCentro().getX()+ax*(e->getRaioHorizontal()),e->getCentro().getY()+ay*(e->getRaioVertical())));

                    }
                } else if (op==ESCALA){
                    if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux->objeto);
                        c->redimensionar(event->x(), gfWrldSizeY-event->y());
                    } else if (aux->objeto->getTipo() == Objeto::Retangulo){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        Vertice centro = q->getCentro();
                        GLdouble aux = distanciaAB;
                        distanciaAB = sqrt(pow(event->x()-centro.getX(), 2)+pow(gfWrldSizeY-event->y()-centro.getY(), 2));
                        GLint fatorx = 3, fatory = 3;//abs(distanciaAB-(xclick-centro.getX())), fatory = abs(distanciaAB-(yclick -centro.getY()));
                        if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                            q->escala((-1)*fatorx, (-1)*fatory);
                        } else{ // "saindo"
                            q->escala(fatorx, fatory);
                        }
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){  // de qualquer Vertice selecionado
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        GLint rh = abs(event->x()-e->getCentro().getX())+1;
                        GLint rv = abs(gfWrldSizeY-event->y()-e->getCentro().getY())+1;
                        GLint ax = e->getControl().getX()==e->getCentro().getX()+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().getY()==e->getCentro().getY()+e->getRaioVertical()?(1):(-1);

                        e->setControl(Vertice(e->getCentro().getX()+ax*(rh),e->getCentro().getY()+ay*(rv)));
                        e->setRaioHorizontal(rh);
                        e->setRaioVertical(rv);
                        //cout<<rh<<","<<rv<<endl;
                    }
                } else if (op==DESLOCARVerticeS){
                    if (aux->objeto->getTipo() == Objeto::Retangulo){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux->objeto);
                        if (q->getA().isSelect()){
                            q->setA(Vertice(event->x(), gfWrldSizeY-event->y())); //
                            q->setB(Vertice(q->getA().getX(), q->getB().getY()));
                            q->setC(Vertice(q->getC().getX(),q->getA().getY()));
                        } else if (q->getB().isSelect()){
                            q->setB(Vertice(event->x(), gfWrldSizeY-event->y()));
                            q->setA(Vertice(q->getB().getX(), q->getA().getY()));
                            q->setD(Vertice(q->getD().getX(), q->getB().getY()));
                        } else if (q->getC().isSelect()){
                            q->setC(Vertice(event->x(), gfWrldSizeY-event->y()));
                            q->setA(Vertice(q->getA().getX(), q->getC().getY()));
                            q->setD(Vertice(q->getC().getX(), q->getD().getY()));
                        } else if (q->getD().isSelect()){
                            q->setD(Vertice(event->x(), gfWrldSizeY-event->y()));
                            q->setB(Vertice(q->getB().getX(), q->getD().getY()));
                            q->setC(Vertice(q->getD().getX(), q->getC().getY()));
                        }
                    } else if (aux->objeto->getTipo() == Objeto::CIRCULO){
                        // fazer primeiro a elipse;
                    } else if (aux->objeto->getTipo() == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux->objeto);
                        cout<<e->getControl().isSelect()<<endl;
                        if (e->getControl().isSelect()){ // apenas por Vertice de controle
                            e->setRaioHorizontal(abs(event->x()-e->getCentro().getX()));
                            e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().getY()));
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
        forma = Objeto::Retangulo;
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
        op = DESLOCARVerticeS;
        desenha = false;
    } else if(q->text() == "Rotação") {
        op = ROTACAO;
        desenha = false;
    }
}
