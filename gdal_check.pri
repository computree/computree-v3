!exists($${GDAL_INC_PATH}) {
    USE_GDAL_ERROR_MSG += "GDAL INCLUDE directory not found ! => $${GDAL_INC_PATH}"
}

!exists($${GDAL_LIBS_PATH}) {
    USE_GDAL_ERROR_MSG += "GDAL LIBS directory not found ! => $${GDAL_LIBS_PATH}"
}
