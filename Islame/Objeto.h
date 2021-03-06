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
        bool preenchido; // true  -> a forma está preenchida com alguma cor
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
            preenchido = false;

        }
        Objeto(){}
       // void virtual getDimensao() = 0;

        bool isPreenchido(){
            return preenchido;
        }

        void setPreenchido(bool pr){
           preenchido = pr;
        }

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

class Ponto{
    private:
        GLint x, y;
        bool select;
    public:
        Ponto(GLint x, GLint y){
            this->x = x;
            this->y = y;
            select = false;
        }
        Ponto(){};
        bool isSelect(){
            return select;
        }
        void setSelect(bool s){
            select = s;
        }

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


/*
 * Criamos uma classe Circulo e outra Elipse.
 * Poderíamos tratar tudo como elipse, porém,
 * de fato, o usuário pode ter a necessidade de desenhar exatamente um circulo.
 * Esta forma, quando editada com o deslocamento de pontos ou escala, continuará um circulo
 * A elipse, por sua vez, pode virar um circulo, caso o raio vertical seja igual ao do horizontal (deslocamento de pontos)
*/
class Elipse: public Objeto{
    private:
        Ponto centro;
        GLint raioHorizontal, raioVertical;
        Ponto control;
        /* control.x  = centro.x + raioHorizontal
         * control.y = centro.y - raioVertical
         */


    public:

        Elipse(Ponto centro, GLint raioHorizontal, GLint raioVertical, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo) : Objeto(colorfill, colorLine, espessuraLinha, tipo){
            this->centro = centro;
            this->raioHorizontal = raioHorizontal;
            this->raioVertical = raioVertical;
            this->control = centro;
        }
        void desenha(){
            GLfloat co[4];
            getColorLine(co);
            glColor4f( co[0],co[1],co[2], co[3]);

            GLdouble d2;
            GLint x =0;
            GLint y = raioVertical;
            GLdouble d1 = raioVertical*raioVertical - (raioHorizontal*raioHorizontal*raioVertical) + (0.25*raioHorizontal*raioHorizontal);

            glPointSize(getEspessuraLinha());

            glBegin(GL_POINTS);
              //  glVertex2i(x+centro.getX(), y+centro.getY());
                glVertex2i(  x+centro.getX(),  y+centro.getY() );
                glVertex2i(  x+centro.getX(), -y+centro.getY() );
                glVertex2i( -x+centro.getX(),  y+centro.getY() );
                glVertex2i( -x+centro.getX(), -y+centro.getY() );
            glEnd();
            while (raioHorizontal*raioHorizontal*(y-0.5) > raioVertical*raioVertical*(x+1)){
                if (d1<0){
                    d1 += raioVertical*raioVertical*(2*x+3);
                }else{
                    d1 += raioVertical*raioVertical*(2*x+3) + raioHorizontal*raioHorizontal*((-2)*y +2);
                    y--;
                }
                x++;
                glBegin(GL_POINTS);
                    glVertex2i(  x+centro.getX(),  y+centro.getY() );
                    glVertex2i(  x+centro.getX(), -y+centro.getY() );
                    glVertex2i( -x+centro.getX(),  y+centro.getY() );
                    glVertex2i( -x+centro.getX(), -y+centro.getY() );
                glEnd();
            }
            d2 = raioVertical*raioVertical*(x+0.5)*(x+0.5) + raioHorizontal*raioHorizontal*(y-1)*(y-1) - raioHorizontal*raioHorizontal*raioVertical*raioVertical;
            while (y>0){
                if (d2<0){
                    d2+=raioVertical*raioVertical*(2*x+2) + raioHorizontal*raioHorizontal*((-2)*y + 3);
                    x++;
                } else {
                    d2+=raioHorizontal*raioHorizontal*((-2)*y + 3);
                }
                y--;
                glBegin(GL_POINTS);
                    glVertex2i(  x+centro.getX(),  y+centro.getY() );
                    glVertex2i(  x+centro.getX(), -y+centro.getY() );
                    glVertex2i( -x+centro.getX(),  y+centro.getY() );
                    glVertex2i( -x+centro.getX(), -y+centro.getY() );
                glEnd();
            }
        }

        Ponto getCentro(){
            return centro;
        }
        GLint getRaioHorizontal(){
            return raioHorizontal;
        }
        GLint getRaioVertical(){
            return raioVertical;
        }
        Ponto getControl(){
            return control;
        }
        Ponto *getPControl(){
            return &control;
        }

        void setCentro(Ponto c){
            centro = c;
           // updateControl();
        }
        void setRaioHorizontal(GLint n){
            raioHorizontal = n;
           // updateControl();
        }
        void setRaioVertical(GLint n){
            raioVertical = n;
          //  updateControl();
        }
        void setControl(Ponto c){
            c.setSelect(this->control.isSelect());
            this->control = c;
        }


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



class Quadrilatero: public Objeto{
    private:
        Ponto A;
        Ponto B, C, D;
        Ponto centro;
        Ponto max, min;
        /*
         * Mantemos A, B, C, D para facilitar o processo de rasterização dos lados, bem como manipulação dos pontos de controle
         * Mesmo assim, é necessário manter os pontos max e min, pois não sabemos quais deles serão max e min
         * max e min serão atualizados sempre que A B C ou D também o forem
        */
    public:
        Quadrilatero(Ponto A, Ponto B, Ponto C, Ponto D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo) : Objeto(colorfill, colorLine, espessuraLinha, tipo){
            this->A = A;
            this->B = B;
            this->C = C;
            this->D = D;
            Ponto m(A.getX(), A.getY());
            min = m;
            max = m;
            Ponto c((max.getX()-min.getX())/2 + min.getX(), (max.getY()- min.getY())/2 + min.getY());
            this->centro = c;
        }
    private:

        //! Algortimo de rasterização da linha: Bresenham
        void Bresenham(Ponto p1, Ponto p2){
            GLfloat co[4];
            getColorLine(co);
            glColor4f( co[0],co[1],co[2], co[3]);
            /*glPointSize(getEspessuraLinha()+10); //especifica o diâmetro do ponto
            glBegin(GL_POINTS);
                glVertex2i(centro.getX(), centro.getY());
            glEnd();
            glPointSize(getEspessuraLinha()+5); //especifica o diâmetro do ponto

            glBegin(GL_POINTS);
                glVertex2i(A.getX(), A.getY());
            glEnd();

            glPointSize(getEspessuraLinha()+15); //especifica o diâmetro do ponto

            glBegin(GL_POINTS);
                glVertex2i(D.getX(), D.getY());
            glEnd();
            */

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

       Ponto getCentro(){
           return centro;
       }

       Ponto getA(){
            return A;
       }
       Ponto *getPA(){
           return &A;
       }

       Ponto getB(){
            return B;
       }
       Ponto *getPB(){
            return &B;
       }
       Ponto getC(){
            return C;
       }
       Ponto *getPC(){
            return &C;
       }
       Ponto getD(){
            return D;
       }
       Ponto *getPD(){
            return &D;
       }
       void atualizaMINMAX(Ponto A){

           Ponto c((max.getX()-min.getX())/2 + min.getX(), (max.getY()- min.getY())/2 + min.getY());
           this->centro = c;
       }

       Ponto setA(Ponto A){
           A.setSelect(this->A.isSelect());
           this->A = A;

           if (A.getX()>D.getX()){
               max.setX(A.getX());
               min.setX(D.getX());
           } else{
               max.setX(D.getX());
               min.setX(A.getX());
           }
           if (A.getY()>D.getY()){
               max.setY(A.getY());
               min.setY(D.getY());
           } else{
               max.setY(D.getY());
               min.setY(A.getY());
           }
           Ponto c((max.getX()-min.getX())/2 + min.getX(), (max.getY()- min.getY())/2 + min.getY());
           this->centro = c;
       }
       Ponto setB(Ponto B){
           B.setSelect(this->B.isSelect());
           this->B = B;
           //atualizaMINMAX(B);

       }
       Ponto setC(Ponto C){
           C.setSelect(this->C.isSelect());
           this->C = C;
           //atualizaMINMAX(C);

       }
       Ponto setD(Ponto D){
           D.setSelect(this->D.isSelect());
           this->D = D;
           //atualizaMINMAX(D);
           if (A.getX()>D.getX()){
               max.setX(A.getX());
               min.setX(D.getX());
           } else{
               max.setX(D.getX());
               min.setX(A.getX());
           }
           if (A.getY()>D.getY()){
               max.setY(A.getY());
               min.setY(D.getY());
           } else{
               max.setY(D.getY());
               min.setY(A.getY());
           }

           Ponto c((max.getX()-min.getX())/2 + min.getX(), (max.getY()- min.getY())/2 + min.getY());
           this->centro = c;
       }
       void escala(GLint fatorx, GLint fatory){
           /*if (A.getX()==max.getX()){
               A.setX(A.getX()+fatorx);
               D.setX(D.getX()-fatorx);
           } else{
               A.setX(A.getX()-fatorx);
               D.setX(D.getX()+fatorx);
           }
           if (A.getY()==max.getY()){
               A.setY(A.getY()+fatory);
               D.setY(D.getY()-fatory);
           } else{
               A.setY(A.getY()-fatory);
               D.setY(D.getY()+fatory);
           }*/
           if (A.getX()>D.getX()){
               A.setX(A.getX()+fatorx);
               D.setX(D.getX()-fatorx);
           } else{
               D.setX(D.getX()+fatorx);
               A.setX(A.getX()-fatory);
           }
           if (A.getY()>D.getY()){
               A.setY(A.getY()+fatory);
               D.setY(D.getY()-fatory);
           } else{
               D.setY(D.getY()+fatory);
               A.setY(A.getY()-fatory);
           }
           B.setX(A.getX());
           B.setY(D.getY());
           C.setX(D.getX());
           C.setY(A.getY());

       }
};



#endif // OBJETO_H
