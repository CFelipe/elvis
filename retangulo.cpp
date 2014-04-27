#include "vertice.h"
#include "retangulo.h"

Retangulo::Retangulo(Vertice A, Vertice B, Vertice C, Vertice D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, tipo) {
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
    Vertice m(A.getX(), A.getY());
    min = m;
    max = m;
    Vertice c((max.getX()-min.getX())/2 + min.getX(), (max.getY()- min.getY())/2 + min.getY());
    this->centro = c;
    this->tipo = Objeto::RETANGULO;
}

void Retangulo::desenha() {
    Bresenham(A, B);
    Bresenham(B, D);
    Bresenham(D, C);
    Bresenham(C, A);
}

void Retangulo::Bresenham(Ponto p1, Ponto p2) {
    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);

    // Especifica o diâmetro do Vertice
    glPointSize(getEspessuraLinha());

    GLint x, y;
    if (p2.getX()!=p1.getX() && p2.getY()!=p1.getY()){
        GLfloat m = (GLfloat) (p2.getY()-p1.getY()) / (p2.getX()-p1.getX());
        if (m>0  && m<1){

            GLint dx = p2.getX() - p1.getX();
            GLint dy = p2.getY() - p1.getY();
            GLint pk = 2*dy - dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;
            x = p1.getX();
            y = p1.getY();
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.getX()<p2.getX()){
                while (x<p2.getX()){

                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (x>p2.getX()){

                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m>-1 && m<0){
            printf("%f\n", m);
            GLint dx = p2.getX() - p1.getX();
            GLint dy = - p2.getY() + p1.getY();
            GLint pk = 2*dy + dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;

            x = p1.getX();
            y = p1.getY();
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.getX()<p2.getX()){
                while (x<p2.getX()){
                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y--;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (x>p2.getX()){
                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y++;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m>1){
            GLint dx = p2.getX() - p1.getX();
            GLint dy = p2.getY() - p1.getY();
            GLint pk = 2*dx + dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.getX();
            y = p1.getY();
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.getY()<p2.getY()){
                while (y<p2.getY()){
                    y++;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.getY()){
                    y--;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m<-1){
            GLint dx = p2.getX() - p1.getX();
            GLint dy = -p2.getY() + p1.getY();
            GLint pk = 2*dx - dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.getX();
            y = p1.getY();
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.getY()>p2.getY()){
                while (y>p2.getY()){
                    y--;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.getY()){
                    y++;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }

        } else if (m==1){
            x = p1.getX();
            y = (GLfloat) p1.getY();
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.getY()<p2.getY()){
                while (y<p2.getY()){
                    x++;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.getY()){
                    x--;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m==-1){
            x = p1.getX();
            y = (GLfloat) p1.getY();
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.getY()>p2.getY()){
                while (y>p2.getY()){
                    x++;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.getY()){
                    x--;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        }
    } else if (p2.getY()==p1.getY()){ //se a linha for horizontal:

        x = p1.getX();
        y = (GLfloat) p1.getY();
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.getX()<p2.getX()){
            while (x<p2.getX()){
                x++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (x>p2.getX()){
                x--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    } else { // se a linha for vertical:
        x = p1.getX();
        y = (GLfloat) p1.getY();
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.getY()<p2.getY()){
            while (y<p2.getY()){
                y++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (y>p2.getY()){
                y--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    }
}
