#include "ponto.h"

Ponto::Ponto(GLint x, GLint y) {
    GLint x;
    GLint y;
}

Ponto::Ponto operator+(Ponto lhs, const Ponto& rhs) {
    return Ponto(lhs.x + rhs.x, lhs.y + rhs.y);
}
