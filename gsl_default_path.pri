unix {
    GSL_BASE_PATH = "/usr/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include/gsl"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib/x86_64-linux-gnu"
}

windows {
    GSL_BASE_PATH = "$$PWD/../Computree_dependencies/libgsl-2.1/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib/x64"
}
