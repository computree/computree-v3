#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T15:07:29
#
#-------------------------------------------------

QT       += core gui

TARGET = plugincreator
TEMPLATE = app


contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
