# this is the common include for all the plugins

# to make qmake more faster :
CONFIG -= depend_includepath

include(destdir.pri)

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

DESTDIR = $${PLUGINSHARED_PLUGIN_DESTDIR}

TEMPLATE = lib
CONFIG += plugin

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    UI_DIR = debug/.ui
    RCC_DIR = debug/.rcc
} else {
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    UI_DIR = release/.ui
    RCC_DIR = release/.rcc
}

COMPUTREE_BATCH_PLUGIN_DESTDIR = $${PLUGINSHARED_DESTDIR}/bplugins
COMPUTREE_CORE_DIR = $${CT_PREFIX}/ComputreeCore

PLUGIN_SHARED_DIR = $${CT_PREFIX}/pluginshared
PLUGIN_SHARED_INTERFACE_DIR = $${CT_PREFIX}/pluginshared

msvc:LIBS += $${PLUGINSHARED_DESTDIR}/pluginShared.lib
win32-g++:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared.a
linux-g++:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared.so*
linux-g++-64:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared.so*
mac:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared.dylib

msvc:DEFINES += _USE_MATH_DEFINES

INCLUDEPATH += $${PLUGIN_SHARED_INTERFACE_DIR}
