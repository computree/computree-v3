CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

MUST_USE_PCL = 1

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = ctlibpcl

DEFINES += CTLIBPCL_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    tools/ct_pcltools.h \
    ct_pcldefines.h

SOURCES += \ 
    tools/ct_pcltools.cpp
