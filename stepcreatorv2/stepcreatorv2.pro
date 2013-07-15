QT       += core gui

TARGET = stepcreatorv2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    inmodeldialog.cpp \
    outmodeldialog.cpp \
    widgets/inresultwidget.cpp \
    widgets/ingroupwidget.cpp \
    models/inresultmodel.cpp \
    models/ingroupmodel.cpp \
    models/initemmodel.cpp \
    widgets/abstractwidget.cpp \
    widgets/initemwidget.cpp \
    tools.cpp \
    models/abstractmodel.cpp

HEADERS  += mainwindow.h \
    inmodeldialog.h \
    outmodeldialog.h \
    widgets/inresultwidget.h \
    widgets/ingroupwidget.h \
    models/inresultmodel.h \
    models/ingroupmodel.h \
    models/initemmodel.h \
    widgets/abstractwidget.h \
    widgets/initemwidget.h \
    tools.h \
    models/abstractmodel.h

FORMS    += mainwindow.ui \
    inmodeldialog.ui \
    outmodeldialog.ui \
    widgets/inresultwidget.ui \
    widgets/ingroupwidget.ui \
    widgets/initemwidget.ui


INCLUDEPATH += ./models
INCLUDEPATH += ./widgets



















