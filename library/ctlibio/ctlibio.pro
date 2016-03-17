CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = ctlibio

DEFINES += CTLIBIO_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \ 
    readers/ct_reader_ascrgb.h \
    readers/ct_reader_ascrgb_def_models.h \
    readers/ct_reader_gdal.h \
    readers/ct_reader_gdal_def_models.h \
    readers/ct_reader_larchitect_grid.h \
    readers/ct_reader_larchitect_grid_def_models.h \
    readers/ct_reader_opf.h \
    readers/ct_reader_opf_def_models.h \
    readers/ct_reader_terrascanprj.h \
    readers/ct_reader_terrascanprj_def_models.h \
    readers/ct_reader_xyb.h \
    readers/ct_reader_xyb_def_models.h \
    ctlibio_global.h \
    readers/ct_reader_obj.h \
    readers/ct_reader_obj_def_models.h \
    readers/ct_reader_asciigrid3d.h \
    readers/ct_reader_asciigrid3d_def_models.h \
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
    readers/ct_reader_opf.cpp \
    readers/ct_reader_terrascanprj.cpp \
    readers/ct_reader_xyb.cpp \
    readers/ct_reader_obj.cpp \
    readers/ct_reader_asciigrid3d.cpp \
    readers/ct_reader_pgm.cpp \
    readers/ct_reader_pbm.cpp \
    readers/ct_reader_points_ascii.cpp
