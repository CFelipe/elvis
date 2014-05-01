#include "circulo.h"

Circulo::Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, Objeto::CIRCULO) {
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}

void Circulo::desenha() {
    glPointSize(getEspessuraLinha());
    GLfloat corL[4];
    GLfloat corF[4];
    getColorLine(corL);
    getColorFill(corF);

    GLint x, y, d, dL, dse;

    // Desenha fill
    if(preenchido) {
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
            Objeto::linhaFill(Ponto(-x+xc + 1,  y+yc - 1),
                              Ponto( x+xc - 1,  y+yc - 1));
            Objeto::linhaFill(Ponto(-y+xc + 1,  x+yc - 1),
                              Ponto( y+xc - 1,  x+yc - 1));
            Objeto::linhaFill(Ponto(-y+xc + 1, -x+yc + 1),
                              Ponto( y+xc - 1, -x+yc + 1));
            Objeto::linhaFill(Ponto(-x+xc + 1, -y+yc + 1),
                              Ponto( x+xc - 1, -y+yc + 1));
        }
    }

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

}

void Circulo::redimensionar(GLint xmouse, GLint ymouse) {
    raio =  sqrt(pow((xmouse - xc), 2)+ pow((ymouse - yc), 2));
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}
