include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

CONFIG -= plugin
TARGET = CompuTreeCore
TEMPLATE = lib

QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $${PLUGINSHARED_DESTDIR}
DEFINES += COMPUTREECORE_LIBRARY

INCLUDEPATH += ./
INCLUDEPATH += ./src

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    src/cdm_configfile.h \
    computreeCore_global.h \
    src/cdm_stepmanageroptions.h \
    src/cdm_stepmanager.h \
    src/cdm_pluginmanager.h \
    src/cdm_scriptmanagerabstract.h \
    src/cdm_scriptmanagerxml.h \
    src/cdm_stdiolistener.h \
    src/cdm_stdioparser.h \
    src/parserAction/cdm_parseractionmanager.h \
    src/parserAction/cdm_iparseraction.h \
    src/parserAction/cdm_abstractparseraction.h \
    src/parserAction/cdm_parseractiongetplugindir.h \
    src/parserAction/cdm_parseractionsetplugindir.h \
    src/computreeCoreDefaultIOCmd.h \
    src/parserAction/cdm_parseractionlaunchstepmanager.h \
    src/cdm_actionsmanager.h \
    src/cdm_log.h \
    src/cdm_tools.h \
    src/cdm_internationalization.h

SOURCES += \
    src/cdm_configfile.cpp \
    src/cdm_stepmanageroptions.cpp \
    src/cdm_stepmanager.cpp \
    src/cdm_pluginmanager.cpp \
    src/cdm_scriptmanagerabstract.cpp \
    src/cdm_scriptmanagerxml.cpp \
    src/cdm_stdiolistener.cpp \
    src/cdm_stdioparser.cpp \
    src/parserAction/cdm_parseractionmanager.cpp \
    src/parserAction/cdm_abstractparseraction.cpp \
    src/parserAction/cdm_parseractiongetplugindir.cpp \
    src/parserAction/cdm_parseractionsetplugindir.cpp \
    src/parserAction/cdm_parseractionlaunchstepmanager.cpp \
    src/cdm_actionsmanager.cpp \
    src/cdm_log.cpp \
    src/cdm_tools.cpp \
    src/cdm_internationalization.cpp
