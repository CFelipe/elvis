#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"

#include <QMainWindow>

class Window : public QWidget {
    Q_OBJECT

public:
    Window();

private:
    GLWidget* glArea;

    QMenuBar* menuBar;

    QToolBar* leftBar;

    QToolBar* bottomBar;
    QToolButton* linhaColorButton;
    QToolButton* fillColorButton;
    QToolButton *toggleGradeButton;

    QAction *addSelecionarAct;
    QAction *addPolilinhaAct;
    QAction *addElipseAct;
    QAction *addCirculoAct;
    QAction *addRetanguloAct;

    QAction *panAct;

    QAction *copiarAct;
    QAction *transladarAct;
    QAction *rotacionarAct;
    QAction *escalarAct;
    QAction *deslocarPtsAct;
    QAction *insertRemovePontoAct;

    QAction *agruparAct;
    QAction *desagruparAct;



    void createActions();
    void createLeftBar();
    void createBottomBar();
    void createStatusBar();
    void setupFileMenu();
    void setupHelpMenu();
    void mostrarAcoesObjeto(bool visivel);

    void updateFillButton();
    void updateLinhaButton();

public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());
    void setOperacao(QAction* q);
    void setLinhaColor();
    void setFillColor();
    void setEspessuraLinha(int espessura);
    void toggleGrade();
};

#endif // WINDOW_H
