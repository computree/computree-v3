INCLUDEPATH += $$OPENCV_INC_PATH

LIBS += -L$$OPENCV_LIBS_PATH

unix {

OPENCV_LIB_ADD =
OPENCV_LIB_ADD += opencv_calib3d
OPENCV_LIB_ADD += opencv_core
OPENCV_LIB_ADD += opencv_features2d
OPENCV_LIB_ADD += opencv_flann
OPENCV_LIB_ADD += opencv_highgui
OPENCV_LIB_ADD += opencv_imgcodecs
OPENCV_LIB_ADD += opencv_imgproc
OPENCV_LIB_ADD += opencv_ml
OPENCV_LIB_ADD += opencv_objdetect
OPENCV_LIB_ADD += opencv_photo
OPENCV_LIB_ADD += opencv_shape
OPENCV_LIB_ADD += opencv_stitching
OPENCV_LIB_ADD += opencv_superres
OPENCV_LIB_ADD += opencv_video
OPENCV_LIB_ADD += opencv_videoio
OPENCV_LIB_ADD += opencv_videostab
OPENCV_LIB_ADD += opencv_viz

    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$OPENCV_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$OPENCV_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

windows {

OPENCV_LIB_ADD =
OPENCV_LIB_ADD += opencv_ts
OPENCV_LIB_ADD += opencv_world

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

