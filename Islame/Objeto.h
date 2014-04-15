#ifndef OBJETO_H
#define OBJETO_H

#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>


enum Forma {CIRCULO, QUADRILATERO, ELIPSE, LINHA, POLILINHA};

class Objeto{
    private:
        Forma tipo;
        bool select;
        GLfloat colorfill[4];
        GLfloat colorLine[4];
        GLint xclick, yclick;
        GLint espessuraLinha;
    public:
        Objeto(GLfloat colorFill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo){ //
            select = false;
            this->colorfill[0] = colorFill[0];
            this->colorfill[1] = colorFill[1];
            this->colorfill[2] = colorFill[2];
            this->colorfill[3] = colorFill[3];
            this->colorLine[0] = colorLine[0];
            this->colorLine[1] = colorLine[1];
            this->colorLine[2] = colorLine[2];
            this->colorLine[3] = colorLine[3];
            this->tipo = tipo;
            this->espessuraLinha = espessuraLinha;

        }
        Objeto(){}
        void virtual redimensionar(GLint xmouse, GLint ymouse)=0;
       // void virtual getDimensao() = 0;
        GLint getEspessuraLinha(){
            return espessuraLinha;
        }
        void setEspessuraLinha(GLint v){
            espessuraLinha = v;
        }
        bool isSelect(){
              return select;
        }
        void setSelect(bool op){
               select = op;
        }
        void setColorFill(GLfloat R, GLfloat G, GLfloat B, GLfloat A){
            colorfill[0] = R;
            colorfill[1] = G;
            colorfill[2] = B;
            colorfill[3] = A;
        }
        void getColorFill(GLfloat res[4]){
            res[0] = colorfill[0];
            res[1] = colorfill[1];
            res[2] = colorfill[2];
            res[3] = colorfill[3];
        }
        void setColorLine(GLfloat R, GLfloat G, GLfloat B, GLfloat A){
            colorLine[0] = R;
            colorLine[1] = G;
            colorLine[2] = B;
            colorLine[3] = A;
        }
        void getColorLine(GLfloat res[4]){
            res[0] = colorLine[0];
            res[1] = colorLine[1];
            res[2] = colorLine[2];
            res[3] = colorLine[3];
        }
        void setXClick(GLint n){
            xclick = n;
        }
        void setYClick(GLint n){
            yclick = n;
        }
        GLint getXClick(){
            return xclick ;
        }
        GLint getYClick(){
            return yclick;
        }
        Forma getTipo(){
              return tipo;
        }
        void virtual desenha() = 0;


};


class Circulo: public Objeto{
    private:
        GLint raio, xc, yc;
    public:
        Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo) : Objeto(colorfill, colorLine, espessuraLinha, tipo){
            this->raio = raio;
            this->xc = xc;
            this->yc = yc;
        }
        void redimensionar(GLint xmouse, GLint ymouse){
            raio =  sqrt(pow((xmouse - xc), 2)+ pow((ymouse - yc), 2));
            this->raio = raio;
            this->xc = xc;
            this->yc = yc;
        }

        GLint getXc() {
            return xc;
        }
        GLint getYc() {
            return yc;
        }
        GLint getRaio() {
            return raio;
        }
        GLint setXc(GLint n) {
            xc = n;
        }
        GLint setYc(GLint n) {
            yc = n;
        }

    //! Algoritmo de rasterização do circulo
    void desenha(){
        glPointSize(getEspessuraLinha());
        GLfloat co[4];
        getColorLine(co);
        glColor4f( co[0],co[1],co[2], co[3]);
        GLint x = 0;
        GLint  y = raio;
        GLint  d = 1 - raio;
        GLint  dL = 3;
        GLint  dse = -2*raio + 5;
        glBegin(  GL_POINTS );
            glVertex2i( x+xc, y+yc );
            glVertex2i( x+xc, -y+yc );
            glVertex2i( -x+xc, y+yc );
            glVertex2i( -x+xc, -y+yc );
            glVertex2i( y+xc, x+yc );
            glVertex2i( y+xc, -x+yc );
            glVertex2i( -y+xc, x+yc );
            glVertex2i( -y+xc, -x+yc );
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
            glBegin(  GL_POINTS);
                glVertex2i( x+xc, y+yc );
                glVertex2i( x+xc, -y+yc );
                glVertex2i( -x+xc, y+yc );
                glVertex2i( -x+xc, -y+yc );
                glVertex2i( y+xc, x+yc );
                glVertex2i( y+xc, -x+yc );
                glVertex2i( -y+xc, x+yc );
                glVertex2i( -y+xc, -x+yc );
            glEnd( );

        }
    }

};

class Ponto{
    private:
        GLint x, y;
    public:
        Ponto(GLint x, GLint y){
            this->x = x;
            this->y = y;

        }
        Ponto(){};
        GLint getX(){
            return x;
        }
        GLint getY(){
            return y;
        }
        void setY(GLint y) {
            this->y = y;
        }
        void setX(GLint x){
            this->x = x;
        }

};


class Quadrilatero: public Objeto{
    private:
        Ponto A; // âncora, isto é, vértice de onde partirá o processo de rasterização
        Ponto B, C, D;
    public:
        Quadrilatero(Ponto A, Ponto B, Ponto C, Ponto D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo) : Objeto(colorfill, colorLine, espessuraLinha, tipo){
            this->A = A;
            this->B = B;
            this->C = C;
            this->D = D;
        }
    private:
        //! Algortimo de rasterização da linha: Bresenham
        void Bresenham(Ponto p1, Ponto p2){
            GLfloat co[4];
            getColorLine(co);
            glColor4f( co[0],co[1],co[2], co[3]);
            glPointSize(getEspessuraLinha()); //especifica o diâmetro do ponto

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
     public:
        void desenha(){
            Bresenham(A, B);
            Bresenham(B, D);
            Bresenham(D, C);
            Bresenham(C, A);
        }
        void redimensionar(GLint xmouse, GLint ymouse){
            D.setX(xmouse);
            D.setY(ymouse);
            B.setX(D.getX());
            B.setY(A.getY());
            C.setX(A.getX());
            C.setY(D.getY());
        }
       Ponto getA(){
            return A;
       }
       Ponto getB(){
            return B;
       }
       Ponto getC(){
            return C;
       }
       Ponto getD(){
            return D;
       }
       Ponto setA(Ponto A){
           this->A = A;
       }
       Ponto setB(Ponto B){
           this->B = B;
       }
       Ponto setC(Ponto C){
           this->C = C;
       }
       Ponto setD(Ponto D){
           this->D = D;
       }
};

#endif // OBJETO_H
