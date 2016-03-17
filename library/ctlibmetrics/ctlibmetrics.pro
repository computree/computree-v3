CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = ctlibmetrics

DEFINES += CTLIBMETRICS_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \ 
    ctlibmetrics_global.h \
    ct_metric/abstract/ct_abstractmetric_raster.h \
    ct_metric/abstract/ct_abstractmetric_xyz.h \
    ct_metric/points/ct_cloudmetrics.h \
    tools/ct_valueandbool.h \
    ct_metric/abstract/ct_abstractmetricgeneric.h

SOURCES += \
    ct_metric/abstract/ct_abstractmetric_raster.cpp \
    ct_metric/abstract/ct_abstractmetric_xyz.cpp \
    ct_metric/points/ct_cloudmetrics.cpp \
    ct_metric/abstract/ct_abstractmetricgeneric.cpp
