TEMPLATE      = subdirs
SUBDIRS       = pluginshared pluginbase ComputreeCore ComputreeGui
CONFIG       += console ordered

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
