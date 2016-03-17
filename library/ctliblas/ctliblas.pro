CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

# need filters and metrics
COMPUTREE += ctlibfilters ctlibmetrics

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)
include($${CT_PREFIX}/include_ct_library.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = ctliblas

DEFINES += CTLIBLAS_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \ 
    readers/ct_reader_las.h \
    readers/ct_reader_las_def_models.h \
    tools/las/abstract/ct_abstractlaspointformat.h \
    tools/las/ct_lascolorpacket.h \
    tools/las/ct_lasdata.h \
    tools/las/ct_lasdefine.h \
    tools/las/ct_lasgpspacket.h \
    tools/las/ct_lasnirpacket.h \
    tools/las/ct_laspointformat0.h \
    tools/las/ct_laspointformat1.h \
    tools/las/ct_laspointformat2.h \
    tools/las/ct_laspointformat3.h \
    tools/las/ct_laspointformat4.h \
    tools/las/ct_laspointformat5.h \
    tools/las/ct_laspointformat6.h \
    tools/las/ct_laspointformat7.h \
    tools/las/ct_laspointformat8.h \
    tools/las/ct_laspointformat9.h \
    tools/las/ct_laspointformat10.h \
    tools/las/ct_laspointinfo.h \
    tools/las/ct_laswavepacket.h \
    ctliblas_global.h \
    readers/headers/ct_lasheader.h \
    itemdrawable/las/ct_stdlaspointsattributescontainer.h \
    exporters/ct_exporter_las.h \
    filters/abstract/ct_abstractfilter_las.h

SOURCES += \ 
    readers/ct_reader_las.cpp \
    tools/las/abstract/ct_abstractlaspointformat.cpp \
    tools/las/ct_lascolorpacket.cpp \
    tools/las/ct_lasdata.cpp \
    tools/las/ct_lasgpspacket.cpp \
    tools/las/ct_lasnirpacket.cpp \
    tools/las/ct_laspointformat0.cpp \
    tools/las/ct_laspointformat1.cpp \
    tools/las/ct_laspointformat2.cpp \
    tools/las/ct_laspointformat3.cpp \
    tools/las/ct_laspointformat4.cpp \
    tools/las/ct_laspointformat5.cpp \
    tools/las/ct_laspointformat6.cpp \
    tools/las/ct_laspointformat7.cpp \
    tools/las/ct_laspointformat8.cpp \
    tools/las/ct_laspointformat9.cpp \
    tools/las/ct_laspointformat10.cpp \
    tools/las/ct_laspointinfo.cpp \
    tools/las/ct_laswavepacket.cpp \
    readers/headers/ct_lasheader.cpp \
    itemdrawable/las/ct_stdlaspointsattributescontainer.cpp \
    exporters/ct_exporter_las.cpp \
    filters/abstract/ct_abstractfilter_las.cpp
