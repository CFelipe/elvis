#include "window.h"
#include "objeto.h"

#include <QtWidgets>

Documento* Window::docAtual = new Documento();

Window::Window() {
    setWindowTitle(tr("Elvis"));

    QGridLayout *mainLayout = new QGridLayout;
    menuBar = new QMenuBar();

    glArea = new GLWidget();

    setupMenus();

    createActions();
    createLeftBar();
    createBottomBar();

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(leftBar, 1, 0);
    mainLayout->addWidget(glArea, 1, 1);
    mainLayout->addWidget(bottomBar, 2, 0, 1, 2);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowStretch(2, 0);

    mostrarAcoesObjeto(true);

    setLayout(mainLayout);

    glArea->installEventFilter(this);
}

bool Window::eventFilter(QObject *watched, QEvent *e) {
    if (watched == glArea && e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
        if (ke->key() == Qt::Key_Delete) {
            docAtual->deletarSelecionados();
            glArea->updateGL();
            return true;
        }
    }

    return QWidget::eventFilter(watched, e);
}

void Window::createActions() {
    // -----------
    // Ferramentas
    // -----------
    selecionarAct = new QAction("Selecionar", this);
    selecionarAct->setIcon(QIcon(":/images/selecionar.png"));
    selecionarAct->setShortcut(QKeySequence("S"));

    selecionarPontosAct = new QAction("Selecionar pontos", this);
    selecionarPontosAct->setIcon(QIcon(":/images/selecionar_pontos.png"));
    selecionarPontosAct->setShortcut(QKeySequence("Shift+S"));

    addPolilinhaAct = new QAction("Desenhar polilinha", this);
    addPolilinhaAct->setIcon(QIcon(":/images/polilinha.png"));
    addPolilinhaAct->setShortcut(QKeySequence("P"));

    addElipseAct = new QAction("Desenhar elipse", this);
    addElipseAct->setIcon(QIcon(":/images/elipse.png"));
    addElipseAct->setShortcut(QKeySequence("E"));

    addCirculoAct = new QAction("Desenhar círculo", this);
    addCirculoAct->setIcon(QIcon(":/images/circulo.png"));
    addCirculoAct->setShortcut(QKeySequence("C"));

    addRetanguloAct = new QAction("Desenhar retângulo", this);
    addRetanguloAct->setIcon(QIcon(":/images/retangulo.png"));
    addRetanguloAct->setShortcut(QKeySequence("R"));

    addRetanguloAct = new QAction("Desenhar retângulo", this);
    addRetanguloAct->setIcon(QIcon(":/images/retangulo.png"));
    addRetanguloAct->setShortcut(QKeySequence("R"));

    panAct = new QAction("Mover viewport", this);
    panAct->setIcon(QIcon(":/images/pan.png"));

    selecionarAct->setCheckable(true);
    selecionarPontosAct->setCheckable(true);
    addPolilinhaAct->setCheckable(true);
    addElipseAct->setCheckable(true);
    addCirculoAct->setCheckable(true);
    addRetanguloAct->setCheckable(true);
    addElipseAct->setChecked(true);

    panAct->setCheckable(true);

    // -------------------
    // Operações de objeto
    // -------------------
    transladarAct = new QAction("Translação", this);
    copiarAct = new QAction("Cópia", this);
    escalarAct = new QAction("Escalar", this);
    deslocarPtsAct = new QAction("Deslocar pontos", this);
    rotacionarAct = new QAction("Rotacionar", this);
    insertRemovePontoAct = new QAction("insertRemovePonto", this);

    copiarAct->setCheckable(true);
    transladarAct->setCheckable(true);
    rotacionarAct->setCheckable(true);
    escalarAct->setCheckable(true);
    deslocarPtsAct->setCheckable(true);
    insertRemovePontoAct->setCheckable(true);
}

void Window::mostrarAcoesObjeto(bool visivel) {
    if(visivel) {
        copiarAct->setVisible(true);
        transladarAct->setVisible(true);
        rotacionarAct->setVisible(true);
        escalarAct->setVisible(true);
        deslocarPtsAct->setVisible(true);
    } else {
        copiarAct->setVisible(false);
        transladarAct->setVisible(false);
        rotacionarAct->setVisible(false);
        escalarAct->setVisible(false);
        deslocarPtsAct->setVisible(false);
    }
}

void Window::createLeftBar() {
    leftBar = new QToolBar("Ferramentas principais");
    leftBar->setOrientation(Qt::Vertical);
    leftBar->setFloatable(false);
    leftBar->setMovable(false);
    leftBar->setIconSize(QSize(32, 32));

    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->addAction(selecionarAct);
    toolGroup->addAction(selecionarPontosAct);
    toolGroup->addAction(addPolilinhaAct);
    toolGroup->addAction(addElipseAct);
    toolGroup->addAction(addCirculoAct);
    toolGroup->addAction(addRetanguloAct);

    toolGroup->addAction(panAct);

    toolGroup->addAction(copiarAct);
    toolGroup->addAction(transladarAct);
    toolGroup->addAction(rotacionarAct);
    toolGroup->addAction(escalarAct);
    toolGroup->addAction(deslocarPtsAct);
    toolGroup->addAction(insertRemovePontoAct);
    QObject::connect(toolGroup, SIGNAL(triggered(QAction*)), this, SLOT(setOperacao(QAction * )));

    leftBar->addAction(selecionarAct);
    leftBar->addAction(selecionarPontosAct);
    leftBar->addAction(addPolilinhaAct);
    leftBar->addAction(addElipseAct);
    leftBar->addAction(addCirculoAct);
    leftBar->addAction(addRetanguloAct);

    leftBar->addAction(panAct);

    leftBar->addAction(copiarAct);
    leftBar->addAction(transladarAct);
    leftBar->addAction(rotacionarAct);
    leftBar->addAction(escalarAct);
    leftBar->addAction(deslocarPtsAct);
    leftBar->addAction(insertRemovePontoAct);

    //leftBar->addWidget(listaCamadas);
}

void Window::createBottomBar() {
    bottomBar = new QToolBar();
    bottomBar->setOrientation(Qt::Horizontal);
    bottomBar->setFloatable(false);
    bottomBar->setMovable(false);

    //QLabel* linhaColorLabel = new QLabel("Linha");
    //bottomBar->addWidget(linhaColorLabel);
    linhaColorButton = new QToolButton();
    linhaColorButton->setToolTip("Cor de linha");
    linhaColorButton->setAutoRaise(true);
    updateLinhaButton();
    bottomBar->addWidget(linhaColorButton);
    QObject::connect(linhaColorButton, SIGNAL(clicked()), this, SLOT(setLinhaColor()));

    //QLabel* fillColorLabel = new QLabel("Fill");
    //bottomBar->addWidget(fillColorLabel);
    fillColorButton = new QToolButton();
    updateFillButton();
    fillColorButton->setAutoRaise(true);
    fillColorButton->setToolTip("Cor de preenchimento");
    fillColorButton->setAutoRaise(true);
    bottomBar->addWidget(fillColorButton);
    QObject::connect(fillColorButton, SIGNAL(clicked()), this, SLOT(setFillColor()));

    bottomBar->addSeparator();

    QLabel* espessuraLinhaLabel = new QLabel("Espessura");
    QSpinBox* espessuraLinhaSpinBox = new QSpinBox();
    espessuraLinhaSpinBox->setSuffix(" px");
    espessuraLinhaSpinBox->setMinimum(1);
    espessuraLinhaSpinBox->setMaximum(50);
    espessuraLinhaSpinBox->setSingleStep(1);
    espessuraLinhaSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    bottomBar->addWidget(espessuraLinhaLabel);
    bottomBar->addWidget(espessuraLinhaSpinBox);
    QObject::connect(espessuraLinhaSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setEspessuraLinha(int)));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    bottomBar->addWidget(spacer);

    toggleGradeButton = new QToolButton();
    toggleGradeButton->setIcon(QIcon(":/images/grade.png"));
    toggleGradeButton->setCheckable(true);
    toggleGradeButton->setToolTip("Mostrar/ocultar grade");
    bottomBar->addWidget(toggleGradeButton);
    QObject::connect(toggleGradeButton, SIGNAL(clicked()), this, SLOT(toggleGrade()));

    QSpinBox* zoomSpinBox = new QSpinBox();
    zoomSpinBox->setSuffix("%");
    zoomSpinBox->setMinimum(25);
    zoomSpinBox->setMaximum(1000);
    zoomSpinBox->setValue(100);
    zoomSpinBox->setSingleStep(5);
    bottomBar->addWidget(zoomSpinBox);

}

void Window::setupMenus() {
    QMenu *fileMenu = new QMenu(tr("&Arquivo"), this);
    menuBar->addMenu(fileMenu);

    //fileMenu->addAction(tr("&Novo"), this, SLOT(novo()), QKeySequence::New);
    fileMenu->addAction(tr("&Abrir..."), this, SLOT(abrirElv()), QKeySequence::Open);
    fileMenu->addAction(tr("&Salvar"), this, SLOT(salvarElv()), QKeySequence::Save);
    fileMenu->addAction(tr("&Salvar como..."), this, SLOT(abrirElv()), QKeySequence::SaveAs);
    fileMenu->addAction(tr("&Exportar para SVG"), this, SLOT(exportarSVG()), QKeySequence("Ctrl+E"));
    fileMenu->addAction(tr("&Sair"), qApp, SLOT(quit()), QKeySequence::Quit);

    QMenu *helpMenu = new QMenu(tr("&Ajuda"), this);
    menuBar->addMenu(helpMenu);

    helpMenu->addAction(tr("&Sobre"), this, SLOT(about()));
}

void Window::abrirElv(const QString &path) {
    QString fileName = path;

    if(fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Abrir arquivo"), "", ".elv");

    if(!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            // TODO
        }
    }
}

bool Window::salvarElv() {
    /*
    if(documentoAtual.arqAberto.isEmpty()) {
        return salvarElvComo();
    } else {
        return
    }
    */
}

bool Window::salvarElvComo() {
    //TODO
}

bool Window::exportarSVG() {
    //TODO
}

void Window::about() {
    QMessageBox::about(this,
                       tr("Sobre Elvis"),
                       tr("Elvis é uma aplicação desenvolvida por Felipe Fernandes e Felipe Cortez para disciplina de Computação Gráfica na UFRN"));
}

void Window::setOperacao(QAction* q) {
    if(q == selecionarAct) {
        docAtual->op = SELECIONAR;
        docAtual->desenha = false;
    } else if(q == addRetanguloAct) {
        docAtual->forma = RETANGULO;
        docAtual->op = CRIACAO;
        docAtual->desenha = true;
    } else if(q == addCirculoAct) {
        docAtual->forma = CIRCULO;
        docAtual->op = CRIACAO;
        docAtual->desenha = true;
    } else if(q == addPolilinhaAct) {
        docAtual->forma = POLILINHA;
        docAtual->op = CRIACAO;
        docAtual->desenha = true;
        docAtual->linha = true;
        updateLinhaButton();
    } else if(q == addElipseAct) {
        docAtual->forma = ELIPSE;
        docAtual->op = CRIACAO;
        docAtual->desenha = true;
    } else if(q == transladarAct) {
        docAtual->op = TRANSLACAO;
        docAtual->desenha = false;
    } else if(q == copiarAct) {
        docAtual->op = COPIA;
        docAtual->desenha = false;
    } else if(q == escalarAct) {
        docAtual->op = ESCALA;
        docAtual->desenha = false;
    } else if(q == deslocarPtsAct) {
        docAtual->op = DESLOCARPONTOS;
        docAtual->desenha = false;
    } else if(q == rotacionarAct) {
        docAtual->op = ROTACAO;
        docAtual->desenha = false;
    } else if(q == insertRemovePontoAct) {
        docAtual->op = INSERT_REMOVE_PONTO;
        docAtual->desenha = false;
    }

    glArea->opBotaoDireito = false;
    glArea->desselecionaALL(); // sempre que uma opção (de desenho ou transformação) for escolhida, desselecione todos os objetos
    glArea->updateGL();
}

void Window::updateLinhaButton() {
    if(!docAtual->linha) {
        QPixmap px(20, 20);
        QPainter *p = new QPainter(&px);
        px.fill(Qt::white);
        p->setBrush(Qt::red);
        p->setPen(QPen(Qt::red, 2));
        p->drawLine(QPoint(0, 20), QPoint(20, 0));
        p->setPen(QPen(Qt::black, 1));
        p->drawLine(QPoint( 6,  6), QPoint(13,  6));
        p->drawLine(QPoint(13,  6), QPoint(13, 13));
        p->drawLine(QPoint(13, 13), QPoint( 6, 13));
        p->drawLine(QPoint( 6, 13), QPoint( 6,  7));
        delete p;
        linhaColorButton->setIcon(px);
    } else {
        QColor color = QColor::fromRgbF(docAtual->linhaColorSelecionada[0],
                                        docAtual->linhaColorSelecionada[1],
                                        docAtual->linhaColorSelecionada[2]);

        QPixmap px(20, 20);
        QPainter *p = new QPainter(&px);
        px.fill(Qt::white);
        p->setPen(QPen(color, 5));
        p->drawLine(QPoint( 2,  2), QPoint(17,  2));
        p->drawLine(QPoint(17,  2), QPoint(17, 17));
        p->drawLine(QPoint(17, 17), QPoint( 2, 17));
        p->drawLine(QPoint( 2, 17), QPoint( 2,  2));
        p->setPen(QPen(Qt::black, 1));
        p->drawLine(QPoint( 6,  6), QPoint(13,  6));
        p->drawLine(QPoint(13,  6), QPoint(13, 13));
        p->drawLine(QPoint(13, 13), QPoint( 6, 13));
        p->drawLine(QPoint( 6, 13), QPoint( 6,  7));
        delete p;
        linhaColorButton->setIcon(px);
    }
}

void Window::setLinhaColor() {
    if(QApplication::keyboardModifiers() == Qt::ControlModifier) {
        if(docAtual->linha) {
            docAtual->linha = false;
        } else {
            docAtual->linha = true;
        }
        updateLinhaButton();
    } else {
        const QColor color =
                QColorDialog::getColor(QColor::fromRgbF(docAtual->linhaColorSelecionada[0],
                                                        docAtual->linhaColorSelecionada[1],
                                                        docAtual->linhaColorSelecionada[2]),
                                                        this, "Selecionar cor");

        if (color.isValid()) {
            docAtual->linhaColorSelecionada[0] = color.redF();
            docAtual->linhaColorSelecionada[1] = color.greenF();
            docAtual->linhaColorSelecionada[2] = color.blueF();

            updateLinhaButton();
        }
    }
}

void Window::updateFillButton() {
    QPixmap px(20, 20);

    if(!docAtual->preenchimento) {
        QPainter *p = new QPainter(&px);
        px.fill(Qt::white);
        p->setBrush(Qt::red);
        p->setPen(QPen(Qt::red, 2));
        p->drawLine(QPoint(0, 20), QPoint(20, 0));
        delete p;
        fillColorButton->setIcon(px);
    } else {
        QColor color = QColor::fromRgbF(docAtual->fillColorSelecionada[0],
                                        docAtual->fillColorSelecionada[1],
                                        docAtual->fillColorSelecionada[2]);

        px.fill(color);
        fillColorButton->setIcon(px);
    }
}

void Window::setFillColor() {
    if(QApplication::keyboardModifiers() == Qt::ControlModifier) {
        docAtual->preenchimento = !docAtual->preenchimento;

        updateFillButton();
    } else {
        const QColor color =
                QColorDialog::getColor(QColor::fromRgbF(docAtual->fillColorSelecionada[0],
                                                        docAtual->fillColorSelecionada[1],
                                                        docAtual->fillColorSelecionada[2]),
                                                        this, "Selecionar cor de fill");
        if (color.isValid()) {
            docAtual->fillColorSelecionada[0] = color.redF();
            docAtual->fillColorSelecionada[1] = color.greenF();
            docAtual->fillColorSelecionada[2] = color.blueF();

            updateFillButton();
        }
    }
}

void Window::toggleGrade() {
    toggleGradeButton->setChecked(toggleGradeButton->isChecked());
    docAtual->grade = !docAtual->grade;
    glArea->updateGL();
}

void Window::setEspessuraLinha(int espessura) {
    docAtual->espessuraLinha = espessura;
}
