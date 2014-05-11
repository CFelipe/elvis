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

using namespace std;

GLfloat gfWrldSizeX = 800.f;
GLfloat gfWrldSizeY = 600.f;
GLfloat gfWrldSizeZ = 400.f;

Ponto clickCanvas;
Ponto *click1, click2;
GLdouble distanciaAB = 0; // guarda a distancia entre dois pontos quaisquer A e B
GLdouble anguloDeRotacao = M_PI/3; // Valor do ângulo (em radianos) de rotação. Entrada do usuário (seletor)

/* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
 * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
 * opBotaoDireito=false <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um ponto de controle
*/
bool opBotaoDireito = false;
bool onMouseClick = false;
bool preAgrupamento = false;
bool voltaSelecao = false;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
    Camada* camada1 = new Camada("Camada 1");
    camadaSelecionada = camada1;
    camadas.append(camada1);

    linhaColorSelecionada[0] = 0;
    linhaColorSelecionada[1] = 0;
    linhaColorSelecionada[2] = 0;
    linhaColorSelecionada[3] = 0;

    fillColorSelecionada[0] = 1;
    fillColorSelecionada[1] = 0;
    fillColorSelecionada[2] = 0;
    fillColorSelecionada[3] = 0;

    opBotaoDireito = false;
    onMouseClick = false;

    desenhandoPolilinha = false;

    viewport = Ponto(0, 0);

    gradeSep = 40;

    op = CRIACAO;
    forma = Objeto::ELIPSE;

    desenha = true;
    espessuraLinha = 1;
    preenchimento = true;
    linha = true;
    grade = false;
}

GLint GLWidget::mouseToCoords(GLint mouse) {
    if(grade) {
        if((mouse % gradeSep) < (gradeSep / 2)) {
            return (mouse / gradeSep) * gradeSep;
        } else {
            return ((mouse / gradeSep) * gradeSep) + gradeSep;
        }
    } else {
        return mouse;
    }
}

void GLWidget::desenhaGrade(GLint sep) {
    GLint x, y;
    glLineWidth(1);
    glColor3f( .75f, .75f, .75f );

    // Linhas verticais
    for(x = 0; x <= this->width(); x += sep) {
        glBegin( GL_LINES );
            glVertex2i(x, 0);
            glVertex2i(x, height());
        glEnd( );
    }

    // Linhas horizontais
    for(y = 0; y <= this->height(); y += sep) {
        glBegin( GL_LINES );
            glVertex2i(0, y);
            glVertex2i(width(), y);
        glEnd( );
    }
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
            q->setA(A);
            q->setB(B);
            q->setC(C);
            q->setD(D);}
            break;
        case Objeto::CIRCULO:
        {
            Circulo *c = dynamic_cast <Circulo *>(ob);
            GLint xc = c->getXc()-clickCanvas.x;
            GLint yc = c->getYc()-clickCanvas.y;
            c->setXc(xc*cos(anguloDeRotacao) - yc*sin(anguloDeRotacao) + clickCanvas.x);
            c->setYc(xc*sin(anguloDeRotacao) + yc*cos(anguloDeRotacao) + clickCanvas.y);
        }
            break;
        case Objeto::ELIPSE:
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
        case Objeto::POLILINHA:
        {
            Polilinha *pol = dynamic_cast <Polilinha *>(ob);
            Linha *linha = pol->getInit();
            while (linha!=NULL){
                Ponto P0 = linha->getV0().p;
                Ponto P1 = linha->getV1().p;
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
                linha->setV0(P0);
                linha->setV1(P1);
                linha = linha->getNext();
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

void deslocaPontoPolilinha(Polilinha *pol, int x, int y){
    Linha *sel1 = pol->getLinhaSelecionada1(); // nunca é NULL
    Linha *sel2 = pol->getLinhaSelecionada2();
    GLint dy = y - pol->getYClick();
    GLint dx = x - pol->getXClick();
    if (sel1->getV0().selecionado){ // linha 1, portanto sel2=NULL
        sel1->setV0(Vertice(sel1->getV0().p.x+dx, sel1->getV0().p.y+dy));
    } else if (sel1->getV1().selecionado){
        sel1->setV1(Vertice(sel1->getV1().p.x+dx, sel1->getV1().p.y+dy));
        if (sel2!=NULL){
            sel2->setV0(Vertice(sel2->getV0().p.x+dx, sel2->getV0().p.y+dy));
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
    for(i = 0; i < camadas.size(); i++) {
        for(j = 0; j < camadas.at(i)->objetos->size(); j++) {
            aux = camadas.at(i)->objetos->at(j);
            aux->selecionado = (false);
            if (aux->tipo == Objeto::RETANGULO){
                Retangulo *q = dynamic_cast <Retangulo *>(aux);
                q->A.selecionado = (false);
                q->B.selecionado = (false);
                q->C.selecionado = (false);
                q->D.selecionado = (false);
            } else if (aux->tipo == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(aux);
                e->getPControl()->selecionado = (false);
            } else if (aux->tipo == Objeto::POLILINHA){
                /*
                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                pol->desseleciona();
                */
            }
        }
    }
}

void GLWidget::selecionaCirculo(Objeto *aux, Circulo *c, Ponto click){
    aux->selecionado = (true);
    aux->setXClick(click.x -c->getXc());
    aux->setYClick(click.y - c->getYc());
    if (op==COPIA){
        GLfloat fill[4], line[4];
        c->getColorFill(fill);
        c->getColorLine(line);
        Circulo *novo = new Circulo(c->getRaio(), c->getXc(), c->getYc(), fill,line, c->espessuraLinha, c->linha, c->preenchido);

        aux->selecionado = (false);

        camadaSelecionada->objetos->append(novo);
        camadaSelecionada->objetos->last()->selecionado = true;
        camadaSelecionada->objetos->last()->setXClick(click.x -c->getXc() );
        camadaSelecionada->objetos->last()->setYClick(click.y- c->getYc());
    }
}

void GLWidget::selecionaQuadrilatero(Objeto *aux, Retangulo *q, Ponto click){
    aux->selecionado = (true);
    // aux->setXClick(-q->A.p.x+ click.x);
    // aux->setYClick(-q->A.p.y+click.y );
    aux->setXClick(click.x);
    aux->setYClick(click.y );

    if (op == COPIA){
        GLfloat fill[4], line[4];
        q->getColorFill(fill);
        q->getColorLine(line);
        Retangulo *novo = new Retangulo(q->A, q->B, q->C, q->D, fill, line, q->espessuraLinha, q->linha, q->preenchido);

        camadaSelecionada->objetos->append(novo);

        aux->selecionado = (false);
        camadaSelecionada->objetos->last()->selecionado = true;
        camadaSelecionada->objetos->last()->setXClick(click.x);
        camadaSelecionada->objetos->last()->setYClick(click.y);
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
    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, espessuraLinha, linha, preenchimento);
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

void excluirPontoControlePolilinha(Polilinha *pol, int x, int y){
    if (clippingAreaVertice(pol->getInit()->getV0().p, Ponto(x, y))){
        pol->remove(pol->getInit());
    } else {
        Linha *linha = pol->getInit();
         while (linha!=NULL){
             if (clippingAreaVertice(linha->getV1().p, Ponto(x, y))){
                 if (linha->getNext()!=NULL){
                     linha->getNext()->setV0(linha->getV0());
                 }
                 pol->selecionado = (true);
                 pol->remove(linha);
                 linha = NULL;
             } else linha = linha->getNext();
         }
    }

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

    if(grade) {
        desenhaGrade(gradeSep);
    }

    if (op==ROTACAO && opBotaoDireito){ // se a opção se rotação foi selecionada e o botão direito foi pressionado, então desenhe uma cruzinha
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
    for(i = 0; i < camadas.size(); i++) {
        for(j = 0; j < camadas.at(i)->objetos->size(); j++) {
            aux = camadas.at(i)->objetos->at(j);
            aux->desenha();
        }
    }
    glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    GLint mouseX = mouseToCoords(event->x());
    GLint mouseY = mouseToCoords(gfWrldSizeY - event->y());

    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton) { // se o botão esquerdo for clicado, cria a forma
        if (desenha){
            if (onMouseClick==false){
                if (forma == Objeto::CIRCULO){
                    Circulo *c = new Circulo(0, mouseX, mouseY, fillColorSelecionada, linhaColorSelecionada, espessuraLinha, linha, preenchimento);
                    camadaSelecionada->objetos->append(c);
                } else if (forma == Objeto::RETANGULO) {
                    Ponto M(mouseX, mouseY);
                    Retangulo *q = new Retangulo(M, M, M, M, fillColorSelecionada, linhaColorSelecionada, espessuraLinha, linha, preenchimento);
                    camadaSelecionada->objetos->append(q);
                } else if (forma == Objeto::ELIPSE){
                    GLint xc = mouseX;
                    GLint yc = mouseY;
                    Elipse *e = new Elipse(Ponto(xc, yc), 0, 0, fillColorSelecionada, linhaColorSelecionada, espessuraLinha, linha, preenchimento);
                    camadaSelecionada->objetos->append(e);
                } else if (forma == Objeto::POLILINHA){
                    /*
                    Ponto click(mouseX, mouseY);
                    Polilinha *pol = new Polilinha(linhaColorSelecionada, espessuraLinha);
                    pol->insert(click, click, NULL);
                    camadaSelecionada->objetos->append(pol);
                }
            } else { // se o mouse foi clicado anteriormente, e estivermos desenhando uma polinha ...
                if (forma == Objeto::POLILINHA){
                    /*
                    Ponto click(mouseX, mouseY);
                    onMouseClick = false;
                    Polilinha *pol = dynamic_cast <Polilinha *>(camadaSelecionada->objetos->last());
                    pol->insert(pol->getFim()->getV1().p, click, pol->getFim());
                    */
                }
            }

            onMouseClick = !onMouseClick;
        } else if (op==SELECIONAR){
            if (preAgrupamento==true){
                Retangulo *q = dynamic_cast <Retangulo *>(camadaSelecionada->objetos->last());
                Ponto click(event->x(),gfWrldSizeY-event->y());

                if (clippingAreaVertice(q->A.p, click) ||
                    clippingAreaVertice(q->B.p, click) ||
                    clippingAreaVertice(q->C.p, click) ||
                    clippingAreaVertice(q->D.p, click)) {
                    q->selecionado = true;
                    voltaSelecao = true;
                    op = TRANSLACAO;
                } else {
                    Retangulo *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                    if (cohenClipping(q->A.p, q->B.p,clik->C.p, clik->B.p) || cohenClipping(q->B.p, q->D.p,clik->C.p, clik->B.p) || cohenClipping(q->D.p, q->C.p,clik->C.p, clik->B.p) || cohenClipping(q->C.p, q->A.p,clik->C.p, clik->B.p)){
                        q->selecionado = (true);
                        voltaSelecao = true;
                        op = TRANSLACAO;
                    } else if (event->x()<=q->max.x && event->x()>=q->min.x && (gfWrldSizeY-event->y())<=q->max.y && (gfWrldSizeY-event->y())>=q->min.y){
                        q->selecionado = (true);
                        voltaSelecao = true;
                        op = TRANSLACAO;
                    }else {
                        q->selecionado = (false);
                        preAgrupamento = false;
                        camadaSelecionada->objetos->removeLast();
                        desselecionaALL();
                    }
                }
                if (preAgrupamento==true){
                    Objeto* aux;
                    int i, j;
                    for(i = 0; i < camadas.size(); i++) {
                        GLint sizeJ = camadas.at(i)->objetos->size();
                        for(j = 0; j < sizeJ; j++) {
                            aux = camadas.at(i)->objetos->at(j);
                            if (aux->selecionado){
                                if (aux->tipo==Objeto::CIRCULO){
                                    Circulo *c = dynamic_cast <Circulo *>(aux);
                                    aux->setXClick(event->x() -c->getXc());
                                    aux->setYClick(gfWrldSizeY-event->y() - c->getYc());
                                } else if (aux->tipo==Objeto::ELIPSE){
                                    Elipse *e = dynamic_cast <Elipse *>(aux);
                                    aux->setXClick(event->x()-e->getCentro().p.x);
                                    aux->setYClick(gfWrldSizeY-event->y()-e->getCentro().p.y);
                                } else{
                                    aux->setXClick(event->x());
                                    aux->setYClick(gfWrldSizeY-event->y());
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
                for(i = 0; i < camadas.size(); i++) {
                    oneAnySelection = false;
                    GLint sizeJ = camadas.at(i)->objetos->size();
                    for(j = 0; j < sizeJ; j++) {
                        aux = camadas.at(i)->objetos->at(j);
                        if (aux->tipo == Objeto::CIRCULO){
                            Circulo *c = dynamic_cast <Circulo *>(aux);
                            GLint distancia =sqrt(pow(event->x()-c->getXc(), 2)+pow(gfWrldSizeY-event->y()-c->getYc(), 2));
                            if (abs(c->getRaio()-distancia)<=CONTROL){
                                c->selecionado = true;
                                aux->setXClick(event->x() -c->getXc());
                                aux->setYClick(gfWrldSizeY-event->y() - c->getYc());

                                oneAnySelection = true;
                                voltaSelecao = true;
                                op=TRANSLACAO;
                            } else {
                                c->selecionado = (false);
                            }
                            if (c->selecionado == false && c->preenchido){
                                if(distancia<=c->getRaio()){
                                    c->selecionado = true;
                                    oneAnySelection = true;
                                    aux->setXClick(event->x() -c->getXc());
                                    aux->setYClick(gfWrldSizeY-event->y() - c->getYc());

                                    voltaSelecao = true;
                                    op=TRANSLACAO;
                                } else {
                                    c->selecionado = (false);
                                }
                            }
                        } else if(aux->tipo == Objeto::ELIPSE){
                            Elipse *e = dynamic_cast <Elipse *>(aux);
                            Ponto click(event->x(),gfWrldSizeY-event->y());
                            if (clippingAreaVertice(e->getControl().p, click)){  // clicou sobre o ponto de controle ?
                                aux->selecionado = (true);
                                aux->setXClick(event->x()-e->getCentro().p.x);
                                aux->setYClick(gfWrldSizeY-event->y()-e->getCentro().p.y);

                                oneAnySelection = true;
                                voltaSelecao = true;
                                op=TRANSLACAO;
                            } else {
                                aux->selecionado = (false);
                            }
                        } else if (aux->tipo == Objeto::RETANGULO){
                            Retangulo *q = dynamic_cast <Retangulo *>(aux);
                            Ponto click(event->x(),gfWrldSizeY-event->y());
                            if (clippingAreaVertice(q->A.p, click) || clippingAreaVertice(q->B.p, click) || clippingAreaVertice(q->C.p, click) || clippingAreaVertice(q->D.p, click)){
                                aux->selecionado = (true);
                                aux->setXClick(event->x());
                                aux->setYClick(gfWrldSizeY-event->y());

                                oneAnySelection = true;
                                voltaSelecao = true;
                                op=TRANSLACAO;
                            } else { // testa se o click foi na linha do quadrado (não preenchido)
                                Retangulo *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                                if (cohenClipping(q->A.p, q->B.p,clik->C.p, clik->B.p) || cohenClipping(q->B.p, q->D.p,clik->C.p, clik->B.p) || cohenClipping(q->D.p, q->C.p,clik->C.p, clik->B.p) || cohenClipping(q->C.p, q->A.p,clik->C.p, clik->B.p)){
                                    aux->selecionado = (true);
                                    aux->setXClick(event->x());
                                    aux->setYClick(gfWrldSizeY-event->y());

                                    oneAnySelection = true;
                                    voltaSelecao = true;
                                    op=TRANSLACAO;
                                } else {
                                    aux->selecionado = (false);
                                }
                            }
                            if (q->selecionado == false && q->preenchido){// se o quadrilátero estiver preenchido
                                if (event->x()<=q->D.p.x && event->x()>=q->A.p.x && (gfWrldSizeY-event->y())<=q->A.p.y && (gfWrldSizeY-event->y())>=q->D.p.y){
                                    aux->selecionado = (true);
                                    aux->setXClick(event->x());
                                    aux->setYClick(gfWrldSizeY-event->y());
                                    oneAnySelection = true;
                                    voltaSelecao = true;
                                    op=TRANSLACAO;
                                } else {
                                    aux->selecionado = false;
                                }
                            }
                        } else if (aux->tipo==Objeto::POLILINHA){
                            /*
                            Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                            Retangulo *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                            Linha *linha = pol->getInit();
                            pol->selecionado = (false);
                            while (linha!=NULL && pol->selecionado == false){
                                if (cohenClipping(linha->getV0().p, linha->getV1().p, clik->C.p, clik->B.p)){
                                    pol->selecionado = (true);
                                    aux->setXClick(event->x());
                                    aux->setYClick(gfWrldSizeY-event->y());

                                    oneAnySelection = true;
                                    voltaSelecao = true;
                                    op=TRANSLACAO;
                                }
                                linha = linha->getNext();
                            }
                            */
                        }
                    }
                }
                if (oneAnySelection==false){
                    cout<<"MULTIPLOS"<<endl;
                    Ponto A(event->x(), gfWrldSizeY-event->y());
                    Ponto D(event->x(), gfWrldSizeY-event->y());
                    Ponto B(event->x(), gfWrldSizeY-event->y());
                    Ponto C(event->x(), gfWrldSizeY-event->y());
                    GLfloat colorFill[4] = {1, 0, 0, 0};
                    GLfloat colorLine[4] = {0, 0, 0, 0};
                    Retangulo *q = new Retangulo(A, B, C, D, colorFill, colorLine, 1, true, false);
                    q->isSeletor = true;
                    q->isVisible = true;
                    camadaSelecionada->objetos->append(q);
                    preAgrupamento=false;
                }
            }
            onMouseClick = true;
        } else {
            // Determinação de objeto selecionado baseado em onde o usuário clicou
            Objeto* aux;
            int i, j;
            for(i = 0; i < camadas.size(); i++) {
                GLint sizeJ = camadas.at(i)->objetos->size();
                for(j = 0; j < sizeJ; j++) {
                    aux = camadas.at(i)->objetos->at(j);

                    if (aux->tipo == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux);
                        Ponto click = Ponto(event->x(), gfWrldSizeY-event->y());
                        GLint distancia =sqrt(pow(event->x()-c->getXc(), 2)+pow(gfWrldSizeY-event->y()-c->getYc(), 2));
                        if (abs(c->getRaio()-distancia)<=CONTROL){
                            selecionaCirculo(aux, c, click);
                            //op = TRANSLACAO;
                        } else {
                            aux->selecionado = (false);
                        }
                        if (c->selecionado == false && c->preenchido){
                            if(distancia<=c->getRaio()){
                                selecionaCirculo(aux, c, Ponto(event->x(), gfWrldSizeY-event->y()));
                                //op = TRANSLACAO;
                            }
                        }
                    } else if(aux->tipo == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux);
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
                            aux->selecionado = (true);
                            aux->setXClick(e->getControl().p.x - e->getCentro().p.x);
                            aux->setYClick(e->getControl().p.y - e->getCentro().p.y);
                            e->getPControl()->selecionado = (true);
                        } else{
                            cout<<fxy<<endl;
                            if(fxy==0){ // clicou sobre os limites da elispe?
                                aux->selecionado = (true);
                                aux->setXClick(e->getControl().p.x-e->getCentro().p.x);
                                aux->setYClick(e->getControl().p.y-e->getCentro().p.y);
                            } else {
                                aux->selecionado = (false);
                            }
                        }
                        //} //if (Preenchida){
                        //se a forma estiver preenchida
                        //}
                        if (aux->selecionado){
                            if (op==COPIA){
                                GLfloat fill[4], line[4];
                                e->getColorFill(fill);
                                e->getColorLine(line);
                                Elipse *novo = new Elipse(e->getCentro().p, e->getRaioHorizontal(), e->getRaioVertical(), fillColorSelecionada, linhaColorSelecionada, espessuraLinha, linha, preenchimento);
                                novo->setControl(e->getControl());
                                aux->selecionado = (false);
                                camadaSelecionada->objetos->append(novo);
                                camadaSelecionada->objetos->last()->selecionado = (true);

                                camadaSelecionada->objetos->last()->setXClick(aux->getXClick());
                                camadaSelecionada->objetos->last()->setYClick(aux->getYClick());
                            }
                        }

                    }else if (aux->tipo == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux);
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
                                aux->selecionado = (false);
                            }
                        }
                        if (q->selecionado == false && q->preenchido){// se o quadrilátero estiver preenchido
                            if (event->x()<=q->D.p.x && event->x()>=q->A.p.x && (gfWrldSizeY-event->y())<=q->A.p.y && (gfWrldSizeY-event->y())>=q->D.p.y){
                                selecionaQuadrilatero(aux, q, click);
                            }
                        }
                    } else if (aux->tipo == Objeto::POLILINHA){
                        /*
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                        Retangulo *clik = getAreaClippingMouse(event->x(),gfWrldSizeY-event->y());
                        Linha *linha = pol->getInit();
                        pol->selecionado = (false);
                        while (linha!=NULL && pol->selecionado == false){
                            linha->getVV0()->selecionado = false;
                            linha->getVV1()->selecionado = false;
                            if (cohenClipping(linha->getV0().p, linha->getV1().p, clik->C.p, clik->B.p)){
                                pol->selecionado = (true);
                                pol->setXClick(event->x());
                                pol->setYClick(gfWrldSizeY-event->y());
                                pol->setLinhaSelecionada1(linha);
                                pol->setLinhaSelecionada2(NULL);
                                if (clippingAreaVertice(linha->getV0().p, Ponto(event->x(),gfWrldSizeY-event->y()))){
                                    linha->getVV0()->selecionado = (true);
                                    cout<<"Seelcionado: P0 - Linha 1"<<endl;
                                } else {
                                    if (clippingAreaVertice(linha->getV1().p, Ponto(event->x(),gfWrldSizeY-event->y()))){
                                        linha->getVV1()->selecionado = (true);
                                        pol->setLinhaSelecionada2(linha->getNext());
                                        cout<<"Seelcionado: P1 - Linha"<<endl;
                                        if (linha!=pol->getFim()){
                                            linha->getNext()->getVV0()->selecionado = (true);
                                            linha->getNext()->getVV1()->selecionado = false;
                                            cout<<"Selecionado: P0 - Linha->next()"<<endl;
                                        }
                                    }
                                }

                            }
                            linha = linha->getNext();
                        }
                        if (pol->selecionado==false && op==ESCALA){
                            click1 = NULL;
                            if (clippingAreaVertice(pol->getMax(), Ponto(event->x(),gfWrldSizeY-event->y()))){
                                pol->selecionado = true;
                                click1 = pol->getPMax();
                                click2.x = pol->getMax().x;
                                click2.y = pol->getMax().y;
                            } else if(clippingAreaVertice(pol->getMin(), Ponto(event->x(),gfWrldSizeY-event->y()))){
                                pol->selecionado = true;
                                click1 = pol->getPMin();
                                click2.x = pol->getMin().x;
                                click2.y = pol->getMin().y;
                            }
                        } else if (pol->selecionado == true) {
                            if (op==COPIA){
                                Polilinha *novo = new Polilinha(linhaColorSelecionada, espessuraLinha);
                                camadaSelecionada->objetos->append(novo);
                                Linha *linha = pol->getInit();
                                while (linha!=NULL){
                                    novo->insert(linha->getV0().p, linha->getV1().p, novo->getFim());
                                    linha = linha->getNext();
                                }
                                aux->selecionado = (false);

                                camadaSelecionada->objetos->last()->selecionado = (true);
                                camadaSelecionada->objetos->last()->setXClick(aux->getXClick());
                                camadaSelecionada->objetos->last()->setYClick(aux->getYClick());
                            } else if (op==INSERT_REMOVE_PONTO){ //insere um novo vértice;
                                Ponto newP1(pol->getLinhaSelecionada1()->getV1().p.x, pol->getLinhaSelecionada1()->getV1().p.y);
                                Ponto newP0(event->x(), gfWrldSizeY-event->y());
                                pol->insert(newP0, newP1, pol->getLinhaSelecionada1());
                                pol->getLinhaSelecionada1()->setV1(newP0);
                                pol->getLinhaSelecionada1()->getVV0()->selecionado = false;
                                pol->getLinhaSelecionada1()->getVV1()->selecionado = (true);
                                pol->setLinhaSelecionada2(pol->getLinhaSelecionada1()->getNext());
                                pol->getLinhaSelecionada2()->getVV0()->selecionado = (true);
                                pol->getLinhaSelecionada2()->getVV1()->selecionado = (false);


                            }
                        }
                    */
                    }
                    if (op == ROTACAO){
                        if (opBotaoDireito==false) { // se o usuário não clicou com o botão direito fora da área do quadrado
                            clickCanvas.x = (event->x());
                            clickCanvas.y = (gfWrldSizeY-event->y());
                        }
                        rotacionaObjeto(aux);
                    }
                }
            }
            onMouseClick = true;
            //updateGL();


        }
        updateGL();
    } else if ((event->buttons() & Qt::RightButton) == Qt::RightButton){
        if (desenha){
            if (onMouseClick){ // se durante a rasterização da forma o botão direito for pressionado, então delete a forma [QUALQUER]
                onMouseClick = false;
                camadaSelecionada->objetos->removeLast();
                updateGL();
            }
        } else {
            if (op==ROTACAO){
                clickCanvas.x = (event->x());
                clickCanvas.y = (gfWrldSizeY-event->y());
                opBotaoDireito = true;
                updateGL();
            } else if (op == INSERT_REMOVE_PONTO){ // removemos o ponto selecionado (apenas para polilinha)
                Objeto* aux;
                int i, j;
                for(i = 0; i < camadas.size(); i++) {
                    for(j = 0; j < camadas.at(i)->objetos->size(); j++) {
                        aux = camadas.at(i)->objetos->at(j);

                        if (aux->tipo==Objeto::POLILINHA){
                            /*
                            Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                            excluirPontoControlePolilinha(pol, event->x(), gfWrldSizeY-event->y());
                            if (pol->getInit()==NULL){
                                camadaSelecionada->objetos->removeLast();
                            }
                        */
                        }
                    }
                }
                updateGL();
            }
        }
        desenhandoPolilinha = false; // da especificação: quando clicado com o botão do meio, finaliza o desenho da polilinha
    } else if ((event->buttons() & Qt::MidButton) == Qt::MidButton){
        onMouseClick = false;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    if (desenha==false){
        onMouseClick = false;
        if (op==SELECIONAR && camadaSelecionada->objetos->last()->tipo==Objeto::RETANGULO){
            //camadaSelecionada->objetos->removeLast();
            Retangulo *c = dynamic_cast <Retangulo *>(camadaSelecionada->objetos->last());
            if (c->isSeletor){

                //c->isVisible = false;
                if (preAgrupamento == false){
                    camadaSelecionada->objetos->removeLast();
                }
                updateGL();
            }

        }
        if (voltaSelecao){
            op=SELECIONAR;
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    GLint mouseX = mouseToCoords(event->x());
    GLint mouseY = mouseToCoords(gfWrldSizeY - event->y());

    if (desenha){
        if(onMouseClick){
            // Trocar isso tudo por aux->redimensionar
            if (camadaSelecionada->objetos->last()->tipo == Objeto::CIRCULO){
                Circulo *c = dynamic_cast <Circulo *>(camadaSelecionada->objetos->last());
                c->redimensionar(event->x(), gfWrldSizeY - event->y());
            } else if (camadaSelecionada->objetos->last()->tipo == Objeto::RETANGULO){
                // Trocar pra depender do ponto
                Retangulo *q = dynamic_cast <Retangulo *>(camadaSelecionada->objetos->last());
                q->setD(Ponto(mouseX, mouseY));
                q->setC(Ponto(q->D.p.x, q->A.p.y));
                q->setB(Ponto(q->A.p.x, q->D.p.y));
            } else if (camadaSelecionada->objetos->last()->tipo == Objeto::ELIPSE){
                Elipse *e = dynamic_cast <Elipse *>(camadaSelecionada->objetos->last());
                e->setRaioHorizontal(abs(event->x()-e->getCentro().p.x));
                e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().p.y));
                e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
            } else if (camadaSelecionada->objetos->last()->tipo == Objeto::POLILINHA){
                Polilinha *pol = dynamic_cast <Polilinha *>(camadaSelecionada->objetos->last());
                Ponto click(event->x(),gfWrldSizeY-event->y());
                pol->setFim(pol->getFim()->getV0().p, click);
            }
        }
    } else if (onMouseClick){ // não está desenhando e o mouse clicou e agora está se movendo
        if (op==SELECIONAR){
            Retangulo *seletor = dynamic_cast <Retangulo *>(camadaSelecionada->objetos->last());
            seletor->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
            seletor->setC(Ponto(seletor->D.p.x, seletor->A.p.y));
            seletor->setB(Ponto(seletor->A.p.x, seletor->D.p.y));
            Objeto* aux;
            int i, j;
            preAgrupamento = false;
            for(i = 0; i < camadas.size(); i++) {
                GLint sizeJ = camadas.at(i)->objetos->size()-1;
                for(j = 0; j < sizeJ; j++) {
                    aux = camadas.at(i)->objetos->at(j);

                    if (aux->tipo == Objeto::CIRCULO){
                        Circulo *c = dynamic_cast <Circulo *>(aux);
                        Ponto con(c->getXc()+c->getRaio(), c->getYc());
                        if (cohenClipping(con, con, seletor->max, seletor->min)){
                            c->selecionado = true;
                            preAgrupamento = true;
                        }
                    } else if (aux->tipo == Objeto::RETANGULO){
                        Retangulo *q = dynamic_cast <Retangulo *>(aux);
                        if (q->isSeletor==false && cohenClipping(q->A.p,q->A.p,seletor->max, seletor->min) && cohenClipping(q->B.p,q->B.p,seletor->max, seletor->min) && cohenClipping(q->C.p,q->C.p,seletor->max, seletor->min) && cohenClipping(q->D.p,q->D.p,seletor->max, seletor->min)){
                            q->selecionado = true;
                            preAgrupamento = true;
                        }
                    } else if (aux->tipo == Objeto::ELIPSE){
                        Elipse *e = dynamic_cast <Elipse *>(aux);
                        if (cohenClipping(e->control.p,e->control.p,seletor->max, seletor->min)){
                            e->selecionado = true;
                            preAgrupamento = true;
                        }
                    } else if (aux->tipo == Objeto::POLILINHA){
                        /*
                        Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                        if (cohenClipping(pol->getMax(), pol->getMax(),seletor->max, seletor->min) && cohenClipping(pol->getMin(), pol->getMin(),seletor->max, seletor->min)){
                            pol->selecionado = true;
                            preAgrupamento = true;
                        } else {
                            pol->selecionado = false;
                        }
                        */

                    }
                }
            }
        } else { // se a operação não for a seleção
            Objeto* aux;
            int i, j;
            for(i = 0; i < camadas.size(); i++) {
                for(j = 0; j < camadas.at(i)->objetos->size(); j++) {
                    aux = camadas.at(i)->objetos->at(j);
                    if (aux->selecionado){
                        if (op==TRANSLACAO || op==COPIA){
                                aux->translada(event->x(), gfWrldSizeY - event->y());
                        } else if (op==ESCALA){
                            if (aux->tipo == Objeto::CIRCULO){
                                Circulo *c = dynamic_cast <Circulo *>(aux);
                                c->redimensionar(event->x(), gfWrldSizeY-event->y());
                            } else if (aux->tipo == Objeto::RETANGULO){
                                Retangulo *q = dynamic_cast <Retangulo *>(aux);
                                Ponto centro =  q->centro.p;
                                GLdouble aux = distanciaAB;
                                distanciaAB = sqrt(pow(event->x()-centro.x, 2)+pow(gfWrldSizeY-event->y()-centro.y, 2));
                                GLdouble fatorx = 2, fatory = 2;//abs(distanciaAB-(xclick-centro.x)), fatory = abs(distanciaAB-(yclick -centro.y));
                                if (aux-distanciaAB>0){ // setinha do mouse "entrando" na forma
                                    q->escala(-1*fatorx, -1*fatory);
                                } else{ // "saindo"
                                    q->escala(fatorx, fatory);
                                }
                            } else if (aux->tipo == Objeto::ELIPSE){  // de qualquer ponto selecionado
                                Elipse *e = dynamic_cast <Elipse *>(aux);
                                GLint rh = abs(event->x()-e->getCentro().p.x)+1;
                                GLint rv = abs(gfWrldSizeY-event->y()-e->getCentro().p.y)+1;
                                GLint ax = e->getControl().p.x==e->getCentro().p.x+e->getRaioHorizontal()?(1):(-1), ay = e->getControl().p.y==e->getCentro().p.y+e->getRaioVertical()?(1):(-1);

                                e->setControl(Ponto(e->getCentro().p.x+ax*(rh),e->getCentro().p.y+ay*(rv)));
                                e->setRaioHorizontal(rh);
                                e->setRaioVertical(rv);
                            } else if (aux->tipo == Objeto::POLILINHA){
                                /*
                                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                                if (click1!=NULL){
                                    pol->escala(Ponto(event->x(), gfWrldSizeY - event->y()));
                                 }
                                 */
                            }
                        } else if (op==DESLOCARPONTOS){
                            if (aux->tipo == Objeto::RETANGULO){
                                Retangulo *q = dynamic_cast <Retangulo *>(aux);
                                if (q->A.selecionado){
                                    q->setA(Ponto(event->x(), gfWrldSizeY-event->y())); //
                                    q->setB(Ponto(q->A.p.x, q->B.p.y));
                                    q->setC(Ponto(q->C.p.x,q->A.p.y));
                                    q->atualizaMINMAX();
                                } else if (q->B.selecionado){
                                    q->setB(Ponto(event->x(), gfWrldSizeY-event->y()));
                                    q->setA(Ponto(q->B.p.x, q->A.p.y));
                                    q->setD(Ponto(q->D.p.x, q->B.p.y));
                                } else if (q->C.selecionado){
                                    q->setC(Ponto(event->x(), gfWrldSizeY-event->y()));
                                    q->setA(Ponto(q->A.p.x, q->C.p.y));
                                    q->setD(Ponto(q->C.p.x, q->D.p.y));
                                    q->atualizaMINMAX();
                                } else if (q->D.selecionado) {
                                    q->setD(Ponto(event->x(), gfWrldSizeY-event->y()));
                                    q->setB(Ponto(q->B.p.x, q->D.p.y));
                                    q->setC(Ponto(q->D.p.x, q->C.p.y));
                                    q->atualizaMINMAX();
                                }
                            } else if (aux->tipo == Objeto::CIRCULO){
                                Circulo *c = dynamic_cast <Circulo *>(aux);
                                c->redimensionar(event->x(), gfWrldSizeY-event->y());
                            } else if (aux->tipo == Objeto::ELIPSE){
                                Elipse *e = dynamic_cast <Elipse *>(aux);
                                if (e->getControl().selecionado){ // apenas por ponto de controle
                                    e->setRaioHorizontal(abs(event->x()-e->getCentro().p.x));
                                    e->setRaioVertical(abs(gfWrldSizeY-event->y()-e->getCentro().p.y));
                                    e->setControl(Ponto(event->x(), gfWrldSizeY-event->y()));
                                }
                            } else if (aux->tipo == Objeto::POLILINHA){
                                /*
                                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                                deslocaPontoPolilinha(pol, event->x(), gfWrldSizeY-event->y());
                                */
                            }
                        } else if (op==INSERT_REMOVE_PONTO){
                                /*
                            if (aux->tipo == Objeto::POLILINHA){
                                Polilinha *pol = dynamic_cast <Polilinha *>(aux);
                                deslocaPontoPolilinha(pol, event->x(), gfWrldSizeY-event->y());
                            }
                            */
                        }
                    }
                }
            }
        }
    }
    updateGL();
}
