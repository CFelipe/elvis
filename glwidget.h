#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include "objeto.h"
#include "circulo.h"
#include "elipse.h"
#include "linha.h"
#include "ponto.h"
#include "vertice.h"
#include "retangulo.h"
#include "camada.h"
#include <QtOpenGL/QGLWidget>
#include <QList>

enum Operacao {SELECIONAR, TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO, INSERT_REMOVE_PONTO, AGRUPAMENTO};

typedef struct l {
    Objeto *objeto;
    l *next;
    l *previous;
} Lista;

class GLWidget : public QGLWidget {

    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);

        QList<Camada*> camadas;
        Camada* camadaSelecionada;

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        void descelecionaALL();
        void selecionaCirculo(Objeto *aux, Circulo *c, Ponto click);
        void selecionaQuadrilatero(Objeto *aux, Retangulo *q, Ponto click);

    public slots:
        void setOperacao(QAction* q);
};

#endif  /* _GLWIDGET_H */
