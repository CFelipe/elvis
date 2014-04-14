#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    void setupFileMenu();
    void setupHelpMenu();
    void createToolbars();
    void createStatusBar();

public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());
};

#endif // WINDOW_H
