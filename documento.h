#ifndef DOCUMENTO_H
#define DOCUMENTO_H

#include "objeto.h"
#include "circulo.h"
#include "elipse.h"
#include "ponto.h"
#include "vertice.h"
#include "retangulo.h"
#include "camada.h"

enum Operacao {SELECIONAR,
               CRIACAO,
               CRIACAO_POLILINHA,
               TRANSLACAO,
               COPIA,
               ESCALA,
               DESLOCARPONTOS,
               ROTACAO,
               INSERT_REMOVE_PONTO,
               AGRUPAMENTO};

/* Contém todo o estado de um documento aberto */
class Documento {
    public:
        Documento();

        Operacao op;
        Forma forma;

        QList<Camada*> camadas;
        Camada* camadaSelecionada;
        GLfloat linhaColorSelecionada[4];
        GLfloat fillColorSelecionada[4];
        GLint espessuraLinha;
        bool preenchimento;
        bool linha;
        bool grade;
        Ponto viewport;
        GLint canvasW;
        GLint canvasH;

        bool desenha;
        bool desenhandoPolilinha;

        QString arqAberto;
        GLint gradeSep;

        void desselecionarTodos();
        void selecionarTodos();
};

#endif // DOCUMENTO_H
