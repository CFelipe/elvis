#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include "objeto.h"
#include <QtOpenGL/QGLWidget>

enum Operacao {TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO};

class GLWidget : public QGLWidget {

    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);

   // private:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        void rotacionaObjeto(Objeto* ob);

        //void selecionaQuadrilatero(Lista *aux, Quadrilatero *q, Ponto click);

        Quadrilatero* getAreaClippingMouse(GLint xmouse, GLint ymouse);


    public slots:
        void setOperacao(QAction* q);
};

#endif  /* _GLWIDGET_H */
