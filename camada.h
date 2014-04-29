#ifndef CAMADA_H
#define CAMADA_H

#include "objeto.h"
#include <QString>
#include <QList>

class Camada {
    public:
        Camada(QString nome);
        QString titulo;
        QList<Objeto*>* objetos;
};

#endif // CAMADA_H
