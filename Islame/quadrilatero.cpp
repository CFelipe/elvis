#include "mainwindow.h"
#include <QtOpenGL>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

typedef struct _GLintPoint {
        GLint x;
        GLint y;
} GLintPoint ;


/*typedef struct l{
    GLintPoint p1, p2;
} GLlistaRetas;
*/

typedef struct q{
    GLintPoint A, B, C, D;
    q *next;
}GLsquareList;

GLsquareList *init=NULL, *fim=NULL;

void insere(GLintPoint A, GLintPoint B,GLintPoint C, GLintPoint D ){ // insere sempre no final da lista
  GLsquareList *novo = new GLsquareList;
    novo->A.x = A.x;
    novo->A.y = A.y;
    novo->B.x = B.x;
    novo->B.y = B.y;
    novo->C.x = C.x;
    novo->C.y = C.y;
    novo->D.x = D.x;
    novo->D.y = D.y;

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
bool onMouseClik = false;
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}

void GLWidget::initializeGL() {

}

GLintPoint p1, p2;

//! Algoritmo Bresenham
void Bresenham(GLintPoint p1, GLintPoint p2) {
    glPointSize(4); //especifica o diâmetro do ponto

    GLint x, y;
    if (p2.x!=p1.x && p2.y!=p1.y){
        GLfloat m = (GLfloat) (p2.y-p1.y) / (p2.x-p1.x);
        if (m>0  && m<1){

            GLint dx = p2.x - p1.x;
            GLint dy = p2.y - p1.y;
            GLint pk = 2*dy - dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;
            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.x<p2.x){
                while (x<p2.x){

                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (x>p2.x){

                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m>-1 && m<0){
            printf("%f\n", m);
            GLint dx = p2.x - p1.x;
            GLint dy = - p2.y + p1.y;
            GLint pk = 2*dy + dx;
            GLint dy_2 = 2*dy;
            GLint ddxy = dy_2 - 2*dx;

            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.x<p2.x){
                while (x<p2.x){
                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y--;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (x>p2.x){
                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y++;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m>1){
            GLint dx = p2.x - p1.x;
            GLint dy = p2.y - p1.y;
            GLint pk = 2*dx + dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y<p2.y){
                while (y<p2.y){
                    y++;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.y){
                    y--;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m<-1){
            GLint dx = p2.x - p1.x;
            GLint dy = -p2.y + p1.y;
            GLint pk = 2*dx - dy;
            GLint dx_2 = 2*dx;
            GLint ddxy = dx_2 - 2*dy;

            x = p1.x;
            y = p1.y;
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y>p2.y){
                while (y>p2.y){
                    y--;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.y){
                    y++;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }

        } else if (m==1){

            x = p1.x;
            y = (GLfloat) p1.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y<p2.y){
                while (y<p2.y){
                    x++;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y>p2.y){
                    x--;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        } else if (m==-1){
            x = p1.x;
            y = (GLfloat) p1.y;
            glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
            if (p1.y>p2.y){
                while (y>p2.y){
                    x++;
                    y--;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            } else {
                while (y<p2.y){
                    x--;
                    y++;
                    glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }
            }
        }
    } else if (p2.y==p1.y){ //se a linha for horizontal:

        x = p1.x;
        y = (GLfloat) p1.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.x<p2.x){
            while (x<p2.x){
                x++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (x>p2.x){
                x--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
    } else { // se a linha for vertical:
        x = p1.x;
        y = (GLfloat) p1.y;
        glBegin( GL_POINTS );
            glVertex2i( (GLint)x, (GLint)y );
        glEnd( );
        if (p1.y<p2.y){
            while (y<p2.y){
                y++;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        } else {
            while (y>p2.y){
                y--;

                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }
        }
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
    glClearColor( 0.9f, 0.9f, 0.9f, 0.f );
        glClear( GL_COLOR_BUFFER_BIT );

        glColor3f( 1.f, 0.f, 0.f );

        glPushMatrix( );
        glPointSize(3);
        GLsquareList *aux = init;
        while (aux!=NULL){
            Bresenham(aux->A, aux->B);
            Bresenham(aux->B, aux->D);
            Bresenham(aux->D, aux->C);
            Bresenham(aux->C, aux->A);

            aux = aux->next;
        }
        glPopMatrix( );
        // Flush the pipeline, swap the buffers
        glFlush();

}



void GLWidget::mousePressEvent(QMouseEvent *event) {

    if (event->buttons() && Qt::LeftButton){
        GLintPoint A; // ponto de clik
        GLintPoint D; // ponto de soltar o mouse
        GLintPoint B, C;

        A .x= event->x();
        A.y = gfWrldSizeY-event->y();
        D.x = event->x();
        D.y = gfWrldSizeY-event->y();
        B.x = D.x;
        B.y = A.y;
        C.x = A.x;
        C.y = D.y;
        insere (A, B, C, D);
        onMouseClik = true;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){


}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->buttons() && Qt::LeftButton){
        onMouseClik = false;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    //cout<<onMouseClik<<endl;
    if (onMouseClik){

        //fim->p2.x = event->x();
        //fim->p2.y = gfWrldSizeY-event->y();

        fim->D.x = event->x();
        fim->D.y = gfWrldSizeY-event->y();
        fim->B.x = fim->D.x;
        fim->B.y = fim->A.y;
        fim->C.x = fim->A.x;
        fim->C.y = fim->D.y;
        updateGL();
    }
    //if(event->buttons() & Qt::LeftButton)
    //    cout<<"MOVE"<<endl;
}

void GLWidget::keyPressEvent(QKeyEvent* event) {

}
