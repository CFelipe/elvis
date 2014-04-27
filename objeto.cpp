#include "objeto.h"

Objeto::Objeto(GLfloat colorFill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo) {
    selecionado = false;
    this->colorfill[0] = colorFill[0];
    this->colorfill[1] = colorFill[1];
    this->colorfill[2] = colorFill[2];
    this->colorfill[3] = colorFill[3];
    this->colorLine[0] = colorLine[0];
    this->colorLine[1] = colorLine[1];
    this->colorLine[2] = colorLine[2];
    this->colorLine[3] = colorLine[3];
    this->tipo = tipo;
    this->espessuraLinha = espessuraLinha;
    preenchido = false;
}
