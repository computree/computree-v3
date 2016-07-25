OPENCV_LIB_ADD =
OPENCV_LIB_ADD += opencv_ts
OPENCV_LIB_ADD += opencv_world

INCLUDEPATH += $$OPENCV_INC_PATH

LIBS += -L$$OPENCV_LIBS_PATH

unix {
    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION}d*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION}d was not found"
            } else {
                LIBS += -l$${a}$${OPENCV_VERSION}d
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a}$${OPENCV_VERSION} was not found"
            } else {
                LIBS += -l$${a}$${OPENCV_VERSION}
            }
        }
    }
}

windows {
    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}d*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}d was not found"
            } else {
                LIBS += -l$${a}$${OPENCV_VERSION}d
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION} was not found"
            } else {
                LIBS += -l$${a}$${OPENCV_VERSION}
            }
        }
    }
}

isEmpty(USE_OPENCV_ERROR_MSG) {
    DEFINES += USE_OPENCV
}

