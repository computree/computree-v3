!exists($${GEOS_INC_PATH}) {
    USE_GEOS_ERROR_MSG += "GEOS INCLUDE directory not found ! => $${GEOS_INC_PATH}"
}

!exists($${GEOS_LIBS_PATH}) {
    USE_GEOS_ERROR_MSG += "GEOS LIBS directory not found ! => $${GEOS_LIBS_PATH}"
}
