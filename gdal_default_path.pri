unix {
    GDAL_BASE_PATH = "/usr/local/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
}

windows {
    GDAL_BASE_PATH = "$$PWD/../Computree_dependencies/gdal/"

    GDAL_INC_PATH = $${GDAL_BASE_PATH}include
    GDAL_LIBS_PATH = $${GDAL_BASE_PATH}lib
}
