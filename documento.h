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
               PAN};

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
        QString estiloLinha;
        bool preenchimento;
        bool linha;
        bool grade;
        Ponto viewport;
        GLint canvasW;
        GLint canvasH;
        Ponto selecaoMin;
        Ponto selecaoMax;

        bool desenha;
        bool desenhandoPolilinha;

        bool selecaoMultipla;

        QString arqAberto;
        GLint gradeSep;

        void desselecionarTodos();
        void selecionarTodos();
        void deletarSelecionados();

        bool exportarSVG(const QString &fileName);

       private:
            QString dasharrayToSVG(QString dash);
};

#endif // DOCUMENTO_H
