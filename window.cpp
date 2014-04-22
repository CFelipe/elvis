#include "window.h"
#include "glwidget.h"
#include "objeto.h"

#include <QtWidgets>

Window::Window() {
    setWindowTitle(tr("Elvis"));

    QGridLayout *mainLayout = new QGridLayout;
    menuBar = new QMenuBar();

    glArea = new GLWidget();
    glArea->setFocus();

    // -------------------------------------
    // Substituir por uma só função (ou não)
    setupFileMenu();
    setupHelpMenu();
    // -------------------------------------
    createActions();
    createLeftBar();
    createBottomBar();

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(leftBar, 1, 0);
    mainLayout->addWidget(glArea, 1, 1);
    mainLayout->addWidget(bottomBar, 2, 0, 2, 1);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowStretch(2, 0);

    setLayout(mainLayout);
}

void Window::createActions() {
    // -----------
    // Ferramentas
    // -----------
    addSelecionarAct = new QAction("Selecionar", this);
    addPolilinhaAct = new QAction("Polilinha", this);
    addElipseAct = new QAction("Elipse", this);
    addCirculoAct = new QAction("Círculo", this);
    addRetanguloAct = new QAction("Retângulo", this);

    addSelecionarAct->setCheckable(true);
    addPolilinhaAct->setCheckable(true);
    addElipseAct->setCheckable(true);
    addCirculoAct->setCheckable(true);
    addRetanguloAct->setCheckable(true);

    addElipseAct->setChecked(true);

    // -------------------
    // Operações de objeto
    // -------------------
    duplicarAct = new QAction("Duplicar", this);
    duplicarAct->setVisible(false);
    transladarAct = new QAction("Transladar", this);
    transladarAct->setVisible(false);
    rotacionarAct = new QAction("Rotacionar", this);
    rotacionarAct->setVisible(false);
    escalarAct = new QAction("Escalar", this);
    escalarAct->setVisible(false);
    espelharAct = new QAction("Espelhar", this);
    espelharAct->setVisible(false);
    deslocarPtsAct = new QAction("Deslocar pontos", this);
    deslocarPtsAct->setVisible(false);
}

void Window::mostrarAcoesObjeto(bool visivel) {
    if(visivel) {
        duplicarAct->setVisible(true);
        transladarAct->setVisible(true);
        rotacionarAct->setVisible(true);
        escalarAct->setVisible(true);
        espelharAct->setVisible(true);
        deslocarPtsAct->setVisible(true);
    } else {
        duplicarAct->setVisible(false);
        transladarAct->setVisible(false);
        rotacionarAct->setVisible(false);
        escalarAct->setVisible(false);
        espelharAct->setVisible(false);
        deslocarPtsAct->setVisible(false);

    }
}

void Window::createLeftBar() {
    leftBar = new QToolBar("Ferramentas principais");
    leftBar->setOrientation(Qt::Vertical);
    leftBar->setFloatable(false);
    leftBar->setMovable(false);

    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->addAction(addSelecionarAct);
    toolGroup->addAction(addPolilinhaAct);
    toolGroup->addAction(addElipseAct);
    toolGroup->addAction(addCirculoAct);
    toolGroup->addAction(addRetanguloAct);
    QObject::connect(toolGroup, SIGNAL(triggered(QAction*)), glArea, SLOT(setForma(QAction * )));

    leftBar->addAction(addSelecionarAct);
    leftBar->addAction(addPolilinhaAct);
    leftBar->addAction(addElipseAct);
    leftBar->addAction(addCirculoAct);
    leftBar->addAction(addRetanguloAct);

    leftBar->addAction(duplicarAct);
    leftBar->addAction(transladarAct);
    leftBar->addAction(rotacionarAct);
    leftBar->addAction(escalarAct);
    leftBar->addAction(deslocarPtsAct);
}

void Window::createBottomBar() {
    bottomBar = new QToolBar();
    bottomBar->setOrientation(Qt::Horizontal);
    bottomBar->setFloatable(false);
    bottomBar->setMovable(false);

    QToolButton* fillButton = new QToolButton();
    QColor* fillColor = new QColor(0, 128, 0);
    QPixmap px(20, 20);
    px.fill(*fillColor);
    fillButton->setIcon(px);
    fillButton->setAutoRaise(true);
    bottomBar->insertWidget(0, fillButton);

    QSpinBox* zoomSpinBox = new QSpinBox();
    zoomSpinBox->setSuffix("%");
    zoomSpinBox->setMaximum(1000);
    zoomSpinBox->setValue(100);
    zoomSpinBox->setMinimum(25);
    zoomSpinBox->setSingleStep(5);
    bottomBar->insertWidget(0, zoomSpinBox);
}

void Window::setupFileMenu() {
    QMenu *fileMenu = new QMenu(tr("&Arquivo"), this);
    menuBar->addMenu(fileMenu);

    fileMenu->addAction(tr("&Novo"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Abrir..."), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("&Sair"), qApp, SLOT(quit()), QKeySequence::Quit);
}

void Window::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Ajuda"), this);
    menuBar->addMenu(helpMenu);

    helpMenu->addAction(tr("&Sobre"), this, SLOT(about()));
}

void Window::openFile(const QString &path) {
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Abrir arquivo"), "", ".elv");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            // Abre arquivo
        }
    }
}

void Window::about() {
    QMessageBox::about(this,
                       tr("Sobre Elvis"),
                       tr("Elvis é uma aplicação desenvolvida por Felipe Fernandes e Felipe Cortez para disciplina de Computação Gráfica na UFRN"));
}

void Window::newFile() {
    // Tratar criação de arquivo aqui
}
