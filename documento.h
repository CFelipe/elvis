#ifndef DOCUMENTO_H
#define DOCUMENTO_H

#include "objeto.h"
#include "circulo.h"
#include "elipse.h"
#include "linha.h"
#include "ponto.h"
#include "vertice.h"
#include "retangulo.h"
#include "camada.h"

enum Operacao_2 {DOIS_SELECIONAR,
                 DOIS_CRIACAO,
                 DOIS_CRIACAO_POLILINHA,
                 DOIS_TRANSLACAO,
                 DOIS_COPIA,
                 DOIS_ESCALA,
                 DOIS_DESLOCARPONTOS,
                 DOIS_ROTACAO,
                 DOIS_INSERT_REMOVE_PONTO,
                 DOIS_AGRUPAMENTO};

/* Contém todo o estado de um documento aberto */
class Documento {
    public:
        Documento();

        Operacao_2 op;
        QList<Camada*> camadas;
        Camada* camadaSelecionada;
        GLfloat linhaColorSelecionada[4];
        GLfloat fillColorSelecionada[4];
        GLint espessuraLinha;
        bool preenchimento;
        bool linha;
        bool grade;
        Ponto viewport;
        QString arqAberto;
        int gradeSep;

        void desselecionarTodos();
        void selecionarTodos();
};

#endif // DOCUMENTO_H
