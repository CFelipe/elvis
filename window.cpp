#include "window.h"
#include <QtWidgets>
#include <QGLWidget>

Window::Window(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(tr("Elvis"));
    setupFileMenu();
    setupHelpMenu();
    createToolbars();
    createStatusBar();

    //QGLWidget* glArea;

    //setCentralWidget(glArea);
}

void Window::createToolbars() {
    QToolBar *toolbar = addToolBar("main toolbar");
    toolbar->setOrientation(Qt::Vertical);

    toolbar->addAction("Desenhar vértice?");
    toolbar->addAction("Desenhar polilinha");
    toolbar->addAction("Desenhar elipse");
    toolbar->addAction("Desenhar círculo");
    toolbar->addAction("Desenhar retângulo");
    toolbar->addSeparator();

    toolbar->addAction("Copiar");
    toolbar->addAction("Transladar");
    toolbar->addAction("Rotacionar");
    toolbar->addAction("Espelhar");
    toolbar->addAction("Escalar");
    toolbar->addAction("Deslocar");
    toolbar->addSeparator();

    toolbar->addAction("Agrupar");
    toolbar->addAction("Desagrupar");
    toolbar->addAction("Alinhar");

}

void Window::createStatusBar() {
    QToolButton* fillButton = new QToolButton();
    QColor* fillColor = new QColor(0, 128, 0);
    QPixmap px(20, 20);
    px.fill(*fillColor);
    fillButton->setIcon(px);
    fillButton->setAutoRaise(false);
    statusBar()->insertWidget(0, fillButton);

    QSpinBox* zoomSpinBox = new QSpinBox();
    zoomSpinBox->setSuffix("%");
    zoomSpinBox->setMaximum(1000);
    zoomSpinBox->setMinimum(25);
    zoomSpinBox->setSingleStep(5);
    statusBar()->insertPermanentWidget(1, zoomSpinBox);
}

void Window::setupFileMenu() {
    QMenu *fileMenu = new QMenu(tr("&Arquivo"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&Novo"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Abrir..."), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("&Sair"), qApp, SLOT(quit()), QKeySequence::Quit);
}

void Window::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Ajuda"), this);
    menuBar()->addMenu(helpMenu);

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

void Window::newFile()
{
    // Tratar criação de arquivo aqui
}
