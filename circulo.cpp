#include "circulo.h"

Circulo::Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, Objeto::CIRCULO) {
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}

void Circulo::desenha() {
    glPointSize(getEspessuraLinha());
    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);
    GLint x = 0;
    GLint  y = raio;
    GLint  d = 1 - raio;
    GLint  dL = 3;
    GLint  dse = -2*raio + 5;

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

        if(preenchido) {
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
    }
}

void Circulo::redimensionar(GLint xmouse, GLint ymouse) {
    raio =  sqrt(pow((xmouse - xc), 2)+ pow((ymouse - yc), 2));
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}
