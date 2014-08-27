#-------------------------------------------------
#
# Project created by QtCreator 2014-08-13T09:42:59
#
#-------------------------------------------------
include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

PLUGIN_BASE_INTERFACE_DIR = $${CT_PREFIX}/pluginbase

msvc:LIBS += $${PLUGINSHARED_PLUGIN_DESTDIR}/plug_base.lib
win32-g++:LIBS += $${PLUGINSHARED_PLUGIN_DESTDIR}/libplug_base.a
linux-g++:LIBS += $${PLUGINSHARED_PLUGIN_DESTDIR}/libplug_base.so*
linux-g++-64:LIBS += $${PLUGINSHARED_PLUGIN_DESTDIR}/libplug_base.so*
mac:LIBS += $${PLUGINSHARED_PLUGIN_DESTDIR}/libplug_base.dylib

INCLUDEPATH += $${PLUGIN_BASE_INTERFACE_DIR}

QT       += testlib

QT       -= gui

TARGET = tst_modeltest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= plugin

TEMPLATE = app


SOURCES += \
    main.cpp \
    tst_modeltest.cpp \
    steptest.cpp \
    cloudtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    tst_modeltest.h \
    steptest.h \
    cloudtest.h
