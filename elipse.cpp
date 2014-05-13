#include "elipse.h"
#include "window.h"

Elipse::Elipse(Ponto centro, GLint raioHorizontal, GLint raioVertical) : Objeto(ELIPSE) {
    this->centro = centro;
    this->raioHorizontal = raioHorizontal;
    this->raioVertical = raioVertical;
    this->control = centro;
    this->selecionado = true;
}

void Elipse::translada(GLint mouseX, GLint mouseY) {
    GLint ax = getControl().p.x == getCentro().p.x + getRaioHorizontal() ? (1):(-1);
    GLint ay = getControl().p.y == getCentro().p.y + getRaioVertical() ? (1):(-1);
    setCentro(Ponto(mouseX - xclick, mouseY - yclick));
    setControl(Ponto(getCentro().p.x + ax * (getRaioHorizontal()), getCentro().p.y + ay * (getRaioVertical())));
}

void Elipse::desseleciona() {
    this->selecionado = false;
    this->control.selecionado = false;
}

Ponto Elipse::boundsMax() {
    return Ponto(centro.x + raioHorizontal, centro.y + raioVertical);
}

Ponto Elipse::boundsMin() {
    return Ponto(centro.x - raioHorizontal, centro.y - raioVertical);
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

    glPointSize(espessuraLinha);

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
                glVertex2i( intXView( x+centro.x), intYView( y+centro.y));
                glVertex2i( intXView( x+centro.x), intYView(-y+centro.y));
                glVertex2i( intXView(-x+centro.x), intYView( y+centro.y));
                glVertex2i( intXView(-x+centro.x), intYView(-y+centro.y));
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
                glVertex2i(intXView( x+centro.x), intYView( y+centro.y));
                glVertex2i(intXView( x+centro.x), intYView(-y+centro.y));
                glVertex2i(intXView(-x+centro.x), intYView( y+centro.y));
                glVertex2i(intXView(-x+centro.x), intYView(-y+centro.y));
            glEnd();
        }

        linha_i = (linha_i + 1) % estiloLinha.size();
    }
}

void Elipse::desenhaFill() {
    GLfloat co[4];
    getColorFill(co);
    glColor4f( co[0],co[1],co[2], co[3]);
    glPointSize(1);

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
            linhaFill(pView(Ponto(-x + centro.x,  y + centro.y)),
                      pView(Ponto( x + centro.x,  y + centro.y)));
            linhaFill(pView(Ponto(-x + centro.x, -y + centro.y)),
                      pView(Ponto( x + centro.x, -y + centro.y)));
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

        linhaFill(pView(Ponto(-x + centro.x,  y + centro.y)),
                  pView(Ponto( x + centro.x,  y + centro.y)));
        linhaFill(pView(Ponto(-x + centro.x, -y + centro.y)),
                  pView(Ponto( x + centro.x, -y + centro.y)));
    }
}

void Elipse::desenhaControles() {
   glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xAAAA);
    glColor3f(0,0.5, 0);
    glBegin(GL_LINES);
         glVertex2i(intXView(-getRaioHorizontal()+getCentro().p.x), intYView( getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView(-getRaioHorizontal()+getCentro().p.x), intYView(-getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView( getRaioHorizontal()+getCentro().p.x), intYView( getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView( getRaioHorizontal()+getCentro().p.x), intYView(-getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView(-getRaioHorizontal()+getCentro().p.x), intYView( getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView( getRaioHorizontal()+getCentro().p.x), intYView( getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView( getRaioHorizontal()+getCentro().p.x), intYView(-getRaioVertical()+getCentro().p.y));
         glVertex2i(intXView(-getRaioHorizontal()+getCentro().p.x), intYView(-getRaioVertical()+getCentro().p.y));
    glEnd();

    glDisable(GL_LINE_STIPPLE);
    glPointSize(8);
    glBegin(GL_POINTS);
        glVertex2i(intXView(getCentro().p.x),  intYView(getCentro().p.y));
        glVertex2i(intXView(getControl().p.x), intYView(getControl().p.y));
    glEnd();

    /*
    glPointSize(1);
    glColor3f( 0,0, 0 );
    glBegin(GL_LINE_LOOP);
        glVertex2i(e->getControl().p.x+CONTROL, e->getControl().p.y+CONTROL);
        glVertex2i(e->getControl().p.x-CONTROL, e->getControl().p.y+CONTROL);
        glVertex2i(e->getControl().p.x-CONTROL, e->getControl().p.y-CONTROL);
        glVertex2i(e->getControl().p.x+CONTROL, e->getControl().p.y-CONTROL);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2i(e->getCentro().p.x+CONTROL, e->getCentro().p.y+CONTROL);
        glVertex2i(e->getCentro().p.x-CONTROL, e->getCentro().p.y+CONTROL);
        glVertex2i(e->getCentro().p.x-CONTROL, e->getCentro().p.y-CONTROL);
        glVertex2i(e->getCentro().p.x+CONTROL, e->getCentro().p.y-CONTROL);

   glEnd();
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0xAAAA);
   glBegin(GL_LINES);
        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x,e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);
        glVertex2i(-e->getRaioHorizontal()+e->getCentro().p.x, -e->getRaioVertical()+e->getCentro().p.y);

   glEnd();
   glDisable(GL_LINE_STIPPLE);
   */
}
