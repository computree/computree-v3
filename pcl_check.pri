!exists($${PCL_INC_PATH}) {
    USE_PCL_ERROR_MSG += "PCL INCLUDE directory not found ! => $${PCL_INC_PATH}"
}

!exists($${PCL_LIBS_PATH}) {
    USE_PCL_ERROR_MSG += "PCL LIBS directory not found ! => $${PCL_LIBS_PATH}"
}

!exists($${EIGEN_INC_PATH}) {
    USE_PCL_ERROR_MSG += "EIGEN INCLUDE directory not found ! => $${EIGEN_INC_PATH}"
}

!exists($${QHULL_INC_PATH}) {
    USE_PCL_ERROR_MSG += "QHULL INCLUDE directory not found ! => $${QHULL_INC_PATH}"
}

!exists($${FLANN_INC_PATH}) {
    USE_PCL_ERROR_MSG += "FLANN INCLUDE directory not found ! => $${FLANN_INC_PATH}"
}

!exists($${BOOST_INC_PATH}) {
    USE_PCL_ERROR_MSG += "BOOST INCLUDE directory not found ! => $${BOOST_INC_PATH}"
}

!exists($${BOOST_LIBS_PATH}) {
    USE_PCL_ERROR_MSG += "BOOST LIBS directory not found ! => $${BOOST_LIBS_PATH}"
}
