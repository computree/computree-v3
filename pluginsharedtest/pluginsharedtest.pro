#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T09:42:59
#
#-------------------------------------------------
include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

QT       += testlib

QT       -= gui

TARGET = tst_modeltest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= plugin

TEMPLATE = app


SOURCES += \
    main.cpp \
    tst_modeltest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    tst_modeltest.h
