##### OSG ####

include(osg_default_path.pri)

exists(osg_user_path.pri) {
    include(osg_user_path.pri)
}

include(osg_check.pri)

#if OSG can be used
isEmpty(USE_OSG_ERROR_MSG) {
    include(include_osg_necessary.pri)

    isEmpty(USE_OSG_ERROR_MSG) {
        warning("OSG found and it will be used in this plugin")
    }
}

!contains(DEFINES, USE_OSG) {
    for(a, USE_OSG_ERROR_MSG) {
        warning("Error when search OSG : $${a}")
    }
    error("OSG not found, see warning above for more information")
}


##### GDAL ####

#if we must check if we can use gdal
!isEmpty(CHECK_CAN_USE_GDAL) {
    # we check if GDAL can be used
    CHECK_GDAL = 1
}

#if we want absolutely use gdal library
contains( COMPUTREE, ctlibgdal ) {
    # we set that the GDAL check test must pass
    MUST_USE_GDAL = 1
}

#if we want absolutely use gdal (the GDAL check test must pass)
!isEmpty(MUST_USE_GDAL) {
    # we check if GDAL can be used
    CHECK_GDAL = 1
}

#if we must check if gdal can be used
!isEmpty(CHECK_GDAL) {
    include(gdal_default_path.pri)

    exists(gdal_user_path.pri) {
        include(gdal_user_path.pri)
    }

    include(gdal_check.pri)

    #if GDAL can be used
    isEmpty(USE_GDAL_ERROR_MSG) {
        include(include_gdal_necessary.pri)

        isEmpty(USE_GDAL_ERROR_MSG) {
            warning("GDAL found and it will be used in this plugin")
        }
    }
}

#if we must absolutely use GDAL
!isEmpty(MUST_USE_GDAL) {
    !contains(DEFINES, USE_GDAL) {
        for(a, USE_GDAL_ERROR_MSG) {
            warning("Error when search GDAL : $${a}")
        }
        error("GDAL not found, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_GDAL) {
        !contains(DEFINES, USE_GDAL) {
            warning(This plugin can use GDAL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_GDAL_ERROR_MSG) {
                for(a, USE_GDAL_ERROR_MSG) {
                    warning("Error when search GDAL : $${a}")
                }
            }
        }
    }
}

##### GSL ####

#if we must check if we can use gsl
!isEmpty(CHECK_CAN_USE_GSL) {
    # we check if GSL can be used
    CHECK_GSL = 1
}

#if we want absolutely use gsl library
contains( COMPUTREE, ctlibgsl ) {
    # we set that the GSL check test must pass
    MUST_USE_GSL = 1
}

#if we want absolutely use gsl (the GSL check test must pass)
!isEmpty(MUST_USE_GSL) {
    # we check if GSL can be used
    CHECK_GSL = 1
}

#if we must check if gsl can be used
!isEmpty(CHECK_GSL) {
    include(gsl_default_path.pri)

    exists(gsl_user_path.pri) {
        include(gsl_user_path.pri)
    }

    include(gsl_check.pri)

    #if GSL can be used
    isEmpty(USE_GSL_ERROR_MSG) {
        include(include_gsl_necessary.pri)

        isEmpty(USE_GSL_ERROR_MSG) {
            warning("GSL found and it will be used in this plugin")
        }
    }
}

#if we must absolutely use GSL
!isEmpty(MUST_USE_GSL) {
    !contains(DEFINES, USE_GSL) {
        for(a, USE_GSL_ERROR_MSG) {
            warning("Error when search GSL : $${a}")
        }
        error("GSL not found, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_GSL) {
        !contains(DEFINES, USE_GSL) {
            warning(This plugin can use GSL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_GSL_ERROR_MSG) {
                for(a, USE_GSL_ERROR_MSG) {
                    warning("Error when search GSL : $${a}")
                }
            }
        }
    }
}

##### PCL ####

#if we must check if we can use pcl
!isEmpty(CHECK_CAN_USE_PCL) {
    # we check if PCL can be used
    CHECK_PCL = 1
}

#if we want absolutely use pcl library
contains( COMPUTREE, ctlibpcl ) {
    # we set that the PCL check test must pass
    MUST_USE_PCL = 1
}

#if we want absolutely use pcl (the PCL check test must pass)
!isEmpty(MUST_USE_PCL) {
    # we check if PCL can be used
    CHECK_PCL = 1
}

#if we must check if pcl can be used
!isEmpty(CHECK_PCL) {
    include(pcl_default_path.pri)

    exists(pcl_user_path.pri) {
        include(pcl_user_path.pri)
    }

    include(pcl_check.pri)

    #if PCL can be used
    isEmpty(USE_PCL_ERROR_MSG) {
        include(include_pcl_necessary.pri)

        isEmpty(USE_PCL_ERROR_MSG) {
            warning("PCL found and it will be used in this plugin")
        }
    }
}

#if we must absolutely use PCL
!isEmpty(MUST_USE_PCL) {
    !contains(DEFINES, USE_PCL) {
        for(a, USE_PCL_ERROR_MSG) {
            warning("Error when search PCL : $${a}")
        }
        error("PCL not found, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_PCL) {
        !contains(DEFINES, USE_PCL) {
            warning(This plugin can use PCL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_PCL_ERROR_MSG) {
                for(a, USE_PCL_ERROR_MSG) {
                    warning("Error when search PCL : $${a}")
                }
            }
        }
    }
}

##### OPENCV ####

#if we must check if we can use opencv
!isEmpty(CHECK_CAN_USE_OPENCV) {
    # we check if OPENCV can be used
    CHECK_OPENCV = 1
}

#if we want absolutely use opencv library
contains( COMPUTREE, ctlibopencv ) {
    # we set that the OPENCV check test must pass
    MUST_USE_OPENCV = 1
}

#if we want absolutely use opencv (the OPENCV check test must pass)
!isEmpty(MUST_USE_OPENCV) {
    # we check if OPENCV can be used
    CHECK_OPENCV = 1
}

#if we must check if opencv can be used
!isEmpty(CHECK_OPENCV) {
    include(opencv_default_path.pri)

    exists(opencv_user_path.pri) {
        include(opencv_user_path.pri)
    }

    include(opencv_check.pri)

    #if OPENCV can be used
    isEmpty(USE_OPENCV_ERROR_MSG) {
        include(include_opencv_necessary.pri)

        isEmpty(USE_OPENCV_ERROR_MSG) {
            warning("OPENCV found and it will be used in this plugin")
        }
    }
}

#if we must absolutely use OPENCV
!isEmpty(MUST_USE_OPENCV) {
    !contains(DEFINES, USE_OPENCV) {
        for(a, USE_OPENCV_ERROR_MSG) {
            warning("Error when search OPENCV : $${a}")
        }
        error("OPENCV not found, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_OPENCV) {
        !contains(DEFINES, USE_OPENCV) {
            warning(This plugin can use OPENCV but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_OPENCV_ERROR_MSG) {
                for(a, USE_OPENCV_ERROR_MSG) {
                    warning("Error when search OPENCV : $${a}")
                }
            }
        }
    }
}

##### EIGEN IF PCL ######

!contains( DEFINES, USE_PCL ) {
    isEmpty(PLUGIN_SHARED_DIR) {
        INCLUDEPATH += ./eigen
    } else {
        INCLUDEPATH += $${PLUGIN_SHARED_DIR}/eigen
    }
}
