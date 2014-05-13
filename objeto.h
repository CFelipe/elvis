#ifndef OBJETO_H
#define OBJETO_H

#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>
#include "vertice.h"

enum Forma {CIRCULO,
            RETANGULO,
            ELIPSE,
            LINHA,
            POLILINHA};

class Objeto {
    public:

        Objeto(Forma tipo);

        int id;
        Forma tipo;
        GLfloat colorfill[4];
        GLfloat colorLine[4];
        GLint xclick, yclick; // Tirar!
        GLint espessuraLinha;
        QString estiloLinha;
        bool selecionado;
        bool preenchido;
        bool linha;

        void desenha();
        void virtual desenhaLinha() = 0;
        void virtual desenhaFill() = 0;
        void virtual desenhaControles() = 0;

        void virtual translada(GLint mouseX, GLint mouseY) = 0;

        void virtual desseleciona() = 0;

        Ponto virtual boundsMin() = 0;
        Ponto virtual boundsMax() = 0;

        void Bresenham(Ponto p1, Ponto p2);
        void linhaFill(Ponto p1, Ponto p2);

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

        static Ponto pView(Ponto p);
        static int intXView(GLint i);
        static int intYView(GLint i);

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

    private:
        // Incrementador de ID
        static int nId;
};

#endif // OBJETO_H
