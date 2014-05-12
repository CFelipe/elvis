#include "circulo.h"
#include "window.h"

Circulo::Circulo(GLint raio, GLint xc, GLint yc) : Objeto(CIRCULO) {
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
    this->selecionado = true;
}

Circulo::Circulo(Circulo *c) : Objeto(CIRCULO) {
    this->raio = c->raio;
    this->xc = c->xc;
    this->yc = c->yc;
    this->colorfill[0] = c->colorfill[0];
    this->colorfill[1] = c->colorfill[1];
    this->colorfill[2] = c->colorfill[2];
    this->colorLine[0] = c->colorLine[0];
    this->colorLine[1] = c->colorLine[1];
    this->colorLine[2] = c->colorLine[2];
    this->espessuraLinha = c->espessuraLinha;
    this->selecionado = true;
}

GLint Circulo::xcView() {
    return xc + Window::docAtual->viewport.x;
}

GLint Circulo::ycView() {
    return yc + Window::docAtual->viewport.y;
}

void Circulo::translada(GLint mouseX, GLint mouseY) {
    xc = mouseX - xclick;
    yc = mouseY - yclick;
}

void Circulo::desseleciona() {
    this->selecionado = false;
}

Ponto Circulo::boundsMax() {
    return Ponto(raio + xc, raio + yc);
}

Ponto Circulo::boundsMin() {
    return Ponto(raio - xc, raio - yc);
}

void Circulo::desenhaLinha() {
    glPointSize(espessuraLinha);
    GLfloat corL[4];
    getColorLine(corL);

    GLint x, y, d, dL, dse;

    // Desenha linha
    glColor4f(corL[0],corL[1],corL[2], corL[3]);

    x = 0;
    y = raio;
    d = 1 - raio;
    dL = 3;
    dse = -2*raio + 5;

    int linha_i = 0;

    while (y>x){
        if (d<0){
           d+=dL;
           dL+=2;
           dse+=2;
        } else {
           d+=dse;
           dL+=2;
           dse+=4;
           y--;
        }
        x++;

        if(estiloLinha[linha_i] == '-') {
            glBegin(GL_POINTS);
                glVertex2i( x + xcView(),  y+ycView());
                glVertex2i( x + xcView(), -y+ycView());
                glVertex2i(-x + xcView(),  y+ycView());
                glVertex2i(-x + xcView(), -y+ycView());
                glVertex2i( y + xcView(),  x+ycView());
                glVertex2i( y + xcView(), -x+ycView());
                glVertex2i(-y + xcView(),  x+ycView());
                glVertex2i(-y + xcView(), -x+ycView());
            glEnd( );
        }

        linha_i = (linha_i + 1) % estiloLinha.size();
    }

    glBegin(GL_POINTS);
        glVertex2i(xcView() - raio, ycView());
        glVertex2i(xcView() + raio, ycView());
        glVertex2i(xcView(), ycView() - raio);
        glVertex2i(xcView(), ycView() + raio);
    glEnd();

}

void Circulo::desenhaFill() {
    GLfloat corF[4];
    getColorFill(corF);

    GLint x, y, d, dL, dse;

    glColor4f(corF[0],corF[1],corF[2], corF[3]);

    x = 0;
    y = raio;
    d = 1 - raio;
    dL = 3;
    dse = -2*raio + 5;

    while (y>x) {
        if (d<0) {
           d+=dL;
           dL+=2;
           dse+=2;
        } else {
           d+=dse;
           dL+=2;
           dse+=4;
           y--;
        }

        x++;

        // De cima pra baixo
        Objeto::linhaFill(Ponto(-x+xcView(),  y+ycView()),
                          Ponto( x+xcView(),  y+ycView()));
        Objeto::linhaFill(Ponto(-y+xcView(),  x+ycView()),
                          Ponto( y+xcView(),  x+ycView()));
        Objeto::linhaFill(Ponto(-y+xcView(), -x+ycView()),
                          Ponto( y+xcView(), -x+ycView()));
        Objeto::linhaFill(Ponto(-x+xcView(), -y+ycView()),
                          Ponto( x+xcView(), -y+ycView()));
    }

    Objeto::linhaFill(Ponto(xcView() - raio, ycView()),
                      Ponto(xcView() + raio, ycView()));
}

void Circulo::desenhaControles() {
    glPointSize(8);
    glBegin(GL_POINTS);
        glColor3f( 0,0.5 , 0 );
        glVertex2i(xcView(), ycView());
        glVertex2i(xcView() + getRaio(), ycView());
    glEnd();


    /*
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
    */
}

void Circulo::redimensionar(GLint xmouse, GLint ymouse) {
    raio =  sqrt(pow((xmouse - xc), 2)+ pow((ymouse - yc), 2));
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}
