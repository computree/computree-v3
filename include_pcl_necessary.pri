PCL_LIB_ADD =
PCL_LIB_ADD += pcl_common
PCL_LIB_ADD += pcl_features
PCL_LIB_ADD += pcl_filters
PCL_LIB_ADD += pcl_io_ply
PCL_LIB_ADD += pcl_io
PCL_LIB_ADD += pcl_kdtree
PCL_LIB_ADD += pcl_keypoints
PCL_LIB_ADD += pcl_octree
PCL_LIB_ADD += pcl_outofcore
PCL_LIB_ADD += pcl_people
PCL_LIB_ADD += pcl_recognition
PCL_LIB_ADD += pcl_registration
PCL_LIB_ADD += pcl_sample_consensus
PCL_LIB_ADD += pcl_search
PCL_LIB_ADD += pcl_segmentation
PCL_LIB_ADD += pcl_surface
PCL_LIB_ADD += pcl_tracking
PCL_LIB_ADD += pcl_visualization

#BOOST_LIB_ADD =
#BOOST_LIB_ADD += boost_system
#BOOST_LIB_ADD += boost_thread

INCLUDEPATH += $$PCL_INC_PATH
INCLUDEPATH += $$EIGEN_INC_PATH
INCLUDEPATH += $$QHULL_INC_PATH
INCLUDEPATH += $$FLANN_INC_PATH
INCLUDEPATH += $$BOOST_INC_PATH

DEPENDPATH  += $$QHULL_INC_PATH

LIBS += -L$$PCL_LIBS_PATH
LIBS += -L$$BOOST_LIBS_PATH
LIBS += -L$$QHULL_LIBS_PATH

unix {
    QHULL_LIB_ADD =
    QHULL_LIB_ADD += qhull

    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$PCL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$PCL_LIBS_PATH/lib$${a}.so
            }
        }
    }

    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$QHULL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += $$QHULL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

windows {
QHULL_LIB_ADD =
#QHULL_LIB_ADD += qhull
#QHULL_LIB_ADD += qhull_p
#QHULL_LIB_ADD += qhullcpp
QHULL_LIB_ADD += qhullstatic
QHULL_LIB_ADD += qhullstatic_p

    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/$${a}_debug*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a}_debug was not found"
            } else {
                LIBS += -l$${a}_debug
            }
        } else {

            !exists($$PCL_LIBS_PATH/$${a}_release*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a}_release was not found"
            } else {
                LIBS += -l$${a}_release
            }
        }
    }


    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/$${a}_d*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a}_d was not found"
            } else {
                LIBS += -l$${a}_d
            }
        } else {
            !exists($$QHULL_LIBS_PATH/$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                LIBS += -l$${a}
            }
        }
    }
}

greaterThan(QT_MAJOR_VERSION, 4) {
    load(moc)
    QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE
}

isEmpty(USE_PCL_ERROR_MSG) {
    DEFINES += USE_PCL
    DEFINES += USE_BOOST
}

