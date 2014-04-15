#ifndef CIRCULO_H
#define CIRCULO_H
#include "Objeto.h"

class Circulo: public Objeto{
    private:
        int raio, xc, yc;
    public:
        Circulo(int raio, int xc, int yc, float color[4], int tipo) : Objeto(color, tipo){
            this->raio = raio;
            this->xc = xc;
            this->yc = yc;
        }
    int getRaio(){
        return raio;
    }
    void setRaio(int r){
        raio = r;
    }
    int getXc(){
        return xc;
    }
    void setXc(int r){
        xc = r;
    }
    int getYc(){
        return yc;
    }
    void setYc(int r){
        yc = r;
    }
};

#endif // CIRCULO_H
