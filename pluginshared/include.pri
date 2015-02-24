# To use PCL =>
# Do the declaration in file use_pcl.ini in this order with quotes for all lines :
# example for ubuntu 13.10 (remove # symbols) :
#"1.7"
#"/usr/lib"
#"/usr/include/pcl-1.7"
#"/usr/include/eigen3"
#"/usr/include/qhull"
#"/usr/include/flann/"
#"/usr/lib/x86_64-linux-gnu"
#"/usr/include"

# example for windows, with external sub-directory (remove # symbols) WARNING : slash must be like this / :
#"1.7"
#"../../external/pcl/lib"
#"../../external/pcl/include"
#"../../external/eigen/include"
#"../../external/qhull/include"
#"../../external/flann/include"
#"../../external/boost/lib"
#"../../external/boost/include"

include(../include_gdal.pri)

contains(QMAKE_TARGET.arch, x86_64) {
    DEFINES += ENVIRONMENT64
} else {
    DEFINES += ENVIRONMENT32
}

PCL_LIB_VERSION =
PCL_LIB_DIR_PATH =
PCL_INCLUDE_DIR_PATH =
EIGEN_INCLUDE_DIR_PATH =
QHULL_INCLUDE_DIR_PATH =
FLANN_INCLUDE_DIR_PATH =
BOOST_LIB_DIR_PATH =
BOOST_INCLUDE_DIR_PATH =
#end of use_pcl.ini file

PCL_LIB_PATH =
BOOS_LIB_SERIALIZATION_PATH =
BOOS_LIB_FILESYSTEM_PATH =

# Ajout AP : évite de modifier le include.pri chacun de son coté
# A la place créer un use_pcl.ini dans le répertoire racine de computree si on veut utiliser PCL
exists(../../use_pcl.ini) {
    DEFINES += USE_PCL
    message( ------- use_pcl.ini detected ------ )

    # Modif MK : permet de mettre les chemins dans le fichier use_pcl.ini
    TMP_CAT =

    unix | mac {
        TMP_CAT = $$system( cat ../../use_pcl.ini )
    }

    win32 {
        TMP_CAT = $$system( type ..\..\use_pcl.ini )
    }

    #pcl path
    TMP_MEMBER = $$member(TMP_CAT, 0)
    PCL_LIB_VERSION = $$replace(TMP_MEMBER, \", )
    TMP_MEMBER = $$member(TMP_CAT, 1)
    PCL_LIB_DIR_PATH = $$replace(TMP_MEMBER, \", )
    TMP_MEMBER = $$member(TMP_CAT, 2)
    PCL_INCLUDE_DIR_PATH = $$replace(TMP_MEMBER, \", )

    #eigen path
    TMP_MEMBER = $$member(TMP_CAT, 3)
    EIGEN_INCLUDE_DIR_PATH = $$replace(TMP_MEMBER, \", )

    #qhull path
    TMP_MEMBER = $$member(TMP_CAT, 4)
    QHULL_INCLUDE_DIR_PATH = $$replace(TMP_MEMBER, \", )

    #flann path
    TMP_MEMBER = $$member(TMP_CAT, 5)
    FLANN_INCLUDE_DIR_PATH = $$replace(TMP_MEMBER, \", )

    #boost path
    TMP_MEMBER = $$member(TMP_CAT, 6)
    BOOST_LIB_DIR_PATH = $$replace(TMP_MEMBER, \", )
    TMP_MEMBER = $$member(TMP_CAT, 7)
    BOOST_INCLUDE_DIR_PATH = $$replace(TMP_MEMBER, \", )

    #unix
    unix {
        isEmpty($${PCL_LIB_VERSION}) {
            PCL_LIB_PATH = $${PCL_LIB_DIR_PATH}/libpcl_*.so
        } else {
            PCL_LIB_PATH = $${PCL_LIB_DIR_PATH}/libpcl_*.so.$${PCL_LIB_VERSION}
        }

        BOOST_LIB_TO_LINK = $${BOOST_LIB_DIR_PATH}/lib*-gd-*.so*
    }

    # windows
    win32 {
        msvc {
            PCL_LIB_PATH = $${PCL_LIB_DIR_PATH}/pcl_*.lib
            BOOST_LIB_TO_LINK = $${BOOST_LIB_DIR_PATH}/lib*-gd-*.lib
        }

        mingw {
            PCL_LIB_PATH = $${PCL_LIB_DIR_PATH}/libpcl_*.dll.a
            BOOST_LIB_TO_LINK = $${BOOST_LIB_DIR_PATH}/lib*-gd-*.dll.a
        }
    }

    # mac
    mac {
        PCL_LIB_PATH = $${PCL_LIB_DIR_PATH}/libpcl_*.dylib
        BOOST_LIB_TO_LINK = $${BOOST_LIB_DIR_PATH}/lib*-gd-*.dylib
    }

    message(PCL LIB VERSION : $$PCL_LIB_VERSION)
    message(PCL LIB DIRECTORY : $$PCL_LIB_DIR_PATH)
    message(PCL INCLUDE DIRECTORY : $$PCL_INCLUDE_DIR_PATH)
    message(PCL LIB TO LINK : $$PCL_LIB_PATH)
    message(EIGEN INCLUDE DIRECTORY : $$EIGEN_INCLUDE_DIR_PATH)
    message(QHULL INCLUDE DIRECTORY : $$QHULL_INCLUDE_DIR_PATH)
    message(FLANN INCLUDE DIRECTORY : $$FLANN_INCLUDE_DIR_PATH)
    message(BOOST LIB DIRECTORY : $$BOOST_LIB_DIR_PATH)
    message(BOOST INCLUDE DIRECTORY : $$BOOST_INCLUDE_DIR_PATH)
    message(BOOST LIB TO LINK : $$BOOST_LIB_TO_LINK)

    #DEFINES += USE_BOOST
    #DEFINES += USE_BOOST_BINARY
} else {
    message( ------- use_pcl.ini NOT detected ------ )
}

message( ------- include.pri ------ )

# PCL and dependency
contains( DEFINES, USE_PCL ) {
    message( "you want to use pcl : " )

    # use boost when use pcl !
    DEFINES += USE_BOOST

    ###############################################

    !exists($${PCL_INCLUDE_DIR_PATH}) {
        error( "PCL INCLUDE directory not found ! => " $${PCL_INCLUDE_DIR_PATH})
    }

    !exists($${PCL_LIB_DIR_PATH}) {
        error( "PCL LIB directory not found ! => " $${PCL_LIB_DIR_PATH})
    }

    !exists($${PCL_LIB_PATH}) {
        error( "PCL LIBRARY not found ! =>" $${PCL_LIB_PATH} )
    }

    INCLUDEPATH += $${PCL_INCLUDE_DIR_PATH}
    LIBS += $${PCL_LIB_PATH}

    ###############################################

    # EIGEN DIRECTORY
    !exists($${EIGEN_INCLUDE_DIR_PATH}) {
        error( "EIGEN include directory not found ! =>" $${EIGEN_INCLUDE_DIR_PATH})
    }

    INCLUDEPATH += $${EIGEN_INCLUDE_DIR_PATH}

    ###############################################

    # QHULL DIRECTORY
    !exists($${QHULL_INCLUDE_DIR_PATH}) {
        error( "QHULL include directory not found ! =>" $${QHULL_INCLUDE_DIR_PATH})
    }

    INCLUDEPATH += $${QHULL_INCLUDE_DIR_PATH}

    ###############################################

    # FLANN DIRECTORY
    !exists($${FLANN_INCLUDE_DIR_PATH}) {
        error( "FLANN include directory not found ! =>" $${FLANN_INCLUDE_DIR_PATH})
    }

    INCLUDEPATH += $${FLANN_INCLUDE_DIR_PATH}

    message(OK)

} else {
    DEFINES += USE_EIGEN_POINT

    contains(DEFINES, USE_EIGEN_POINT) {

        isEmpty(PLUGIN_SHARED_DIR) {
            INCLUDEPATH += ./eigen
        } else {
            INCLUDEPATH += $${PLUGIN_SHARED_DIR}/eigen
        }
    }
}

# BOOST
contains(DEFINES, USE_BOOST) {

    message( "you want to use boost : " )

    !exists($${BOOST_INCLUDE_DIR_PATH}) {
        error( "BOOST INCLUDE directory not found ! =>" $${BOOST_INCLUDE_DIR_PATH})
    }

    !exists($${BOOST_LIB_DIR_PATH}) {
        error( "BOOST LIB directory not found ! =>" $${BOOST_LIB_DIR_PATH})
    }

    greaterThan(QT_MAJOR_VERSION, 4) {
        load(moc)
        QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE
    }

    message(OK)

    INCLUDEPATH += $${BOOST_INCLUDE_DIR_PATH}

    LIBS += $${BOOST_LIB_TO_LINK}
}
