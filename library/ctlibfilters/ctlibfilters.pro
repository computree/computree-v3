CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = ctlibfilters

DEFINES += CTLIBFILTERS_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \ 
    ctlibfilters_global.h \
    filters/abstract/ct_abstractfilter_xyz.h \
    filters/ct_filteringridt_xyz.h \
    filters/ct_filteringridt_xyz.hpp \
    filters/operation/ct_gridmaximum.h \
    filters/operation/ct_gridmean.h \
    filters/operation/ct_gridminimum.h

SOURCES += \
    filters/abstract/ct_abstractfilter_xyz.cpp \
    filters/operation/ct_gridmaximum.cpp \
    filters/operation/ct_gridmean.cpp \
    filters/operation/ct_gridminimum.cpp
