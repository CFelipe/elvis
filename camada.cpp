#include "camada.h"

Camada::Camada(QString titulo) {
    this->titulo = titulo;
    this->objetos = new QList<Objeto*>();
}
