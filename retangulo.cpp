#include "retangulo.h"
#include "vertice.h"
#include "objeto.h"

Retangulo::Retangulo(Vertice A, Vertice B, Vertice C, Vertice D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha) : Objeto(colorfill, colorLine, espessuraLinha, Objeto::RETANGULO) {
    this->A = Vertice(A.p.x, A.p.y);
    this->B = Vertice(B.p.x, B.p.y);
    this->C = Vertice(C.p.x, C.p.y);
    this->D = Vertice(D.p.x, D.p.y);
    Vertice m(A.p.x, A.p.y);
    min = m;
    max = m;
    Vertice c((max.p.x-min.p.x)/2 + min.p.x, (max.p.y- min.p.y)/2 + min.p.y);
    this->centro = c;
    this->tipo = Objeto::RETANGULO;
}

void Retangulo::desenha() {
    Bresenham(A, B);
    Bresenham(B, D);
    Bresenham(D, C);
    Bresenham(C, A);
}

// Deveria ser ponto, mas mudo isso depois
void Retangulo::Bresenham(Vertice p1, Vertice p2) {
    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);

    // Especifica o diâmetro do Vertice
    glPointSize(getEspessuraLinha());

    GLint x, y;
    if (p2.p.x!=p1.p.x && p2.p.y!=p1.p.y){
        GLfloat m = (GLfloat) (p2.p.y-p1.p.y) / (p2.p.x-p1.p.x);
        if (m>0  && m<1){

            GLint dx = p2.p.x - p1.p.x;
            GLint dy = p2.p.y - p1.p.y;
            GLint pk = 2*dy - dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;
            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.x<p2.p.x){
                while (x<p2.p.x){

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
                while (x>p2.p.x){

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
            GLint dx = p2.p.x - p1.p.x;
            GLint dy = - p2.p.y + p1.p.y;
            GLint pk = 2*dy + dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;

            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.x<p2.p.x){
                while (x<p2.p.x){
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
                while (x>p2.p.x){
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
            GLint dx = p2.p.x - p1.p.x;
            GLint dy = p2.p.y - p1.p.y;
            GLint pk = 2*dx + dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y<p2.p.y){
                while (y<p2.p.y){
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
                while (y>p2.p.y){
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
            GLint dx = p2.p.x - p1.p.x;
            GLint dy = -p2.p.y + p1.p.y;
            GLint pk = 2*dx - dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.p.x;
            y = p1.p.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y>p2.p.y){
                while (y>p2.p.y){
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
                while (y<p2.p.y){
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
            x = p1.p.x;
            y = (GLfloat) p1.p.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y<p2.p.y){
                while (y<p2.p.y){
                    x++;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.p.y){
                    x--;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m==-1){
            x = p1.p.x;
            y = (GLfloat) p1.p.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.p.y>p2.p.y){
                while (y>p2.p.y){
                    x++;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.p.y){
                    x--;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        }
    } else if (p2.p.y==p1.p.y){ //se a linha for horizontal:

        x = p1.p.x;
        y = (GLfloat) p1.p.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.p.x<p2.p.x){
            while (x<p2.p.x){
                x++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (x>p2.p.x){
                x--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    } else { // se a linha for vertical:
        x = p1.p.x;
        y = (GLfloat) p1.p.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.p.y<p2.p.y){
            while (y<p2.p.y){
                y++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (y>p2.p.y){
                y--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    }
}
