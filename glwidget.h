#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include "objeto.h"
#include "retangulo.h"
#include "ponto.h"
#include <QtOpenGL/QGLWidget>

enum Operacao {TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO, INSERT_REMOVE_PONTO};

typedef struct l {
    Objeto *objeto;
    l *next;
    l *previous;
} Lista;

class GLWidget : public QGLWidget {

    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        void rotacionaObjeto(Objeto* ob);
        Retangulo* getAreaClippingMouse(GLint xmouse, GLint ymouse);
    public slots:
        void setOperacao(QAction* q);
};

#endif  /* _GLWIDGET_H */
