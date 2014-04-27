#include <QtOpenGL>

#ifndef PONTO_H
#define PONTO_H

class Ponto {
    public:
        Ponto(GLint x, GLint y);
        GLint x;
        GLint y;
};

#endif // PONTO_H
