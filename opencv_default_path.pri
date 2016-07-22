unix {
    OPENCV_BASE_PATH = "/usr/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_VERSION = 300
}

windows {
    OPENCV_BASE_PATH = "C:/opencv/build/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_VERSION = 300
}
