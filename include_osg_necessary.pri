INCLUDEPATH += $$OSG_INC_PATH

LIBS += -L$$OSG_LIBS_PATH

unix {

OSG_LIB_ADD = libosg
OSG_LIB_ADD += libosgGA
OSG_LIB_ADD += libosgViewer
OSG_LIB_ADD += libosgWidget
OSG_LIB_ADD += libosgManipulator
OSG_LIB_ADD += libosgSim
OSG_LIB_ADD += libosgText
OSG_LIB_ADD += libosgUtil
OSG_LIB_ADD += libosgDB
OSG_LIB_ADD += libosgFX
OSG_LIB_ADD += libOpenThreads

    for(a, OSG_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OSG_LIBS_PATH/$${a}*) {
                USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a} was not found"
            } else {
                LIBS += $$OSG_LIBS_PATH/$${a}.so
            }
        } else {
            !exists($$OSG_LIBS_PATH/$${a}*) {
                USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a} was not found"
            } else {
                LIBS += $$OSG_LIBS_PATH/$${a}.so
            }
        }
    }
}

windows {

OSG_LIB_ADD = osg
OSG_LIB_ADD += OpenThreads
OSG_LIB_ADD += osgGA
OSG_LIB_ADD += osgViewer
OSG_LIB_ADD += osgWidget
OSG_LIB_ADD += osgManipulator
OSG_LIB_ADD += osgSim
OSG_LIB_ADD += osgText
OSG_LIB_ADD += osgUtil
OSG_LIB_ADD += osgDB
OSG_LIB_ADD += osgFX

    for(a, OSG_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OSG_LIBS_PATH/$${a}d*) {
                USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a}d was not found"
            } else {
                LIBS += -l$${a}d
            }
        } else {
            !exists($$OSG_LIBS_PATH/$${a}*) {
                USE_OSG_ERROR_MSG += "Library $$OSG_LIBS_PATH/$${a} was not found"
            } else {
                LIBS += -l$${a}
            }
        }
    }
}

isEmpty(USE_OSG_ERROR_MSG) {
    DEFINES += USE_OSG
}

