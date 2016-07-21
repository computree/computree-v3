GSL_LIB_ADD = libgsl

INCLUDEPATH += $$GSL_INC_PATH

LIBS += -L$$GSL_LIBS_PATH

unix {
    for(a, GSL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GSL_LIBS_PATH/lib$${a}_debug*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/lib$${a}_debug was not found"
            } else {
                LIBS += -l$${a}_debug
            }
        } else {
            !exists($$GSL_LIBS_PATH/lib$${a}*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += -l$${a}
            }
        }
    }
}

windows {
    for(a, GSL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GSL_LIBS_PATH/$${a}_d*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/$${a}_d was not found"
            } else {
                LIBS += -l$${a}_d
            }
        } else {

            !exists($$GSL_LIBS_PATH/$${a}*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/$${a} was not found"
            } else {
                LIBS += -l$${a}
            }
        }
    }
}

isEmpty(USE_GSL_ERROR_MSG) {
    DEFINES += USE_GSL
}

