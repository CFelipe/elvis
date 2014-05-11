#ifndef RETANGULO_H
#define RETANGULO_H

#include "objeto.h"
#include "vertice.h"
#include "ponto.h"

/*
 * O retângulo é definido por quatro vértices no sentido anti-horário:
 *
 * A-----------D
 * |           |
 * |           |
 * B-----------C
 */

class Retangulo : public Objeto {
    public:
        Retangulo(Vertice A, Vertice B, Vertice C, Vertice D);

        Vertice A, B, C, D, centro;
        Ponto max, min;
        bool isSeletor;
        bool isVisible;
        /* A variável is visible serve ao agrupamento de formas. Na verdade,
         * quando agrupamos as formas, devemos ter um Retângulo imaginário o tamanho necessário e suficiente para acomodar as formas
         * Tal retângulo será um objeto do tipo Retangulo invisível (ou melhor, apenasos vértices de controle aparecem);
         */
        void desenhaFill();
        void desenhaLinha();
        void desenhaControles();

        void translada(GLint mouseX, GLint mouseY);
        void escala(GLdouble fatorx, GLdouble fatory);

        void desseleciona();

        Ponto boundsMax();
        Ponto boundsMin();

        void Bresenham(Vertice p1, Vertice p2);
        void atualizaMINMAX();

        void setA(Vertice A){
           A.selecionado = (this->A.selecionado);
           this->A = A;
           atualizaMINMAX();
       }

       void setB(Vertice B){
           B.selecionado = (this->B.selecionado);
           this->B = B;
           atualizaMINMAX();

       }

       void setC(Vertice C){
           C.selecionado = (this->C.selecionado);
           this->C = C;
           atualizaMINMAX();
       }

       void setD(Vertice D){
           D.selecionado = (this->D.selecionado);
           this->D = D;
           atualizaMINMAX();
       }
};

#endif // RETANGULO_H
