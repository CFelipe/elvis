#include "ponto.h"

Ponto::Ponto(GLint x, GLint y) {
    this->x = x;
    this->y = y;
}

Ponto Ponto::operator+(const Ponto& rhs) {
    return Ponto(x + rhs.x, y + rhs.y);
}
