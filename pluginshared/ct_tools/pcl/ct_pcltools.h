#ifndef CT_PCLTOOLS_H
#define CT_PCLTOOLS_H

#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_point.h"
#include "ct_itemdrawable/accessibility/ct_iaccesspointcloud.h"

class PLUGINSHAREDSHARED_EXPORT CT_PCLTools
{
public:
    CT_PCLTools();

#ifdef USE_PCL
    /**
     * @brief Convert a cloud index to a pcl point cloud
     */
    static boost::shared_ptr< CT_PCLCloud > staticConvertToPCLCloud(CT_PCIR pcir);
    static boost::shared_ptr< CT_PCLCloud > staticConvertToPCLCloud(const CT_IAccessPointCloud *item);
    static boost::shared_ptr< CT_PCLCloud > staticConvertToPCLCloud(const CT_AbstractPointCloudIndex *pci);
    static boost::shared_ptr< CT_PCLCloud > staticConvertToPCLCloud(const CT_AbstractCloudT<CT_PointData> *ipc);

    /**
     * @brief Convert a index from a pcl cloud to a global index for Computree
     * @param pcir / item / pci : the cloud index (or item) that you have passed to staticConvertToPCLCloud(...) OR the item to which you have called the method getPCLCloud()
     * @warning !!!! the index you want to convert must be issued from the original pcl cloud returned by the method staticConvertToPCLCloud(...). If the pcl cloud was reduced or modified
     * this method will returns a bad global index !!!
     */
    static ct_index_type staticPCLIndexToGlobalIndex(CT_PCIR pcir, const ct_index_type &index);
    static ct_index_type staticPCLIndexToGlobalIndex(const CT_IAccessPointCloud *item, const ct_index_type &index);
    static ct_index_type staticPCLIndexToGlobalIndex(const CT_AbstractPointCloudIndex *pci, const ct_index_type &index);
#endif
};

#endif // CT_PCLTOOLS_H
