#include "mainwindow.h"
#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

using namespace std;

#define PRECISAO 0.01

typedef struct{
    GLfloat red, green, blue;
}Color;

typedef struct q{
    GLint xc, yc;
    GLint xclick, yclick; // usados para translação e cópia, a fim guardar o pixel o mouse clicou na forma e depois arrastou
    GLint raio;
    bool isCor;
    bool isSelect;
    q *next;
}GLCircleList;


GLCircleList *init=NULL, *fim=NULL;

void insere(GLint xc, GLint yc, GLint raio){ // insere sempre no final da lista
    GLCircleList *novo = new GLCircleList;
    novo->xc = xc;
    novo->yc = yc;
    novo->isCor = false;
    novo->isSelect = false;
    novo->raio = raio;
    novo->next = NULL;
    if (init==NULL && fim==NULL){
        init=novo;
    } else {
        fim->next = novo;
    }
    fim = novo;
}



GLfloat gfWrldSizeX = 800.f;
GLfloat gfWrldSizeY = 600.f;
GLfloat gfWrldSizeZ = 400.f;
GLint xmouse=0, ymouse=0;
bool desenha = true; // true = desenha. false = seleciona
bool onMouseClik = false;
GLint op = 2;

//! op = 1 --> translação
//! op = 2 --> copiar
//! op = 3 --> escalar
//! .
//! .

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}

void GLWidget::initializeGL() {

}

bool equalFloat(GLfloat a, GLfloat b) { //compara se a==b
    GLfloat d = a-b>0?a-b:b-a;
    return d<=PRECISAO;
}

void preenchimento(int raio, int x, int y, Color fill, Color boundary) {
    GLfloat pixel[4];
    glColor3f( fill.red, fill.green, fill.blue );
    glPointSize(0.5);
    int j = 0, i;
    while (j<raio){
        i=0;
        glReadPixels(i+x, j+y, 1, 1, GL_RGBA, GL_FLOAT, &pixel[0]);
        while (!equalFloat(pixel[0],boundary.red) && !equalFloat(pixel[1],boundary.green) && !equalFloat(pixel[2],boundary.blue)){
            glBegin(GL_POINTS);
                    glVertex2i(i+x, j+y);
                    glVertex2i(i+x, -j+y);
                    glVertex2i(-i+x, j+y);
                    glVertex2i(-i+x, -j+y);

            glEnd();
            i++;
            glReadPixels(i+x, j+y, 1, 1, GL_RGBA, GL_FLOAT, &pixel[0]);
        }
        j++;
    }
}

//! Algoritmo de rasterização do circulo
void drawCircle(GLint raio, GLint xc, GLint yc){
        glPointSize(2);
    GLint x = 0;
    GLint  y = raio;
    GLint  d = 1 - raio;
    GLint  dL = 3;
    GLint  dse = -2*raio + 5;
    glBegin( GL_POINTS );
        glVertex2i( x+xc, y+yc );
        glVertex2i( x+xc, -y+yc );
        glVertex2i( -x+xc, y+yc );
        glVertex2i( -x+xc, -y+yc );
        glVertex2i( y+xc, x+yc );
        glVertex2i( y+xc, -x+yc );
        glVertex2i( -y+xc, x+yc );
        glVertex2i( -y+xc, -x+yc );
    glEnd( );
    while (y>x){
        if (d<0){
               d+=dL;
               dL+=2;
               dse+=2;
        } else {
               d+=dse;
               dL+=2;
               dse+=4;
               y--;

        }
        x++;
        glBegin( GL_POINTS );
            glVertex2i( x+xc, y+yc );
            glVertex2i( x+xc, -y+yc );
            glVertex2i( -x+xc, y+yc );
            glVertex2i( -x+xc, -y+yc );
            glVertex2i( y+xc, x+yc );
            glVertex2i( y+xc, -x+yc );
            glVertex2i( -y+xc, x+yc );
            glVertex2i( -y+xc, -x+yc );
        glEnd( );

    }
}


void GLWidget::resizeGL(int w, int h) {
    float vfAspect = 0.f;
        float viewDepth = gfWrldSizeZ/2.f;

        // Usar toda a janela.
        glViewport(0, 0, w, h);

        h = (h==0) ? 1 : h;
        w = (w==0) ? 1 : w;
        vfAspect = ( h < w ) ? (float)w/(float)h : (float)h/(float)w ;
        cout<<w<<", "<<h<<endl;
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        glOrtho( 0, w, // * vfAspect,
                 0, h, // * vfAspect,
                -viewDepth, viewDepth );

        gfWrldSizeX = w;
        gfWrldSizeY = h;
        glMatrixMode( GL_MODELVIEW );

        glLoadIdentity( );
}

void GLWidget::paintGL() {
    glClearColor( 0.9, 0.9, 0.9, 0);
        glClear( GL_COLOR_BUFFER_BIT );

        glColor3f( 1.f, 0.f, 0.f );

        glPushMatrix( );
        GLCircleList *aux = init;
        while (aux!=NULL){
            drawCircle(aux->raio, aux->xc, aux->yc);
            if (aux->isSelect==true){
                glPointSize(6);
                glBegin(GL_POINTS);
                    glColor3f( 0,0.5 , 0 );
                    glVertex2i(aux->xc, aux->yc);
                    glVertex2i(aux->xc+aux->raio, aux->yc);
                  glEnd();
            }
            //if (aux->isCor==true) preenchimento(aux->raio,aux->xc, aux->yc, {0,0,0}, {1, 0, 0});

            aux = aux->next;
            glColor3f( 1.f, 0.f, 0.f );
        }
        glPopMatrix( );
        // Flush the pipeline, swap the buffers
        glFlush();

}



void GLWidget::mousePressEvent(QMouseEvent *event) {
    if (desenha){
        if (event->buttons() && Qt::LeftButton){
            if (onMouseClik==false){
                GLint xc =event->x();
                GLint yc = gfWrldSizeY-event->y();
                insere (xc, yc, 0);
            } else {
                fim->isCor= true;
            }
            onMouseClik = !onMouseClik;
           // updateGL();
        }
    } else { // selecionar
        GLCircleList *aux = init;
        GLCircleList *auxFim = fim;
        do{
            if (aux!=NULL){
                if (event->x()<=aux->xc+aux->raio && (gfWrldSizeY-event->y())<aux->yc+aux->raio && event->x()>aux->xc-aux->raio && (gfWrldSizeY-event->y())>aux->yc-aux->raio){
                    aux->isSelect = true;
                    aux->xclick = event->x() -aux->xc;
                    aux->yclick = gfWrldSizeY-event->y() - aux->yc;
                    if (op==2){
                        insere (aux->xc, aux->yc, aux->raio);
                        aux->isSelect = false;
                        fim->isSelect = true;
                        fim->xclick = event->x() -fim->xc;
                        fim->yclick = gfWrldSizeY-event->y()- fim->yc;
                        //updateGL();
                    }
                } else {
                    aux->isSelect = false;
                }
                aux = aux->next;
            }
        }while (aux!=auxFim->next);
        onMouseClik = true;
         updateGL();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    //if (desenha==false){
        onMouseClik = false;
   // }

}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event){
    //if (event->buttons() && Qt::LeftButton){
        onMouseClik = false;
    //}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    //cout<<onMouseClik<<endl;
    if (desenha){
        if (onMouseClik){
            fim->raio = sqrt(pow((event->x() - fim->xc), 2)+ pow((gfWrldSizeY-event->y()-fim->yc), 2));
        }
    } else if (onMouseClik){ // Primeiro Teste: translação
        GLCircleList *aux = init;
        while (aux!=NULL){
            if (aux->isSelect==true){
                if (op==1 || op==2){
                    aux->xc=event->x()-aux->xclick;
                    aux->yc=gfWrldSizeY-event->y()-aux->yclick;
                } else if (op==3){
                    aux->raio = sqrt(pow((event->x() - aux->xc), 2)+ pow((gfWrldSizeY-event->y()-aux->yc), 2));
                }
            }
            aux = aux->next;
        }
    }
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key()==83){ // se pressionar 's'
          desenha=false;
          onMouseClik = false;
    } else if (event->key()==68) { // se pressionar 'd'
          desenha=true;
           GLCircleList *aux=  init;
          while (aux!=NULL){
              aux->isSelect = false;
              aux = aux->next;
          }
          updateGL();
    }
}
