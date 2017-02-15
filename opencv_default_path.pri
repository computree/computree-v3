unix {
    OPENCV_BASE_PATH = "/usr/local/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_VERSION = 300
}

windows {
    OPENCV_BASE_PATH = "$$PWD/../Computree_dependencies/opencv/build/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}x64/vc12/lib"
    OPENCV_VERSION = 300
}
