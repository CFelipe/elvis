#ifndef ELIPSE_H
#define ELIPSE_H

class Elipse : public Objeto {
    private:
        Vertice centro;
        GLint raioHorizontal, raioVertical;
        Vertice control;

    public:
        Elipse(Vertice centro, GLint raioHorizontal, GLint raioVertical, GLfloat colorfill[4], GLfloat colorLine[4], GLint espessuraLinha, Forma tipo);

        void desenha();

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
            centro = c;
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
            c.setSelect(this->control.isSelect());
            this->control = c;
        }


};

#endif // ELIPSE_H
