#ifndef PONTO_H
#define PONTO_H

#include <QtOpenGL>

class Ponto {
    public:
        Ponto() {};
        Ponto(GLint x, GLint y);
        GLint x;
        GLint y;

        Ponto operator+(const Ponto& rhs);
};

#endif // PONTO_H
