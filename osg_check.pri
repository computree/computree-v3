!exists($${OSG_INC_PATH}) {
    USE_OSG_ERROR_MSG += "OSG INCLUDE directory not found ! => $${OSG_INC_PATH}"
}

!exists($${OSG_LIBS_PATH}) {
    USE_OSG_ERROR_MSG += "OSG LIBS directory not found ! => $${OSG_LIBS_PATH}"
}
