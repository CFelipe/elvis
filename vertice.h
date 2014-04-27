#ifndef VERTICE_H
#define VERTICE_H

class Vertice {
    public:
        Vertice(GLint x, GLint y);

        GLint getX(){
            return x;
        }
        GLint getY(){
            return y;
        }
        void setY(GLint y) {
            this->y = y;
        }
        void setX(GLint x){
            this->x = x;
        }

    private:
        GLint x, y;
        bool select;

};

#endif // VERTICE_H
