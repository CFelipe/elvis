#include "objeto.h"
#include "window.h"

int Objeto::nId = 0;

Objeto::Objeto(Forma tipo) {
    this->id = nId++;
    this->selecionado = false;
    this->tipo = tipo;

    this->espessuraLinha = Window::docAtual->espessuraLinha;
    this->preenchido = Window::docAtual->preenchimento;
    this->linha = Window::docAtual->linha;
    this->estiloLinha = "-";

    this->colorfill[0] = Window::docAtual->fillColorSelecionada[0];
    this->colorfill[1] = Window::docAtual->fillColorSelecionada[1];
    this->colorfill[2] = Window::docAtual->fillColorSelecionada[2];
    this->colorfill[3] = Window::docAtual->fillColorSelecionada[3];

    this->colorLine[0] = Window::docAtual->linhaColorSelecionada[0];
    this->colorLine[1] = Window::docAtual->linhaColorSelecionada[1];
    this->colorLine[2] = Window::docAtual->linhaColorSelecionada[2];
    this->colorLine[3] = Window::docAtual->linhaColorSelecionada[3];
}

void Objeto::desenha() {
    if(preenchido) {
        desenhaFill();
    }
    if(linha) {
        desenhaLinha();
    }
    if(selecionado) {
        desenhaControles();
    }
}

void Objeto::linhaFill(Ponto p1, Ponto p2) {
    // Sempre da esquerda pra direita ou
    // de cima pra baixo
    GLfloat coFill[4];
    getColorFill(coFill);
    glColor4f(coFill[0],coFill[1],coFill[2], coFill[3]);
    glPointSize(1);

    int x, y;
    if(p1.y == p2.y) {
        y = p1.y;
        for(x = p1.x; x <= p2.x; x++) {
            glBegin( GL_POINTS );
                glVertex2i((GLint) x, (GLint) y);
            glEnd( );
        }
    }
}

void Objeto::Bresenham(Ponto p1, Ponto p2) {
    int linha_i = 0;

    GLfloat co[4];
    getColorLine(co);
    glColor4f( co[0],co[1],co[2], co[3]);

    // Especifica o diâmetro do Vertice
    glPointSize(espessuraLinha);

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

            if(estiloLinha[linha_i] == '-') {
                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }

            linha_i = (linha_i + 1) % estiloLinha.size();

            if (p1.x<p2.x){
                while (x<p2.x){

                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y++;
                        pk = pk + ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            } else {
                while (x>p2.x){
                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y--;
                        pk = pk - ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
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

            if(estiloLinha[linha_i] == '-') {
                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }

            linha_i = (linha_i + 1) % estiloLinha.size();

            if (p1.x<p2.x){
                while (x<p2.x){
                    x++;
                    if (pk<0) pk = pk+dy_2;
                    else {
                        y--;
                        pk = pk + ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            } else {
                while (x>p2.x){
                    x--;
                    if (pk<0) pk = pk-dy_2;
                    else {
                        y++;
                        pk = pk - ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
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

            if(estiloLinha[linha_i] == '-') {
                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }

            linha_i = (linha_i + 1) % estiloLinha.size();

            if (p1.y<p2.y){
                while (y<p2.y){
                    y++;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            } else {
                while (y>p2.y){
                    y--;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
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

            if(estiloLinha[linha_i] == '-') {
                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }

            linha_i = (linha_i + 1) % estiloLinha.size();

            if (p1.y>p2.y){
                while (y>p2.y){
                    y--;
                    if (pk<0) pk = pk+dx_2;
                    else {
                        x++;
                        pk = pk + ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            } else {
                while (y<p2.y){
                    y++;
                    if (pk<0) pk = pk-dx_2;
                    else {
                        x--;
                        pk = pk - ddxy;
                    }

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
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

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            } else {
                while (y>p2.y){
                    x--;
                    y--;

                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            }
        } else if (m==-1){
            x = p1.x;
            y = (GLfloat) p1.y;

            if(estiloLinha[linha_i] == '-') {
                glBegin( GL_POINTS );
                    glVertex2i( (GLint)x, (GLint)y );
                glEnd( );
            }

            linha_i = (linha_i + 1) % estiloLinha.size();

            if (p1.y>p2.y){
                while (y>p2.y){
                    x++;
                    y--;
                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
                }
            } else {
                while (y<p2.y){
                    x--;
                    y++;
                    if(estiloLinha[linha_i] == '-') {
                        glBegin( GL_POINTS );
                            glVertex2i( (GLint)x, (GLint)y );
                        glEnd( );
                    }

                    linha_i = (linha_i + 1) % estiloLinha.size();
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

                if(estiloLinha[linha_i] == '-') {
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }

                linha_i = (linha_i + 1) % estiloLinha.size();
            }
        } else {
            while (x>p2.x){
                x--;

                if(estiloLinha[linha_i] == '-') {
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }

                linha_i = (linha_i + 1) % estiloLinha.size();
            }
        }
    } else { // se a linha for vertical:
        x = p1.x;
        y = (GLfloat) p1.y;

        if(estiloLinha[linha_i] == '-') {
            glBegin( GL_POINTS );
                glVertex2i( (GLint)x, (GLint)y );
            glEnd( );
        }

        linha_i = (linha_i + 1) % estiloLinha.size();

        if (p1.y<p2.y){
            while (y<p2.y){
                y++;

                if(estiloLinha[linha_i] == '-') {
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }

                linha_i = (linha_i + 1) % estiloLinha.size();
            }
        } else {
            while (y>p2.y){
                y--;

                if(estiloLinha[linha_i] == '-') {
                    glBegin( GL_POINTS );
                        glVertex2i( (GLint)x, (GLint)y );
                    glEnd( );
                }

                linha_i = (linha_i + 1) % estiloLinha.size();
            }
        }
    }
}
