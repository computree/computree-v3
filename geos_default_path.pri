unix {
    GEOS_BASE_PATH = "/usr/"

    GEOS_INC_PATH = "$${GEOS_BASE_PATH}include/geos"
    GEOS_LIBS_PATH = "$${GEOS_BASE_PATH}lib"
}

windows {
    GEOS_BASE_PATH = "C:/geos/"

    GEOS_INC_PATH = $${GEOS_BASE_PATH}include
    GEOS_LIBS_PATH = $${GEOS_BASE_PATH}lib
}
