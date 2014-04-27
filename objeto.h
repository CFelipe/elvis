#ifndef OBJETO_H
#define OBJETO_H

#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>

struct Ponto {
    GLint x;
    GLint y;
};

class Objeto {
    public:
        enum Forma {CIRCULO, RETANGULO, ELIPSE, LINHA, POLILINHA};

        Objeto(GLfloat colorFill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo);

        Objeto(){}

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
              return selecionado;
        }

        void setSelect(bool op){
               selecionado = op;
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
        Forma tipo;
        bool selecionado;
        GLfloat colorfill[4];
        GLfloat colorLine[4];
        GLint xclick, yclick;
        GLint espessuraLinha;
        bool preenchido;
};

/*
 * Criamos uma classe Circulo e outra Elipse.
 * Poderíamos tratar tudo como elipse, porém,
 * de fato, o usuário pode ter a necessidade de desenhar exatamente um circulo.
 * Esta forma, quando editada com o deslocamento de Vertices ou escala, continuará um circulo
 * A elipse, por sua vez, pode virar um circulo, caso o raio vertical seja igual ao do horizontal (deslocamento de Vertices)
*/

#endif // OBJETO_H
