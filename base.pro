TEMPLATE      = subdirs
SUBDIRS       = pluginshared library/ctlibio library/ctlibmetrics library/ctlibfilters library/ctliblas pluginbase ComputreeCore ComputreeGui
CONFIG       += console ordered

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
