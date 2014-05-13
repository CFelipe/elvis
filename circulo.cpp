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
                glVertex2i(intXView( x + xc), intYView( y+yc));
                glVertex2i(intXView( x + xc), intYView(-y+yc));
                glVertex2i(intXView(-x + xc), intYView( y+yc));
                glVertex2i(intXView(-x + xc), intYView(-y+yc));
                glVertex2i(intXView( y + xc), intYView( x+yc));
                glVertex2i(intXView( y + xc), intYView(-x+yc));
                glVertex2i(intXView(-y + xc), intYView( x+yc));
                glVertex2i(intXView(-y + xc), intYView(-x+yc));
            glEnd( );
        }

        linha_i = (linha_i + 1) % estiloLinha.size();
    }

    glBegin(GL_POINTS);
        glVertex2i(intXView(xc - raio), intYView(yc));
        glVertex2i(intXView(xc + raio), intYView(yc));
        glVertex2i(intXView(xc), intYView(yc - raio));
        glVertex2i(intXView(xc), intYView(yc + raio));
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
        linhaFill(Ponto(intXView(-x+xc), intYView( y+yc)),
                  Ponto(intXView( x+xc), intYView( y+yc)));
        linhaFill(Ponto(intXView(-y+xc), intYView( x+yc)),
                  Ponto(intXView( y+xc), intYView( x+yc)));
        linhaFill(Ponto(intXView(-y+xc), intYView(-x+yc)),
                  Ponto(intXView( y+xc), intYView(-x+yc)));
        linhaFill(Ponto(intXView(-x+xc), intYView(-y+yc)),
                  Ponto(intXView( x+xc), intYView(-y+yc)));
    }

    linhaFill(Ponto(intXView(xc - raio), intYView(yc)),
              Ponto(intXView(xc + raio), intYView(yc)));
}

void Circulo::desenhaControles() {
    glPointSize(8);
    glBegin(GL_POINTS);
        glColor3f( 0,0.5 , 0 );
        glVertex2i(intXView(xc), intYView(yc));
        glVertex2i(intXView(xc + getRaio()), intYView(yc));
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
