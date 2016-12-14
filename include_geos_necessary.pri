GEOS_LIB_ADD = geos

INCLUDEPATH += $$GEOS_INC_PATH

LIBS += -L$$GEOS_LIBS_PATH

unix {
    for(a, GEOS_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GEOS_LIBS_PATH/lib$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$GEOS_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$GEOS_LIBS_PATH/lib$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$GEOS_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

windows {
    for(a, GEOS_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GEOS_LIBS_PATH/$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/$${a}_i was not found"
            } else {
                LIBS += -l$${a}
            }
        } else {

            !exists($$GEOS_LIBS_PATH/$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/$${a}_i was not found"
            } else {
                LIBS += -l$${a}
            }
        }
    }
}

isEmpty(USE_GEOS_ERROR_MSG) {
    DEFINES += USE_GEOS
}

