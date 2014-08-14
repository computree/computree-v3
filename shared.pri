# this is the common include for all the plugins

include(destdir.pri)

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

DESTDIR = $${PLUGINSHARED_PLUGIN_DESTDIR}

TEMPLATE = lib
CONFIG += plugin

OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui
RCC_DIR = .rcc

PLUGIN_SHARED_DIR = $${CT_PREFIX}/pluginshared
PLUGIN_SHARED_INTERFACE_DIR = $${CT_PREFIX}/pluginshared

msvc:LIBS += $${PLUGINSHARED_DESTDIR}/pluginShared*.lib
win32-g++:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared*.a
linux-g++:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared*.so*
linux-g++-64:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared*.so*
mac:LIBS += $${PLUGINSHARED_DESTDIR}/libpluginShared*.dylib

msvc:DEFINES += _USE_MATH_DEFINES

INCLUDEPATH += $${PLUGIN_SHARED_INTERFACE_DIR}
