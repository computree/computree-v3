include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

CONFIG -= plugin

TARGET = CompuTreeBatch
TEMPLATE = app
QT += network

# pour que l'application recherche les librairies dans son propre dossier
QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN

DESTDIR = $${PLUGINSHARED_DESTDIR}

SOURCES += \
    main.cpp \
    batchdialog.cpp \
    StepResultTreeView/mytreedelegate.cpp \
    StepResultTreeView/myqstandarditem.cpp \
    StepResultTreeView/gstepmanager.cpp \
    batch.cpp \
    batchpluginmanager.cpp

HEADERS += \
    batchdialog.h \
    StepResultTreeView/mytreedelegate.h \
    StepResultTreeView/myqstandarditem.h \
    StepResultTreeView/gstepmanager.h \
    interfacesbatch.h \
    batch.h \
    batchpluginmanager.h \
    $$PLUGIN_SHARED_DIR/interfaces.h

FORMS += \
    batchdialog.ui

RESOURCES += \
    systray.qrc

INCLUDEPATH += .
INCLUDEPATH += $${PLUGIN_SHARED_DIR}
INCLUDEPATH += $${EXPORTER_SHARED_DIR}

INCLUDEPATH += $${COMPUTREE_CORE_DIR}
INCLUDEPATH += $${COMPUTREE_CORE_DIR}/src

macx {
    LIBS += $${DESTDIR}/libCompuTreeCore*.dylib
} else {
    unix{
        LIBS += $${DESTDIR}/libCompuTreeCore*.so
    }

    win32 {
        win32-g++ {
        LIBS += $${DESTDIR}/libCompuTreeCore*.a
        }

        win32-msvc* {
        LIBS += $${DESTDIR}/CompuTreeCore*.lib
        }
    }
}




































