contains(DEFINES, USE_OPENCV_DEFAULT) {
    unix {
        OPENCV_INC_PATH = "/usr/local/include"
        OPENCV_LIBS_PATH = "/usr/local/lib"
    }

    windows {
        OPENCV_INC_PATH = "C:/opencv/build/include"
        OPENCV_LIBS_PATH = "C:/opencv/build/x64/vc12/lib"
    }
}
 
!isEmpty(OPENCV_INC_PATH) {   
    !exists($${OPENCV_INC_PATH}) {
        error( "OPENCV INCLUDE directory not found ! => " $${OPENCV_INC_PATH})
    }

    !exists($${OPENCV_LIBS_PATH}) {
        error( "OPENCV LIBS directory not found ! => " $${OPENCV_LIBS_PATH})
    }

    CONFIG(debug, debug|release) {
        error(Only the release version can be compiled with OpenCV in unix version)
    }

    OPENCV_LIB_ADD =
    OPENCV_LIB_ADD += opencv_ts
    OPENCV_LIB_ADD += opencv_world

    DEFINES += USE_OPENCV

    INCLUDEPATH += $$OPENCV_INC_PATH
    LIBS += -L$$OPENCV_LIBS_PATH

    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            LIBS += -l$${a}d
        } else {
            LIBS += -l$${a}
        }
    }

    message(You want to use OPENCV : ok)
} else { 
    message(OPENCV will not be used )
} 
