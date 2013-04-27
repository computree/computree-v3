include(../../pluginsharedv2/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)
include(_LISEZ_MOI_.pri)

TARGET = plug_tutorialfr

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    tufr_pluginentry.h \
    tufr_steppluginmanager.h
SOURCES += \
    tufr_pluginentry.cpp \
    tufr_steppluginmanager.cpp

OTHER_FILES +=








