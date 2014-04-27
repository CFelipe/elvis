#include "circulo.h"

Circulo::Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo) : Objeto(colorfill, colorLine, espessuraLinha, tipo) {
    this->raio = raio;
    this->xc = xc;
    this->yc = yc;
}

Circulo::desenha() {
    glPointSize(getEspessuraLinha());
    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);
    GLint x = 0;
    GLint  y = raio;
    GLint  d = 1 - raio;
    GLint  dL = 3;
    GLint  dse = -2*raio + 5;

    glBegin(GL_POINTS);
        glVertex2i( x+xc,  y+yc );
        glVertex2i( x+xc, -y+yc );
        glVertex2i(-x+xc,  y+yc );
        glVertex2i(-x+xc, -y+yc );
        glVertex2i( y+xc,  x+yc );
        glVertex2i( y+xc, -x+yc );
        glVertex2i(-y+xc,  x+yc );
        glVertex2i(-y+xc, -x+yc );
    glEnd( );

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
}
