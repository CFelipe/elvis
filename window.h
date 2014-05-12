#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"
#include "documento.h"

class Window : public QWidget {
    Q_OBJECT

public:
    Window();
    static Documento* docAtual;

private:
    GLWidget* glArea;
    QMenuBar* menuBar;

    // Esquerda
    QToolBar* leftBar;

    QAction *selecionarAct;
    QAction *selecionarPontosAct;

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

    QListView *listaCamadas;

    // Embaixo
    QToolBar* bottomBar;

    QToolButton* linhaColorButton;
    QToolButton* fillColorButton;
    QToolButton* toggleGradeButton;

    void createActions();
    void createLeftBar();
    void createBottomBar();
    void createStatusBar();
    void setupMenus();
    void mostrarAcoesObjeto(bool visivel);

    void updateFillButton();
    void updateLinhaButton();

    bool eventFilter(QObject *watched, QEvent *e);

public slots:
    void about();
    void abrirElv(const QString &path = QString());
    bool salvarElv();
    bool salvarElvComo();
    bool exportarSVG();
    void setOperacao(QAction* q);
    void setLinhaColor();
    void setFillColor();
    void setEspessuraLinha(int espessura);
    void toggleGrade();
};

#endif // WINDOW_H
