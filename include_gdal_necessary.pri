GDAL_LIB_ADD = gdal

INCLUDEPATH += $$GDAL_INC_PATH

LIBS += -L$$GDAL_LIBS_PATH

unix {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$GDAL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$GDAL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

windows {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/$${a}_i*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}_i was not found"
            } else {
                LIBS += -l$${a}_i
            }
        } else {

            !exists($$GDAL_LIBS_PATH/$${a}_i*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}_i was not found"
            } else {
                LIBS += -l$${a}_i
            }
        }
    }
}

isEmpty(USE_GDAL_ERROR_MSG) {
    DEFINES += USE_GDAL
}

