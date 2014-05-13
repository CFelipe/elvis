#include "glwidget.h"
#include "window.h"
#include "objeto.h"
#include "elipse.h"
#include "circulo.h"
#include "retangulo.h"
#include "ponto.h"
#include "polilinha.h"
#include "vertice.h"

#include <cmath>

#define CONTROL 5

GLfloat gfWrldSizeX = 800.f;
GLfloat gfWrldSizeY = 600.f;
GLfloat gfWrldSizeZ = 400.f;

Ponto clickCanvas;
Ponto *click1, click2;
GLdouble distanciaAB = 0; // guarda a distancia entre dois pontos quaisquer A e B
GLdouble anguloDeRotacao = M_PI/2; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)

/* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
 * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
 * opBotaoDireito=false <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um ponto de controle
*/
bool opBotaoDireito = false;
bool onMouseClick = false;
bool preAgrupamento = false;
bool voltaSelecao = false;
bool pressing = false;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);

    opBotaoDireito = false;
    onMouseClick = false;

    setFocusPolicy(Qt::StrongFocus);
}

GLint GLWidget::snap(GLint mouse) {
    if(Window::docAtual->grade) {
        if((mouse % Window::docAtual->gradeSep) < (Window::docAtual->gradeSep / 2)) {
            return (mouse / Window::docAtual->gradeSep) * Window::docAtual->gradeSep;
        } else {
            return ((mouse / Window::docAtual->gradeSep) * Window::docAtual->gradeSep) + Window::docAtual->gradeSep;
        }
    } else {
        return mouse;
    }
}

void GLWidget::desenhaSelecao() {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glColor3f(0.55f, 0.55f, 1.f);
    glBegin(GL_LINE_LOOP);
         glVertex2i(Objeto::intXView(Window::docAtual->selecaoMin.x), Objeto::intYView(Window::docAtual->selecaoMin.y));
         glVertex2i(Objeto::intXView(Window::docAtual->selecaoMin.x), Objeto::intYView(Window::docAtual->selecaoMax.y));
         glVertex2i(Objeto::intXView(Window::docAtual->selecaoMax.x), Objeto::intYView(Window::docAtual->selecaoMax.y));
         glVertex2i(Objeto::intXView(Window::docAtual->selecaoMax.x), Objeto::intYView(Window::docAtual->selecaoMin.y));
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void GLWidget::desenhaGrade(GLint sep) {
    GLint x, y;
    glLineWidth(1);
    glColor3f( .75f, .75f, .75f );

    // Linhas verticais
    for(x = 0; x <= Window::docAtual->canvasW; x += sep) {
        glBegin( GL_LINES );
            glVertex2i(Objeto::intXView(x), Objeto::intYView(0));
            glVertex2i(Objeto::intXView(x), Objeto::intYView(Window::docAtual->canvasH));
        glEnd();
    }

    // Linhas horizontais
    for(y = 0; y <= Window::docAtual->canvasH; y += sep) {
        glBegin( GL_LINES );
            glVertex2i(Objeto::intXView(0), Objeto::intYView(y));
            glVertex2i(Objeto::intXView(Window::docAtual->canvasW), Objeto::intYView(y));
        glEnd();
    }
}

void GLWidget::desenhaCanvas() {
    glLineWidth(2);
    glColor3f(0.f, 0.f, 0.f);

    glBegin(GL_LINE_LOOP);
        glVertex2i(Objeto::intXView(0), Objeto::intYView(0));
        glVertex2i(Objeto::intXView(Window::docAtual->canvasW), Objeto::intYView(0));
        glVertex2i(Objeto::intXView(Window::docAtual->canvasW), Objeto::intYView(Window::docAtual->canvasH));
        glVertex2i(Objeto::intXView(0), Objeto::intYView(Window::docAtual->canvasH));
    glEnd( );
}

void GLWidget::initializeGL() {}

// Passar pra objetos
void rotacionaObjeto(Objeto *ob){
    /* 1 Desloca os pontos para a origem
     * 2 Rotaciona
     * 3 Devolve as cordenadas iniciais
     */
    if (ob->selecionado){
        switch(ob->tipo){
        case RETANGULO :
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
            q->setA(A);
            q->setB(B);
            q->setC(C);
            q->setD(D);}
            break;
        case CIRCULO:
        {
            Circulo *c = dynamic_cast <Circulo *>(ob);
            GLint xc = c->getXc()-clickCanvas.x;
            GLint yc = c->getYc()-clickCanvas.y;
            c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.x);
            c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.y);
        }
            break;
        case ELIPSE:
        {
            Elipse *e = dynamic_cast <Elipse *>(ob);
            GLint xc = e->getCentro().p.x-clickCanvas.x;
            GLint yc = e->getCentro().p.y-clickCanvas.y;
            GLint ax = e->getControl().p.x == e->getCentro().p.x+e->getRaioHorizontal() ? (1):(-1);
            GLint ay = e->getControl().p.y == e->getCentro().p.y+e->getRaioVertical() ? (1):(-1);
            Ponto cen(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.x, xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.y );
            GLint aux = e->getRaioHorizontal();
            e->setRaioHorizontal(e->getRaioVertical());
            e->setRaioVertical(aux);
            e->setCentro(cen);
            e->setControl(Ponto(e->getCentro().p.x+ax*(e->getRaioHorizontal()),e->getCentro().p.y+ay*(e->getRaioVertical())));
        }
            break;
        case POLILINHA:
        {
            Polilinha *pol = dynamic_cast <Polilinha *>(ob);
            Vertice* v;
            GLint aux;
            int i;
            for(i = 0; i < pol->vertices.size(); i++) {
                v = pol->vertices.at(i);

                v->p.x -= clickCanvas.x;
                v->p.y -= clickCanvas.y;

                aux = v->p.x;

                v->p.x = (aux * cos(anguloDeRotacao) - v->p.y * sin(anguloDeRotacao) + clickCanvas.x);
                v->p.y = (aux * sin(anguloDeRotacao) - v->p.y * cos(anguloDeRotacao) + clickCanvas.y);
            }

            break;
        }
            default:
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

void GLWidget::desselecionaALL(){
    onMouseClick = false;

    Objeto* aux;
    int i, j;
    for(i = 0; i < Window::docAtual->camadas.size(); i++) {
        for(j = 0; j < Window::docAtual->camadas.at(i)->objetos->size(); j++) {
            aux = Window::docAtual->camadas.at(i)->objetos->at(j);
            aux->selecionado = (false);
            if (aux->tipo == RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(aux);
                q->A.selecionado = false;
                q->B.selecionado = false;
                q->C.selecionado = false;
                q->D.selecionado = false;
            } else if (aux->tipo == ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(aux);
                e->getPControl()->selecionado = (false);
            } else if (aux->tipo == POLILINHA){
                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                pol->desseleciona();
            }
        }
    }
}

void GLWidget::selecionaCirculo(Objeto *aux, Circulo *c, Ponto click){
    aux->selecionado = (true);
    aux->setXClick(click.x - c->getXc());
    aux->setYClick(click.y - c->getYc());
    if (Window::docAtual->op==COPIA){
        GLfloat fill[4], line[4];
        c->getColorFill(fill);
        c->getColorLine(line);
        Circulo* novo = new Circulo(c);

        aux->selecionado = (false);

        Window::docAtual->camadaSelecionada->objetos->append(novo);
        Window::docAtual->camadaSelecionada->objetos->last()->selecionado = true;
        Window::docAtual->camadaSelecionada->objetos->last()->setXClick(click.x -c->getXc() );
        Window::docAtual->camadaSelecionada->objetos->last()->setYClick(click.y- c->getYc());
    }
}

void GLWidget::selecionaQuadrilatero(Objeto *aux, Retangulo *q, Ponto click){
    aux->selecionado = (true);
    aux->setXClick(click.x);
    aux->setYClick(click.y );

    if (Window::docAtual->op == COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Retangulo *novo = new Retangulo(q);

        Window::docAtual->camadaSelecionada->objetos->append(novo);

        aux->selecionado = (false);
        Window::docAtual->camadaSelecionada->objetos->last()->selecionado = true;
        Window::docAtual->camadaSelecionada->objetos->last()->setXClick(click.x);
        Window::docAtual->camadaSelecionada->objetos->last()->setYClick(click.y);
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
    Retangulo *q = new Retangulo(A, B, C, D);
    return q;
}

//!
//! \brief clippingAreaVertice diz se um ponto está dentro ou fora de um região quadriculada em torno de um vértice
//! \param vertice é o vertice entorno do qual teremos a região quadrada
//! \param teste é o ponto de teste
//! \return true se o ponto está dentro
//!
bool clippingAreaVertice(Ponto vertice, Ponto teste){
    return teste.x <= (vertice.x+CONTROL) &&
           teste.y <= (vertice.y+CONTROL) &&
           teste.x >= (vertice.x-CONTROL) &&
           teste.y >= (vertice.y-CONTROL);
}

void GLWidget::resizeGL(int w, int h) {
        float viewDepth = gfWrldSizeZ/2.f;

        // Usar toda a janela.
        glViewport(0, 0, w, h);

        h = (h==0) ? 1 : h;
        w = (w==0) ? 1 : w;
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        glOrtho( 0, w,
                 0, h,
                -viewDepth, viewDepth );

        gfWrldSizeX = w;
        gfWrldSizeY = h;
        glMatrixMode( GL_MODELVIEW );

        glLoadIdentity( );
}

void GLWidget::paintGL() {
    glClearColor( 0.9f, 0.9f, 0.9f, 0.f );
    //glClearColor( 1.f, 1.f, 1.f, 1.f);
    glClear( GL_COLOR_BUFFER_BIT );

    glColor3f( 1.f, 0.f, 0.f );

    if(Window::docAtual->grade) {
        desenhaGrade(Window::docAtual->gradeSep);
    }

    desenhaCanvas();

    if (Window::docAtual->op==ROTACAO && opBotaoDireito){ // se a opção se rotação foi selecionada e o botão direito foi pressionado, então desenhe uma cruzinha
        glPointSize(10);
        glColor3f( 0,0 , 0 );
        glPointSize(3);
        glBegin(GL_LINES);
            glVertex2i(clickCanvas.x+CONTROL, clickCanvas.y);
            glVertex2i(clickCanvas.x-CONTROL, clickCanvas.y);
            glVertex2i(clickCanvas.x, clickCanvas.y+CONTROL);
            glVertex2i(clickCanvas.x, clickCanvas.y-CONTROL);
        glEnd();
    }

    Objeto* aux;
    int i, j;
    for(i = 0; i < Window::docAtual->camadas.size(); i++) {
        for(j = 0; j < Window::docAtual->camadas.at(i)->objetos->size(); j++) {
            aux = Window::docAtual->camadas.at(i)->objetos->at(j);
            aux->desenha();
        }
    }

    if (Window::docAtual->op==SELECIONAR && Window::docAtual->selecaoMultipla) {
        desenhaSelecao();
    }

    glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    qDebug() << "op: " << Window::docAtual->op;
    qDebug() << "forma: " << Window::docAtual->forma;
    qDebug() << "onmouseclick: " << onMouseClick;
    qDebug() << "desenha: " << Window::docAtual->desenha;
    qDebug() << "preagrupamento: " << preAgrupamento;
    qDebug() << "voltaselecao: " << voltaSelecao;
    qDebug() << "------------";

    GLint mouseX = event->x();
    GLint mouseY = gfWrldSizeY - event->y();
    GLint mouseXR = event->x() - Window::docAtual->viewport.x;
    GLint mouseYR = gfWrldSizeY - event->y() - Window::docAtual->viewport.y;

    clickCanvas = Ponto(mouseX, mouseY);
    pressing = true;

    if(Window::docAtual->op == PAN) {
        setCursor(Qt::ClosedHandCursor);
    }

    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton) { // se o botão esquerdo for clicado, cria a forma
        if (Window::docAtual->desenha) {
            if (onMouseClick==false){ // se o mouse ainda não foi clicado
                if (Window::docAtual->forma == CIRCULO){
                    Circulo *c = new Circulo(0, snap(mouseXR), snap(mouseYR));
                    Window::docAtual->camadaSelecionada->objetos->append(c);
                } else if (Window::docAtual->forma == RETANGULO) {
                    Ponto M(snap(mouseXR), snap(mouseYR));
                    Retangulo *q = new Retangulo(M, M, M, M);
                    Window::docAtual->camadaSelecionada->objetos->append(q);
                } else if (Window::docAtual->forma == ELIPSE){
                    Elipse *e = new Elipse(Ponto(snap(mouseXR), snap(mouseYR)), 0, 0);
                    Window::docAtual->camadaSelecionada->objetos->append(e);
                } else if (Window::docAtual->forma == POLILINHA){
                    Polilinha *p = new Polilinha(Ponto(snap(mouseXR), snap(mouseYR)));
                    Window::docAtual->camadaSelecionada->objetos->append(p);
                }
            } else { // se o mouse foi clicado anteriormente e estivermos desenhando uma polinha ...
                if (Window::docAtual->forma == POLILINHA){
                    onMouseClick = false;
                    Polilinha *pol = dynamic_cast <Polilinha *>(Window::docAtual->camadaSelecionada->objetos->last());
                    pol->inserirVertice(mouseXR, mouseYR);
                }
            }

            onMouseClick = !onMouseClick;
        } else if (Window::docAtual->op==SELECIONAR) {
            Window::docAtual->selecaoMultipla = true;
            Window::docAtual->selecaoMin = Ponto(mouseXR, mouseYR);
            Window::docAtual->selecaoMax = Ponto(mouseXR, mouseYR);
            onMouseClick = true;

            if (preAgrupamento==true){
                qDebug() << "caso 25";
                Ponto click(mouseXR, mouseYR);

                if (clippingAreaVertice(Window::docAtual->selecaoMin, click) ||
                    clippingAreaVertice(Window::docAtual->selecaoMax, click)) {
                    qDebug() << "caso 20";
                    voltaSelecao = true;
                    Window::docAtual->op = TRANSLACAO;
                    qDebug() << "caso1";
                } else {
                    Retangulo *clik = getAreaClippingMouse(mouseXR, mouseYR);
                    if (cohenClipping(Window::docAtual->selecaoMin, Window::docAtual->selecaoMax, clik->C.p, clik->B.p)) {
                        voltaSelecao = true;
                        Window::docAtual->op = TRANSLACAO;
                        qDebug() << "caso2";
                    } else if (mouseXR <= Window::docAtual->selecaoMax.x &&
                               mouseXR >= Window::docAtual->selecaoMin.x &&
                               mouseYR <= Window::docAtual->selecaoMax.y &&
                               mouseYR >= Window::docAtual->selecaoMin.y) {
                        voltaSelecao = true;
                        Window::docAtual->op = TRANSLACAO;
                        qDebug() << "caso3";
                    } else {
                        preAgrupamento = false;
                        desselecionaALL();
                    }
                }
                if (preAgrupamento == true) {
                    qDebug() << "caso4";
                    Objeto* aux;
                    int i, j;
                    for(i = 0; i < Window::docAtual->camadas.size(); i++) {
                        GLint sizeJ = Window::docAtual->camadas.at(i)->objetos->size();
                        for(j = 0; j < sizeJ; j++) {
                            aux = Window::docAtual->camadas.at(i)->objetos->at(j);
                            if (aux->selecionado){
                                if (aux->tipo==CIRCULO){
                                    Circulo *c = dynamic_cast <Circulo *>(aux);
                                    aux->setXClick(mouseXR -c->getXc());
                                    aux->setYClick(mouseYR - c->getYc());
                                } else if (aux->tipo==ELIPSE){
                                    Elipse *e = dynamic_cast <Elipse *>(aux);
                                    aux->setXClick(mouseXR -e->getCentro().p.x);
                                    aux->setYClick(mouseYR -e->getCentro().p.y);
                                } else{
                                    aux->setXClick(mouseXR);
                                    aux->setYClick(mouseYR);
                                }
                            }
                        }
                    }
                }
            }
            if (preAgrupamento==false){
                bool oneAnySelection = false;
                Objeto* aux;
                int i, j;
                for(i = 0; i < Window::docAtual->camadas.size(); i++) {
                    oneAnySelection = false;
                    GLint sizeJ = Window::docAtual->camadas.at(i)->objetos->size();
                    for(j = 0; j < sizeJ; j++) {
                        aux = Window::docAtual->camadas.at(i)->objetos->at(j);
                        if (aux->tipo == CIRCULO){
                            qDebug() << "caso 20";
                            Circulo *c = dynamic_cast <Circulo *>(aux);
                            GLint distancia = sqrt(pow(mouseXR - c->getXc(), 2) + pow(mouseYR -c->getYc(), 2));
                            if (abs(c->getRaio()-distancia)<=CONTROL){
                                c->selecionado = true;
                                aux->setXClick(mouseXR - c->getXc());
                                aux->setYClick(mouseYR - c->getYc());

                                oneAnySelection = true;
                                voltaSelecao = true;
                                Window::docAtual->op = TRANSLACAO;
                            } else {
                                c->selecionado = (false);
                            }
                            if (c->selecionado == false && c->preenchido){
                                qDebug() << "caso 21";
                                if(distancia<=c->getRaio()){
                                    c->selecionado = true;
                                    oneAnySelection = true;
                                    aux->setXClick(mouseXR - c->getXc());
                                    aux->setYClick(mouseYR - c->getYc());

                                    voltaSelecao = true;
                                    Window::docAtual->op = TRANSLACAO;
                                } else {
                                    c->selecionado = (false);
                                }
                            }
                        } else if(aux->tipo == ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux);
                            Ponto click(mouseXR, mouseYR);
                            if (clippingAreaVertice(e->getControl().p, click)){  // clicou sobre o ponto de controle?
                                aux->selecionado = (true);
                                aux->setXClick(mouseXR -e->getCentro().p.x);
                                aux->setYClick(mouseYR -e->getCentro().p.y);

                                oneAnySelection = true;
                                voltaSelecao = true;
                                Window::docAtual->op=TRANSLACAO;
                            } else {
                                aux->selecionado = (false);
                            }
                        } else if (aux->tipo == RETANGULO){
                            Retangulo *q = dynamic_cast <Retangulo *>(aux);
                            Ponto click(mouseXR,mouseYR);
                            if (clippingAreaVertice(q->A.p, click) || clippingAreaVertice(q->B.p, click) || clippingAreaVertice(q->C.p, click) || clippingAreaVertice(q->D.p, click)){
                                aux->selecionado = (true);
                                aux->setXClick(mouseXR);
                                aux->setYClick(mouseYR);

                                oneAnySelection = true;
                                voltaSelecao = true;
                                Window::docAtual->op=TRANSLACAO;
                            } else { // testa se o click foi na linha do quadrado (não preenchido)
                                Retangulo *clik = getAreaClippingMouse(mouseXR, mouseYR);
                                if (cohenClipping(q->A.p, q->B.p,clik->C.p, clik->B.p) ||
                                    cohenClipping(q->B.p, q->D.p,clik->C.p, clik->B.p) ||
                                    cohenClipping(q->D.p, q->C.p,clik->C.p, clik->B.p) ||
                                    cohenClipping(q->C.p, q->A.p,clik->C.p, clik->B.p)){
                                    aux->selecionado = (true);
                                    aux->setXClick(mouseXR);
                                    aux->setYClick(mouseYR);

                                    oneAnySelection = true;
                                    voltaSelecao = true;
                                    Window::docAtual->op=TRANSLACAO;
                                } else {
                                    aux->selecionado = (false);
                                }
                            }
                            if (q->selecionado == false && q->preenchido){ // se o quadrilátero estiver preenchido
                                qDebug() << "caso 6";
                                if (mouseXR<=q->D.p.x && mouseXR>=q->A.p.x && (mouseYR)<=q->A.p.y && (mouseYR)>=q->D.p.y){
                                    aux->selecionado = (true);
                                    aux->setXClick(mouseXR);
                                    aux->setYClick(mouseYR);
                                    oneAnySelection = true;
                                    voltaSelecao = true;
                                    Window::docAtual->op=TRANSLACAO;
                                } else {
                                    aux->selecionado = false;
                                }
                            }
                        } else if (aux->tipo==POLILINHA){
                            Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                            Retangulo *clik = getAreaClippingMouse(mouseXR, mouseYR);
                            pol->selecionado = (false);

                            int i;
                            for(i = 0; i < pol->vertices.size(); i++) {
                                if(i + 1 < pol->vertices.size()) {
                                    if (cohenClipping(pol->vertices.at(i)->p, pol->vertices.at(i + 1)->p, clik->C.p, clik->B.p)){
                                        pol->selecionado = true;
                                        aux->setXClick(mouseXR);
                                        aux->setYClick(mouseYR);

                                        oneAnySelection = true;
                                        voltaSelecao = true;
                                        Window::docAtual->op=TRANSLACAO;
                                    }
                                }
                            }
                        }
                    }
                }
                if (oneAnySelection==false){
                    Window::docAtual->selecaoMin = Ponto(mouseXR, mouseYR);
                    Window::docAtual->selecaoMax = Ponto(mouseXR, mouseYR);
                    preAgrupamento=false;
                }
            }
            onMouseClick = true;
        } else {
            // Determinação de objeto selecionado baseado em onde o usuário clicou
            Objeto* aux;
            int i, j;
            for(i = 0; i < Window::docAtual->camadas.size(); i++) {
                GLint sizeJ = Window::docAtual->camadas.at(i)->objetos->size();
                for(j = 0; j < sizeJ; j++) {
                    aux = Window::docAtual->camadas.at(i)->objetos->at(j);

                    if (aux->tipo == CIRCULO){
                        qDebug() << "caso 7";
                        Circulo *c = dynamic_cast <Circulo *>(aux);
                        Ponto click = Ponto(mouseXR, mouseYR);
                        GLint distancia =sqrt(pow(mouseXR-c->getXc(), 2)+pow(mouseYR-c->getYc(), 2));
                        if (abs(c->getRaio()-distancia)<=CONTROL){
                            selecionaCirculo(aux, c, click);
                        } else {
                            aux->selecionado = (false);
                        }
                        if (c->selecionado == false && c->preenchido){
                            if(distancia <= c->getRaio()){
                                selecionaCirculo(aux, c, Ponto(mouseXR, mouseYR));
                            }
                        }
                    } else if(aux->tipo == ELIPSE){
                        qDebug() << "caso 8";
                        Elipse *e = dynamic_cast <Elipse *>(aux);
                        /* caso a elipse não esteja preenchicda,
                                            * então a única possibilidade de selecioná-la será
                                            * através do ponto de controle ou clicando sobre seus limites
                                            * que formam o desenho (e não as linhas potilhadas)
                                        */
                        Ponto click(mouseXR,mouseYR);
                        GLint a = e->getRaioHorizontal(), b=e->getRaioVertical();
                        GLint x = e->getCentro().p.x - click.x, y = e->getCentro().p.y- click.y;

                        GLint fxy = b*b*x*x + a*a*y*y - a*a*b*b;
                        //GLint fxy = (y/b)*(y/b) + (x/a)*(x/a) -1 ;
                        if (clippingAreaVertice(e->getControl().p, click)){  // clicou sobre o ponto de controle ?
                            aux->selecionado = (true);
                            aux->setXClick(e->getControl().p.x - e->getCentro().p.x);
                            aux->setYClick(e->getControl().p.y - e->getCentro().p.y);
                            e->getPControl()->selecionado = (true);
                        } else{
                            if(fxy==0){ // clicou sobre os limites da elispe?
                                aux->selecionado = (true);
                                aux->setXClick(e->getControl().p.x-e->getCentro().p.x);
                                aux->setYClick(e->getControl().p.y-e->getCentro().p.y);
                            } else {
                                aux->selecionado = (false);
                            }
                        }
                        if (aux->selecionado){
                            if (Window::docAtual->op==COPIA){
                                GLfloat fill[4], line[4];
                                e->getColorFill(fill);
                                e->getColorLine(line);
                                Elipse *novo = new Elipse(e->getCentro().p, e->getRaioHorizontal(), e->getRaioVertical());
                                novo->setControl(e->getControl());
                                aux->selecionado = (false);
                                Window::docAtual->camadaSelecionada->objetos->append(novo);
                                Window::docAtual->camadaSelecionada->objetos->last()->selecionado = (true);

                                Window::docAtual->camadaSelecionada->objetos->last()->setXClick(aux->getXClick());
                                Window::docAtual->camadaSelecionada->objetos->last()->setYClick(aux->getYClick());
                            }
                        }

                    }else if (aux->tipo == RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux);
                        Ponto click(mouseXR,mouseYR);
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
                            Retangulo *clik = getAreaClippingMouse(mouseXR,mouseYR);
                            if (cohenClipping(q->A.p, q->B.p,clik->C.p, clik->B.p) ||
                                cohenClipping(q->B.p, q->D.p,clik->C.p, clik->B.p) ||
                                cohenClipping(q->D.p, q->C.p,clik->C.p, clik->B.p) ||
                                cohenClipping(q->C.p, q->A.p,clik->C.p, clik->B.p)) {
                                selecionaQuadrilatero(aux, q, Ponto(mouseXR,mouseYR));
                            } else {
                                aux->selecionado = (false);
                            }
                        }
                        if (q->selecionado == false && q->preenchido){// se o quadrilátero estiver preenchido
                            if (mouseXR<=q->D.p.x && mouseXR>=q->A.p.x && (mouseYR)<=q->A.p.y && (mouseYR)>=q->D.p.y){
                                selecionaQuadrilatero(aux, q, click);
                            }
                        }
                    } else if (aux->tipo == POLILINHA){
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                        Retangulo *clik = getAreaClippingMouse(mouseXR, mouseYR);
                        pol->desseleciona();
                        bool pontoSelecionado = false;
                        int i, tamanho;
                        tamanho = pol->vertices.size();
                        for(i = 0; i < tamanho; i++) {
                            if (clippingAreaVertice(pol->vertices.at(i)->p, Ponto(mouseXR, mouseYR))) {
                                if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
                                    pol->removerVertice(i);
                                    pontoSelecionado = true;
                                    break;
                                } else {
                                    pol->selecionado = (true);
                                    pol->vertices.at(i)->selecionado = true;
                                }
                                pontoSelecionado = true;
                            }
                        }

                        if(!pontoSelecionado) {
                            pol->desseleciona();
                            tamanho = pol->vertices.size() - 1;
                            for(i = 0; i < tamanho; i++) {
                                if(cohenClipping(pol->vertices.at(i)->p, pol->vertices.at(i + 1)->p, clik->C.p, clik->B.p)){
                                    pol->selecionado = (true);
                                    pol->setXClick(mouseXR);
                                    pol->setYClick(mouseYR);
                                    if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
                                        pol->inserirVertice(i, mouseXR, mouseYR);
                                        break;
                                    }
                                }
                            }
                        }

                        if (pol->selecionado == false && Window::docAtual->op==ESCALA){
                            qDebug() << "Escalar polilinha";
                        } else if (pol->selecionado == true) {
                            if (Window::docAtual->op==COPIA) {
                                pol->selecionado = (false);
                                Polilinha *novo = new Polilinha(pol);
                                qDebug() << "daqui passou";
                                Window::docAtual->camadaSelecionada->objetos->append(novo);
                                qDebug() << "daqui passou 2";
                                Window::docAtual->camadaSelecionada->objetos->last()->selecionado = true;
                                Window::docAtual->camadaSelecionada->objetos->last()->setXClick(mouseXR);
                                Window::docAtual->camadaSelecionada->objetos->last()->setYClick(mouseYR);
                            }
                        }
                    }

                    if (Window::docAtual->op == ROTACAO){
                        if (opBotaoDireito==false) { // se o usuário não clicou com o botão direito fora da área do quadrado
                            clickCanvas.x = (mouseXR);
                            clickCanvas.y = (mouseYR);
                        }
                        rotacionaObjeto(aux);
                    }
                }
            }
            onMouseClick = true;
        }
    } else if ((event->buttons() & Qt::RightButton) == Qt::RightButton){
        if (Window::docAtual->desenha){
            if (onMouseClick){ // se durante a rasterização da forma o botão direito for pressionado, então delete a forma [QUALQUER]
                onMouseClick = false;
                Window::docAtual->camadaSelecionada->objetos->removeLast();
            }
        } else {
            if (Window::docAtual->op==ROTACAO){
                clickCanvas.x = (mouseXR);
                clickCanvas.y = (mouseYR);
                opBotaoDireito = true;
            } else if (Window::docAtual->op == INSERT_REMOVE_PONTO || Window::docAtual->op == DESLOCARPONTOS) { // removemos o ponto selecionado (apenas para polilinha)
                /*
                    Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                    pol->removerVerticeSelecionado();
                */
            }
        }
    } else if ((event->buttons() & Qt::MidButton) == Qt::MidButton){
        onMouseClick = false;
    }

    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    pressing = false;
    Window::docAtual->selecaoMultipla = false;

    if(Window::docAtual->op == PAN) {
        setCursor(Qt::OpenHandCursor);
    }

    if (Window::docAtual->desenha==false ){
        onMouseClick = false;

        if (voltaSelecao){
            Window::docAtual->op = SELECIONAR;
        }
    }

    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    GLint mouseX = event->x();
    GLint mouseY = gfWrldSizeY - event->y();
    GLint mouseXR = event->x() - Window::docAtual->viewport.x;
    GLint mouseYR = gfWrldSizeY - event->y() - Window::docAtual->viewport.y;

    if(Window::docAtual->op == PAN) {
        if(pressing) {
            qDebug() << Window::docAtual->viewport.x << "|" << Window::docAtual->viewport.y;
            Window::docAtual->viewport.x += mouseX - clickCanvas.x;
            Window::docAtual->viewport.y += mouseY - clickCanvas.y;
            clickCanvas.x = mouseX;
            clickCanvas.y = mouseY;
        }
    }

    if (Window::docAtual->desenha){
        if(onMouseClick){
            // Criação de objeto
            if (Window::docAtual->camadaSelecionada->objetos->last()->tipo == CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(Window::docAtual->camadaSelecionada->objetos->last());
                c->redimensionar(snap(mouseXR), snap(mouseYR));
            } else if (Window::docAtual->camadaSelecionada->objetos->last()->tipo == RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(Window::docAtual->camadaSelecionada->objetos->last());
                q->setD(Ponto(snap(mouseXR), snap(mouseYR)));
                q->setC(Ponto(q->D.p.x, q->A.p.y));
                q->setB(Ponto(q->A.p.x, q->D.p.y));
            } else if (Window::docAtual->camadaSelecionada->objetos->last()->tipo == ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(Window::docAtual->camadaSelecionada->objetos->last());
                e->setRaioHorizontal(abs(snap(mouseXR) - e->getCentro().p.x));
                e->setRaioVertical(abs(snap(mouseYR) - e->getCentro().p.y));
                e->setControl(Ponto(snap(mouseXR), snap(mouseYR)));
            } else if (Window::docAtual->camadaSelecionada->objetos->last()->tipo == POLILINHA){
                Polilinha *pol = dynamic_cast <Polilinha *>(Window::docAtual->camadaSelecionada->objetos->last());
                pol->deslocarVertice(pol->vertices.last(), snap(mouseXR), snap(mouseYR));
            }
        }
    } else if (onMouseClick){ // não está desenhando e o mouse clicou e agora está se movendo
        if (Window::docAtual->op==SELECIONAR){
            Window::docAtual->selecaoMax = Ponto(mouseXR, mouseYR);

            Objeto* aux;
            preAgrupamento = false;

            int i, j;
            for(i = 0; i < Window::docAtual->camadas.size(); i++) {
                GLint sizeJ = Window::docAtual->camadas.at(i)->objetos->size()-1;
                for(j = 0; j < sizeJ; j++) {
                    aux = Window::docAtual->camadas.at(i)->objetos->at(j);

                    if (aux->tipo == CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux);
                        Ponto con(c->getXc()+c->getRaio(), c->getYc());
                        if (cohenClipping(con, con, Window::docAtual->selecaoMax, Window::docAtual->selecaoMin)){
                            c->selecionado = true;
                            preAgrupamento = true;
                        }
                    } else if (aux->tipo == RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux);
                        if (cohenClipping(q->A.p, q->A.p,
                                          Window::docAtual->selecaoMax, Window::docAtual->selecaoMin) &&
                            cohenClipping(q->B.p, q->B.p,
                                          Window::docAtual->selecaoMax, Window::docAtual->selecaoMin) &&
                            cohenClipping(q->C.p, q->C.p,
                                          Window::docAtual->selecaoMax, Window::docAtual->selecaoMin) &&
                            cohenClipping(q->D.p, q->D.p,
                                          Window::docAtual->selecaoMax, Window::docAtual->selecaoMin)){
                            q->selecionado = true;
                            preAgrupamento = true;
                        }
                    } else if (aux->tipo == ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux);
                        if (cohenClipping(e->control.p, e->control.p,
                                          Window::docAtual->selecaoMax, Window::docAtual->selecaoMin)){
                            e->selecionado = true;
                            preAgrupamento = true;
                        }
                    } else if (aux->tipo == POLILINHA){
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                        if (cohenClipping(pol->boundsMax(), pol->boundsMax(),
                                          Window::docAtual->selecaoMax,  Window::docAtual->selecaoMin) &&
                            cohenClipping(pol->boundsMin(), pol->boundsMin(),
                                          Window::docAtual->selecaoMax, Window::docAtual->selecaoMin)){
                            pol->selecionado = true;
                            preAgrupamento = true;
                        } else {
                            pol->selecionado = false;
                        }

                    }
                }
            }
        } else { // se a operação não for a seleção
            Objeto* aux;
            int i, j;
            for(i = 0; i < Window::docAtual->camadas.size(); i++) {
                for(j = 0; j < Window::docAtual->camadas.at(i)->objetos->size(); j++) {
                    aux = Window::docAtual->camadas.at(i)->objetos->at(j);
                    if (aux->selecionado){
                        if (Window::docAtual->op==TRANSLACAO || Window::docAtual->op==COPIA){
                            qDebug() << "transl";
                            aux->translada(mouseXR, mouseYR);
                        } else if (Window::docAtual->op==ESCALA){
                            // Escala objetos
                            if (aux->tipo == CIRCULO){
                                Circulo *c = dynamic_cast <Circulo *>(aux);
                                c->redimensionar(mouseXR, mouseYR);
                            } else if (aux->tipo == RETANGULO){
                                Retangulo *q = dynamic_cast <Retangulo *>(aux);
                                Ponto centro =  q->centro.p;
                                GLdouble aux = distanciaAB;
                                distanciaAB = sqrt(pow(mouseXR-centro.x, 2)+pow(mouseYR-centro.y, 2));
                                GLdouble fatorx = 2, fatory = 2;//abs(distanciaAB-(xclick-centro.x)), fatory = abs(distanciaAB-(yclick -centro.y));
                                if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                                    q->escala(-1*fatorx, -1*fatory);
                                } else{ // "saindo"
                                    q->escala(fatorx, fatory);
                                }
                            } else if (aux->tipo == ELIPSE){  // de qualquer ponto selecionado
                                Elipse *e = dynamic_cast <Elipse *>(aux);
                                GLint rh = abs(mouseXR-e->getCentro().p.x)+1;
                                GLint rv = abs(mouseYR-e->getCentro().p.y)+1;
                                GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);

                                e->setControl(Ponto(e->getCentro().p.x+ax*(rh),e->getCentro().p.y+ay*(rv)));
                                e->setRaioHorizontal(rh);
                                e->setRaioVertical(rv);
                            } else if (aux->tipo == POLILINHA){
                                /*
                                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                                if (click1!=NULL){
                                    pol->escala(Ponto(event->x(), gfWrldSizeY - event->y()));
                                 }
                                 */
                            }
                        } else if (Window::docAtual->op==DESLOCARPONTOS){
                            voltaSelecao = false;
                            if (aux->tipo == RETANGULO){
                                Retangulo *q = dynamic_cast <Retangulo *>(aux);
                                if (q->A.selecionado){
                                    q->setA(Ponto(mouseXR, mouseYR)); //
                                    q->setB(Ponto(q->A.p.x, q->B.p.y));
                                    q->setC(Ponto(q->C.p.x,q->A.p.y));
                                    q->atualizaMINMAX();
                                } else if (q->B.selecionado){
                                    q->setB(Ponto(mouseXR, mouseYR));
                                    q->setA(Ponto(q->B.p.x, q->A.p.y));
                                    q->setD(Ponto(q->D.p.x, q->B.p.y));
                                } else if (q->C.selecionado){
                                    q->setC(Ponto(mouseXR, mouseYR));
                                    q->setA(Ponto(q->A.p.x, q->C.p.y));
                                    q->setD(Ponto(q->C.p.x, q->D.p.y));
                                    q->atualizaMINMAX();
                                } else if (q->D.selecionado) {
                                    q->setD(Ponto(mouseXR, mouseYR));
                                    q->setB(Ponto(q->B.p.x, q->D.p.y));
                                    q->setC(Ponto(q->D.p.x, q->C.p.y));
                                    q->atualizaMINMAX();
                                }
                            } else if (aux->tipo == CIRCULO){
                                Circulo *c = dynamic_cast <Circulo *>(aux);
                                c->redimensionar(mouseXR, mouseYR);
                            } else if (aux->tipo == ELIPSE){
                                Elipse *e = dynamic_cast <Elipse *>(aux);
                                if (e->getControl().selecionado){ // apenas por ponto de controle
                                    e->setRaioHorizontal(abs(mouseXR-e->getCentro().p.x));
                                    e->setRaioVertical(abs(mouseYR-e->getCentro().p.y));
                                    e->setControl(Ponto(mouseXR, mouseYR));
                                }
                            } else if (aux->tipo == POLILINHA){
                                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                                pol->deslocarVerticeSelecionado(mouseXR, mouseYR);
                            }
                        }
                    }
                }
            }
        }
    }
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent) {
    if (keyEvent->key() == Qt::Key_Up) {
        qDebug() << "up";
        keyEvent->accept();
    } else {
        keyEvent->ignore();
    }
}
