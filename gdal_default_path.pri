unix {
    GDAL_BASE_PATH = "/usr/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include/gdal"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
}

windows {
    GDAL_BASE_PATH = "C:/gdal/"

    GDAL_INC_PATH = $${GDAL_BASE_PATH}include
    GDAL_LIBS_PATH = $${GDAL_BASE_PATH}lib
}
