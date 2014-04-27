#ifndef CIRCULO_H
#define CIRCULO_H

class Circulo: public Objeto {
    public:
        Circulo(GLint raio, GLint xc, GLint yc, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo);

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
        void desenha();

    private:
        GLint raio, xc, yc;
};


#endif // CIRCULO_H
