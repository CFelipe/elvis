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

enum Operacao {TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO, INSERT_REMOVE_PONTO};

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

        /* Esta variável diz se uma polilinha está sendo desenhada;
         * É importante para diferenciar quando uma nova polilinha é criada,
         * e quando uma nova linha é adicionada à polilinha que está sendo desenhada
         */
        bool desenhandoPolilinha;

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
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void descelecionaALL();

    private:
        void selecionaCirculo(Objeto *aux, Circulo *c, Ponto click);
        void selecionaQuadrilatero(Objeto *aux, Retangulo *q, Ponto click);
        Retangulo* getAreaClippingMouse(GLint xmouse, GLint ymouse);
        void desenhaGrade(GLint sep);

};

#endif  /* _GLWIDGET_H */
