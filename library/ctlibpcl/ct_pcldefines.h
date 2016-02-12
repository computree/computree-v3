#ifndef CT_PCLDEFINES_H
#define CT_PCLDEFINES_H

#ifndef Q_MOC_RUN
    #define PCL_NO_PRECOMPILE
    #include <pcl/point_types.h>
    #include <pcl/point_cloud.h>
    #include <pcl/octree/octree_search.h>
    #include <pcl/octree/impl/octree_search.hpp>
    #include <pcl/filters/voxel_grid.h>
    #include <pcl/filters/impl/voxel_grid.hpp>
    #include <pcl/filters/statistical_outlier_removal.h>
    #include <pcl/filters/impl/statistical_outlier_removal.hpp>
#endif

typedef pcl::PointXYZ                                                               CT_PCLPoint;
typedef pcl::PointCloud<CT_PCLPoint>                                                CT_PCLCloud;
typedef std::vector<int>                                                            CT_PCLCloudIndex;

#endif // CT_PCLDEFINES_H
