include(all_check_dependencies.pri) # Just verify if there is problems with dependencies, do nothing else

TEMPLATE      = subdirs
SUBDIRS       = pluginshared library/ctlibio library/ctlibmetrics library/ctlibfilters library/ctliblas pluginbase ComputreeCore ComputreeGui
CONFIG       += console ordered

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
