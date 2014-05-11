#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include "objeto.h"
#include "circulo.h"
#include "elipse.h"
#include "ponto.h"
#include "vertice.h"
#include "retangulo.h"
#include "camada.h"
#include <QtOpenGL/QGLWidget>
#include <QList>

enum Operacao {SELECIONAR, CRIACAO, TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO, INSERT_REMOVE_PONTO, AGRUPAMENTO};

class GLWidget : public QGLWidget {

    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);

        QList<Camada*> camadas;
        Camada* camadaSelecionada;
        GLfloat linhaColorSelecionada[4];
        GLfloat fillColorSelecionada[4];
        GLint espessuraLinha;
        bool desenha; // true = desenha. false = seleciona
        bool preenchimento;
        bool linha;
        bool grade;
        Ponto viewport;
        GLint canvasW;
        GLint canvasH;
        bool desenhandoPolilinha;
        GLint gradeSep;

        Operacao op;
        Objeto::Forma forma;

        /* A variável opBotaoDireito é usada somente para controlar a possibilidade de rotação de acordo como especificado no documento
         * opBotaoDireito=true <--> o botão direito foi pressionado em algum local do canvas
         * opBotaoDireito=false <--> o botão esquerdo foi pressionado sobre a região da figura ou sobre um ponto de controle
        */
        bool opBotaoDireito;
        bool onMouseClick;

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void desselecionaALL();
    private:
        void selecionaCirculo(Objeto *aux, Circulo *c, Ponto click);
        void selecionaQuadrilatero(Objeto *aux, Retangulo *q, Ponto click);
        Retangulo* getAreaClippingMouse(GLint xmouse, GLint ymouse);
        GLint snap(GLint mouse);
        void desenhaGrade(GLint sep);
        void desenhaCanvas();

};

#endif  /* _GLWIDGET_H */
