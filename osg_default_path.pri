unix {
    OSG_BASE_PATH = "/usr/"
    OSG_INC_PATH = $${OSG_BASE_PATH}include/osg
    OSG_LIBS_PATH = $${OSG_BASE_PATH}lib/x86_64-linux-gnu
}

windows {
    OSG_BASE_PATH = "$$PWD/../Computree_dependencies/OpenSceneGraph/"
    OSG_INC_PATH = $${OSG_BASE_PATH}include
    OSG_LIBS_PATH = $${OSG_BASE_PATH}lib
}
