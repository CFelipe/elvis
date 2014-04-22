#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include "objeto.h"
#include <QtOpenGL/QGLWidget>


class GLWidget : public QGLWidget {

    Q_OBJECT

    public:
        // ---------------
        // Lista encadeada
        typedef struct l {
            Objeto *objeto;
            l *next;
            l *previous;
        } Lista;

        Lista *init=NULL, *fim=NULL;

        void insere(Objeto *ob) { // insere sempre no final da lista
            Lista *novo = new Lista;
            novo->objeto = ob;
            novo->next = NULL;
            novo->previous = fim;
            if (init==NULL && fim==NULL){
                init=novo;
            } else {
                fim->next = novo;
            }
            fim = novo;
        }

        void remove(Lista *re){
            if (re==init && re==fim){
                init=NULL;
                fim=NULL;
            } else if (re==init){
                init = re->next;
                (re->next)->previous = NULL; // re->next != NULL (certeza)

            } else if (re==fim){
                fim = re->previous;
                re->previous->next = NULL; //re->previous != NULL (certeza)
            } else {
                (re->previous)->next = re->next;
                (re->next)->previous = re->previous;
            }
        }
        // ---------------

        enum Operacao {TRANSLACAO, COPIA, ESCALA, DESLOCARPONTOS, ROTACAO};

        GLWidget(QWidget *parent = 0);

    private:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        void rotacionaObjeto(Objeto* ob);

        void selecionaQuadrilatero(Lista *aux, Quadrilatero *q, Ponto click);

        Quadrilatero* getAreaClippingMouse(GLint xmouse, GLint ymouse);

        Operacao op;
        Objeto::Forma forma;

        GLint espessuraLinha;
        bool preenchido; // true  -> a forma está preenchida com alguma cor
        bool desenha; // true = desenha. false = seleciona

    public slots:
        void setOperacao(QAction* q);
        void setForma(QAction* q);
};

#endif  /* _GLWIDGET_H */
