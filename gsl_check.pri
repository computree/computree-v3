!exists($${GSL_INC_PATH}) {
    USE_GSL_ERROR_MSG += "GSL INCLUDE directory not found ! => $${GSL_INC_PATH}"
}

!exists($${GSL_LIBS_PATH}) {
    USE_GSL_ERROR_MSG += "GSL LIBS directory not found ! => $${GSL_LIBS_PATH}"
}
