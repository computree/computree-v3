include(../../pluginsharedv2/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)
include(_LISEZ_MOI_.pri)

TARGET = plug_tutorialfr

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    tufr_pluginentry.h \
    tufr_steppluginmanager.h \
    step/tufr_steptutorial01.h \
    step/tufr_steptutorial02.h \
    step/tufr_steptutorial03.h \
    step/tufr_steptutorial04.h \
    step/tufr_steptutorial05.h

SOURCES += \
    tufr_pluginentry.cpp \
    tufr_steppluginmanager.cpp \
    step/tufr_steptutorial01.cpp \
    step/tufr_steptutorial02.cpp \
    step/tufr_steptutorial03.cpp \
    step/tufr_steptutorial04.cpp \
    step/tufr_steptutorial05.cpp

OTHER_FILES +=








