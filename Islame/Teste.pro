#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T22:03:38
#
#-------------------------------------------------


QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = glQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
   # polilinha.cpp \
  # quadrilatero.cpp \
# circulo.cpp

HEADERS  += mainwindow.h \
    polilinha.h \
    Objeto.h \
    Quadrilatero.h

FORMS    += mainwindow.ui

INCPATH = -I/usr/include -I/usr/X11R6/include
LIBS = -lglut -lGLU -lGL -lm -lpthread
LIBPATH = -L/usr/lib -L/usr/X11R6/lib
