unix {
    GEOS_BASE_PATH = "/usr/local/"

    GEOS_INC_PATH = "$${GEOS_BASE_PATH}include"
    GEOS_LIBS_PATH = "$${GEOS_BASE_PATH}lib"
}

windows {
    GEOS_BASE_PATH = "$$PWD/../Computree_dependencies/geos/"

    GEOS_INC_PATH = $${GEOS_BASE_PATH}include
    GEOS_LIBS_PATH = $${GEOS_BASE_PATH}lib
}
