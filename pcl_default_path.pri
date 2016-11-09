unix {
    PCL_BASE_PATH = "/usr/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.7"
    EIGEN_INC_PATH = "$${PCL_BASE_PATH}include/eigen3"
    QHULL_INC_PATH = "$${PCL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    FLANN_INC_PATH = "$${PCL_BASE_PATH}include"
    BOOST_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    BOOST_INC_PATH = "$${PCL_BASE_PATH}include"
}

windows {
    PCL_BASE_PATH = "C:/Program Files/PCL 1.7.2/"
    PCL_3RDPARTY_PATH = "$${PCL_BASE_PATH}3rdParty/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.7"
    EIGEN_INC_PATH = "$${PCL_3RDPARTY_PATH}Eigen/eigen3"
    QHULL_INC_PATH = "$${PCL_3RDPARTY_PATH}Qhull/include"
    QHULL_LIBS_PATH = "$${PCL_3RDPARTY_PATH}Qhull/lib"
    FLANN_INC_PATH = "$${PCL_3RDPARTY_PATH}FLANN/include"
    BOOST_LIBS_PATH = "$${PCL_3RDPARTY_PATH}Boost/lib"
    BOOST_INC_PATH = "$${PCL_3RDPARTY_PATH}Boost/include/boost-1_57"
}
