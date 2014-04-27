#ifndef ELIPSE_H
#define ELIPSE_H

class Elipse : public Objeto {
    public:
        Elipse(Vertice centro, GLint raioHorizontal, GLint raioVertical, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha);

        void desenha();

        Ponto centro;

        Vertice getCentro(){
            return centro;
        }

        GLint getRaioHorizontal(){
            return raioHorizontal;
        }

        GLint getRaioVertical(){
            return raioVertical;
        }

        Vertice getControl(){
            return control;
        }

        Vertice *getPControl(){
            return &control;
        }

        void setCentro(Vertice c){
            centro = c.p;
           // updateControl();
        }
        void setRaioHorizontal(GLint n){
            raioHorizontal = n;
           // updateControl();
        }

        void setRaioVertical(GLint n){
            raioVertical = n;
          //  updateControl();
        }

        void setControl(Vertice c){
            c.selecionado = this->control.selecionado;
            this->control = c;
        }

    private:
        Vertice control;
        GLint raioHorizontal, raioVertical;

};

#endif // ELIPSE_H
