#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T22:40:28
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = elvis
TEMPLATE = app


SOURCES += main.cpp\
    window.cpp \
    glwidget.cpp \
    objeto.cpp \
    retangulo.cpp \
    vertice.cpp \
    elipse.cpp \
    circulo.cpp

HEADERS  += \
    window.h \
    glwidget.h \
    objeto.h \
    retangulo.h \
    vertice.h \
    elipse.h \
    circulo.h

FORMS    += mainwindow.ui
