#ifndef OBJETO_H
#define OBJETO_H

#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>

class Objeto {
    public:
        enum Forma {CIRCULO, RETANGULO, ELIPSE, LINHA, POLILINHA};

        Objeto(GLfloat colorFill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo);

        Forma tipo;
        bool selecionado;
        GLfloat colorfill[4];
        GLfloat colorLine[4];
        GLint xclick, yclick;
        GLint espessuraLinha;
        bool preenchido;
        int id;

         GLint getEspessuraLinha(){
            return espessuraLinha;
        }

        void setEspessuraLinha(GLint v){
            espessuraLinha = v;
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

    private:
        static int nId;
};

#endif // OBJETO_H
