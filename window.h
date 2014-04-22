#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"

#include <QMainWindow>

class Window : public QWidget {
    Q_OBJECT

public:
    Window();

private:
    QGLWidget* glArea;
    QMenuBar* menuBar;
    QToolBar* leftBar;
    QToolBar* bottomBar;

    void createActions();
    void createLeftBar();
    void createBottomBar();
    void createStatusBar();
    void setupFileMenu();
    void setupHelpMenu();
    void mostrarAcoesObjeto(bool visivel);

    QAction *addSelecionarAct;
    QAction *addPolilinhaAct;
    QAction *addElipseAct;
    QAction *addCirculoAct;
    QAction *addRetanguloAct;

    QAction *duplicarAct;
    QAction *transladarAct;
    QAction *rotacionarAct;
    QAction *escalarAct;
    QAction *espelharAct;
    QAction *deslocarPtsAct;

    QAction *agruparAct;
    QAction *desagruparAct;

    //TODO: diversos tipos de alinhamento

public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());
};

#endif // WINDOW_H
