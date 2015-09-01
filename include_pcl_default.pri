contains(DEFINES, USE_PCL_DEFAULT) {
    unix {
        PCL_LIBS_PATH = "/usr/lib"
        PCL_INC_PATH = "/usr/include/pcl-1.7"
        EIGEN_INC_PATH = "/usr/include/eigen3"
        QHULL_INC_PATH = "/usr/include"
        FLANN_INC_PATH = "/usr/include"
        BOOST_LIBS_PATH = "/usr/lib"
        BOOST_INC_PATH = "/usr/include"
    }

    windows {
        PCL_LIBS_PATH = "C:/Program Files/PCL 1.7.2/lib"
        PCL_INC_PATH = "C:/Program Files/PCL 1.7.2/include/pcl-1.7"
        EIGEN_INC_PATH = "C:/Program Files/PCL 1.7.2/3rdParty/Eigen/eigen3"
        QHULL_INC_PATH = "C:/Program Files/PCL 1.7.2/3rdParty/Qhull/include"
        FLANN_INC_PATH = "C:/Program Files/PCL 1.7.2/3rdParty/FLANN/include"
        BOOST_LIBS_PATH = "C:/Program Files/PCL 1.7.2/3rdParty/Boost/lib"
        BOOST_INC_PATH = "C:/Program Files/PCL 1.7.2/3rdParty/Boost/include/boost-1_57"
    }
}
 
!isEmpty(PCL_INC_PATH) {  
    !exists($${PCL_INC_PATH}) {
        error( "PCL INCLUDE directory not found ! => " $${PCL_INC_PATH})
    }

    !exists($${PCL_LIBS_PATH}) {
        error( "PCL LIBS directory not found ! => " $${PCL_LIBS_PATH})
    }

    !exists($${EIGEN_INC_PATH}) {
        error( "EIGEN INCLUDE directory not found ! =>" $${EIGEN_INC_PATH} )
    }

    !exists($${QHULL_INC_PATH}) {
        error( "QHULL INCLUDE directory not found ! =>" $${QHULL_INC_PATH} )
    }

    !exists($${FLANN_INC_PATH}) {
        error( "FLANN INCLUDE directory not found ! =>" $${FLANN_INC_PATH} )
    }

    !exists($${BOOST_INC_PATH}) {
        error( "BOOST INCLUDE directory not found ! =>" $${BOOST_INC_PATH} )
    }

    !exists($${BOOST_LIBS_PATH}) {
        error( "BOOST LIBS directory not found ! =>" $${BOOST_LIBS_PATH} )
    }

    CONFIG(debug, debug|release) {
        error(Only the release version can be compiled with PCL in unix version)
    }

    DEFINES += USE_PCL
    DEFINES += USE_BOOST

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

    INCLUDEPATH += $$PCL_INC_PATH
    INCLUDEPATH += $$EIGEN_INC_PATH
    INCLUDEPATH += $$QHULL_INC_PATH
    INCLUDEPATH += $$FLANN_INC_PATH
    INCLUDEPATH += $$BOOST_INC_PATH

    LIBS += -L$$PCL_LIBS_PATH
    LIBS += -L$$BOOST_LIBS_PATH

    unix {
        for(a, PCL_LIB_ADD) {
            LIBS += -l$${a}
        }
    } else {
        for(a, PCL_LIB_ADD) {
            CONFIG(debug, debug|release) {
                LIBS += -l$${a}_debug
            } else {
                LIBS += -l$${a}_release
            }
        }
    }

    greaterThan(QT_MAJOR_VERSION, 4) {
        load(moc)
        QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE
    }

    message(You want to use PCL : ok)
} else { 
    message(PCL will not be used )
} 
