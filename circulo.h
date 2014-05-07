#ifndef CIRCULO_H
#define CIRCULO_H

#include <QtOpenGL>
#include "objeto.h"

class Circulo: public Objeto {
    public:
        Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, bool linha, bool preenchido);

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
            return xc;
        }
        GLint setYc(GLint n) {
            yc = n;
            return yc;
        }

        // Algoritmo de rasterização do circulo
        void desenhaFill();
        void desenhaLinha();
        void redimensionar(GLint xmouse, GLint ymouse);

    private:
        GLint raio, xc, yc;
};


#endif // CIRCULO_H
