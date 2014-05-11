#ifndef ELIPSE_H
#define ELIPSE_H

#include "objeto.h"
#include "vertice.h"
#include "ponto.h"

class Elipse : public Objeto {
    public:
        Elipse(Ponto centro, GLint raioHorizontal, GLint raioVertical, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, bool linha, bool preenchido);

        Ponto centro;
        Vertice control;
        GLint raioHorizontal, raioVertical;

        void desenhaFill();
        void desenhaLinha();
        void desenhaControles();

        void translada(GLint mouseX, GLint mouseY);

        void desseleciona();

        Ponto boundsMax();
        Ponto boundsMin();

        Vertice getCentro(){
            return centro;
        }

        GLint getRaioHorizontal(){
            return raioHorizontal;
        }

        GLint getRaioVertical(){
            return raioVertical;
        }

        Vertice getControl(){
            return control;
        }

        Vertice *getPControl(){
            return &control;
        }

        void setCentro(Vertice c){
            centro = c.p;
        }
        void setRaioHorizontal(GLint n){
            raioHorizontal = n;
        }

        void setRaioVertical(GLint n){
            raioVertical = n;
        }

        void setControl(Vertice c){
            c.selecionado = this->control.selecionado;
            this->control = c;
        }

};

#endif // ELIPSE_H
