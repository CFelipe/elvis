#ifndef CIRCULO_H
#define CIRCULO_H

#include <QtOpenGL>
#include "objeto.h"

class Circulo: public Objeto {
    public:
        Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha);

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

        // Algoritmo de rasterização do circulo
        void desenha();

        void redimensionar(GLint xmouse, GLint ymouse);

    private:
        GLint raio, xc, yc;
};


#endif // CIRCULO_H
