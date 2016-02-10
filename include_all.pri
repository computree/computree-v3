!exists(include_gdal.pri) {
    message("GDAL will not be used")
} else {
    include(include_gdal.pri)
}

contains(DEFINES, USE_PCL_DEFAULT) {
    !exists(include_pcl_default.pri) {
        message("PCL will not be used")
    } else {
        include(include_pcl_default.pri)
    }
} else {
    !exists(include_pcl.pri) {
        message("PCL will not be used")
    } else {
        include(include_pcl.pri)
    }
}

contains(DEFINES, USE_OPENCV_DEFAULT) {
    !exists(include_opencv_default.pri) {
        message("OPENCV will not be used")
    } else {
        include(include_opencv_default.pri)
    }
} else {
    !exists(include_opencv.pri) {
        message("OPENCV will not be used")
    } else {
        include(include_opencv.pri)
    }
}

!exists(include_osg.pri) {
    !exists(include_osg_default.pri) {
        error("File include_osg.pri not found ! Run script \"convertInclude.bat\" to create this file automatically")
    } else {
        include(include_osg_default.pri)
    }
} else {
    include(include_osg.pri)
}

!contains( DEFINES, USE_PCL ) {
    isEmpty(PLUGIN_SHARED_DIR) {
        INCLUDEPATH += ./eigen
    } else {
        INCLUDEPATH += $${PLUGIN_SHARED_DIR}/eigen
    }
}

contains( COMPUTREE, LIB_IO) {
    LIBS += -L$${PLUGINSHARED_DESTDIR}
    LIBS += -lCTLibIO
}
