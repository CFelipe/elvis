#include "elipse.h"

Elipse::Elipse(Ponto centro, GLint raioHorizontal, GLint raioVertical, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, bool linha, bool preenchido) : Objeto(colorfill, colorLine, espessuraLinha, linha, preenchido, Objeto::ELIPSE) {
    this->centro = centro;
    this->raioHorizontal = raioHorizontal;
    this->raioVertical = raioVertical;
    this->control = centro;
}

void Elipse::desenhaLinha() {
    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);

    GLdouble d2;
    GLint x = 0;
    GLint y = raioVertical;
    GLdouble d1 = raioVertical * raioVertical - (raioHorizontal*raioHorizontal*raioVertical) + (0.25*raioHorizontal*raioHorizontal);

    int linha_i = 0;

    glPointSize(getEspessuraLinha());

    while (raioHorizontal*raioHorizontal*(y-0.5) > raioVertical*raioVertical*(x+1)) {
        if (d1<0){
            d1 += raioVertical*raioVertical*(2*x+3);
        }else{
            d1 += raioVertical*raioVertical*(2*x+3) + raioHorizontal*raioHorizontal*((-2)*y +2);
            y--;
        }
        x++;
        if(estiloLinha[linha_i] == '-') {
            glBegin(GL_POINTS);
                glVertex2i(  x+centro.x,  y+centro.y );
                glVertex2i(  x+centro.x, -y+centro.y );
                glVertex2i( -x+centro.x,  y+centro.y );
                glVertex2i( -x+centro.x, -y+centro.y );
            glEnd();
        }

        linha_i = (linha_i + 1) % estiloLinha.size();
    }

    // Tem algo de errado aqui (quando a elipse fica muito grande)
    d2 = raioVertical*raioVertical*(x+0.5)*(x+0.5) + raioHorizontal*raioHorizontal*(y-1)*(y-1) - raioHorizontal*raioHorizontal*raioVertical*raioVertical;
    while (y>0){
        if (d2<0){
            d2+=raioVertical*raioVertical*(2*x+2) + raioHorizontal*raioHorizontal*((-2)*y + 3);
            x++;
        } else {
            d2+=raioHorizontal*raioHorizontal*((-2)*y + 3);
        }
        y--;

        if(estiloLinha[linha_i] == '-') {
            glBegin(GL_POINTS);
                glVertex2i(  x+centro.x,  y+centro.y );
                glVertex2i(  x+centro.x, -y+centro.y );
                glVertex2i( -x+centro.x,  y+centro.y );
                glVertex2i( -x+centro.x, -y+centro.y );
            glEnd();
        }

        linha_i = (linha_i + 1) % estiloLinha.size();
    }
}

void Elipse::desenhaFill() {
    GLfloat co[4];
    getColorFill(co);
    glColor4f( co[0],co[1],co[2], co[3]);

    GLdouble d2;
    GLint x = 0;
    GLint y = raioVertical;
    GLdouble d1 = raioVertical * raioVertical - (raioHorizontal*raioHorizontal*raioVertical) + (0.25*raioHorizontal*raioHorizontal);

    while (raioHorizontal*raioHorizontal*(y-0.5) > raioVertical*raioVertical*(x+1)) {
        if (d1<0){
            d1 += raioVertical*raioVertical*(2*x+3);
        }else{
            d1 += raioVertical*raioVertical*(2*x+3) + raioHorizontal*raioHorizontal*((-2)*y +2);
            y--;
        }
        x++;

        glBegin(GL_POINTS);
            // De cima pra baixo
            Objeto::linhaFill(Ponto(-x + centro.x,  y + centro.y),
                              Ponto( x + centro.x,  y + centro.y));
            Objeto::linhaFill(Ponto(-x + centro.x, -y + centro.y),
                              Ponto( x + centro.x, -y + centro.y));
        glEnd();
    }

    // Tem algo de errado aqui (quando a elipse fica muito grande)
    d2 = raioVertical*raioVertical*(x+0.5)*(x+0.5) + raioHorizontal*raioHorizontal*(y-1)*(y-1) - raioHorizontal*raioHorizontal*raioVertical*raioVertical;
    while (y>0){
        if (d2<0){
            d2+=raioVertical*raioVertical*(2*x+2) + raioHorizontal*raioHorizontal*((-2)*y + 3);
            x++;
        } else {
            d2+=raioHorizontal*raioHorizontal*((-2)*y + 3);
        }
        y--;

        Objeto::linhaFill(Ponto(-x + centro.x,  y + centro.y),
                          Ponto( x + centro.x,  y + centro.y));
        Objeto::linhaFill(Ponto(-x + centro.x, -y + centro.y),
                          Ponto( x + centro.x, -y + centro.y));
    }
}
