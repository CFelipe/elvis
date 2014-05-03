#include "circulo.h"

Circulo::Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, Objeto::CIRCULO) {
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}

void Circulo::desenhaLinha() {
    glPointSize(getEspessuraLinha());
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
                glVertex2i( x+xc,  y+yc);
                glVertex2i( x+xc, -y+yc);
                glVertex2i(-x+xc,  y+yc);
                glVertex2i(-x+xc, -y+yc);
                glVertex2i( y+xc,  x+yc);
                glVertex2i( y+xc, -x+yc);
                glVertex2i(-y+xc,  x+yc);
                glVertex2i(-y+xc, -x+yc);
            glEnd( );
        }

        linha_i = (linha_i + 1) % estiloLinha.size();
    }

    glBegin(GL_POINTS);
        glVertex2i(xc - raio, yc);
        glVertex2i(xc + raio, yc);
        glVertex2i(xc, yc - raio);
        glVertex2i(xc, yc + raio);
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
        Objeto::linhaFill(Ponto(-x+xc,  y+yc),
                          Ponto( x+xc,  y+yc));
        Objeto::linhaFill(Ponto(-y+xc,  x+yc),
                          Ponto( y+xc,  x+yc));
        Objeto::linhaFill(Ponto(-y+xc, -x+yc),
                          Ponto( y+xc, -x+yc));
        Objeto::linhaFill(Ponto(-x+xc, -y+yc),
                          Ponto( x+xc, -y+yc));
    }

    Objeto::linhaFill(Ponto(xc - raio, yc),
                      Ponto(xc + raio, yc));
}

void Circulo::redimensionar(GLint xmouse, GLint ymouse) {
    raio =  sqrt(pow((xmouse - xc), 2)+ pow((ymouse - yc), 2));
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}
