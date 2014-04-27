#ifndef RETANGULO_H
#define RETANGULO_H

class Retangulo : Objeto {
    public:
        Retangulo(Ponto A, Ponto B, Ponto C, Ponto D , GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo);

    private:
        Vertice A, B, C, D, centro, max, min;

        void Bresenham(Vertice v1, Vertice v2);
};

#endif // RETANGULO_H
