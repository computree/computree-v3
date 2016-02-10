CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = CTLibIO

DEFINES += CTLIBIO_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \ 
    readers/ct_reader_ascrgb.h \
    readers/ct_reader_ascrgb_def_models.h \
    readers/ct_reader_gdal.h \
    readers/ct_reader_gdal_def_models.h \
    readers/ct_reader_larchitect_grid.h \
    readers/ct_reader_larchitect_grid_def_models.h \
    readers/ct_reader_las.h \
    readers/ct_reader_las_def_models.h \
    readers/ct_reader_opf.h \
    readers/ct_reader_opf_def_models.h \
    readers/ct_reader_terrascanprj.h \
    readers/ct_reader_terrascanprj_def_models.h \
    readers/ct_reader_xyb.h \
    readers/ct_reader_xyb_def_models.h \
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
    ctlibio_global.h \
    readers/headers/ct_lasheader.h \
    itemdrawable/las/ct_stdlaspointsattributescontainer.h \
    readers/ct_reader_obj.h \
    readers/ct_reader_obj_def_models.h \
    filters/las/abstract/ct_abstractfilter_las.h \
    readers/ct_reader_asciigrid3d.h \
    readers/ct_reader_asciigrid3d_def_models.h \
    tools/las/ct_laspointformat.h \
    exporters/ct_exporter_las.h \
    readers/ct_reader_pgm.h \
    readers/ct_reader_pgm_def_models.h \
    readers/ct_reader_pbm.h \
    readers/ct_reader_pbm_def_models.h \
    readers/ct_reader_points_ascii.h \
    readers/ct_reader_points_ascii_def_models.h

SOURCES += \ 
    readers/ct_reader_ascrgb.cpp \
    readers/ct_reader_gdal.cpp \
    readers/ct_reader_larchitect_grid.cpp \
    readers/ct_reader_las.cpp \
    readers/ct_reader_opf.cpp \
    readers/ct_reader_terrascanprj.cpp \
    readers/ct_reader_xyb.cpp \
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
    readers/ct_reader_obj.cpp \
    filters/las/abstract/ct_abstractfilter_las.cpp \
    readers/ct_reader_asciigrid3d.cpp \
    exporters/ct_exporter_las.cpp \
    readers/ct_reader_pgm.cpp \
    readers/ct_reader_pbm.cpp \
    readers/ct_reader_points_ascii.cpp
