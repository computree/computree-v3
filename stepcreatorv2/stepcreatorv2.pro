QT       += core gui

TARGET = stepcreatorv2
TEMPLATE = app

OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui
RCC_DIR = .rcc

SOURCES += main.cpp\
        mainwindow.cpp \
    inmodeldialog.cpp \
    outmodeldialog.cpp \
    widgets/inresultwidget.cpp \
    widgets/ingroupwidget.cpp \
    models/inresultmodel.cpp \
    models/ingroupmodel.cpp \
    models/initemmodel.cpp \
    widgets/initemwidget.cpp \
    tools.cpp \
    models/abstractinmodel.cpp \
    widgets/abstractinwidget.cpp \
    models/outresultmodel.cpp \
    models/outitemmodel.cpp \
    models/outgroupmodel.cpp \
    models/abstractoutmodel.cpp \
    widgets/outresultwidget.cpp \
    widgets/outitemwidget.cpp \
    widgets/outgroupwidget.cpp \
    widgets/abstractoutwidget.cpp

HEADERS  += mainwindow.h \
    inmodeldialog.h \
    outmodeldialog.h \
    widgets/inresultwidget.h \
    widgets/ingroupwidget.h \
    models/inresultmodel.h \
    models/ingroupmodel.h \
    models/initemmodel.h \
    widgets/initemwidget.h \
    tools.h \
    models/abstractinmodel.h \
    widgets/abstractinwidget.h \
    models/outresultmodel.h \
    models/outitemmodel.h \
    models/outgroupmodel.h \
    models/abstractoutmodel.h \
    widgets/outresultwidget.h \
    widgets/outitemwidget.h \
    widgets/outgroupwidget.h \
    widgets/abstractoutwidget.h

FORMS    += mainwindow.ui \
    inmodeldialog.ui \
    outmodeldialog.ui \
    widgets/inresultwidget.ui \
    widgets/ingroupwidget.ui \
    widgets/initemwidget.ui \
    widgets/outresultwidget.ui \
    widgets/outitemwidget.ui \
    widgets/outgroupwidget.ui


INCLUDEPATH += ./models
INCLUDEPATH += ./widgets



















