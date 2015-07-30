#ifndef CT_INTERNALPOINTCLOUD_H
#define CT_INTERNALPOINTCLOUD_H

#include "ct_cloud/ct_standardcloudosgt.h"
#include "ct_point.h"

#include "ct_tools/pcl/ct_pcltools.h"

class PLUGINSHAREDSHARED_EXPORT CT_InternalPointCloud : public CT_StandardCloudOsgT<CT_PointData, osg::Array::Vec3ArrayType, 3, GL_FLOAT>
{
public:
    CT_InternalPointCloud(const size_t &initialSize = 0);

#ifdef USE_PCL
    virtual boost::shared_ptr< CT_PCLCloud > getPCLCloud() const
    {
        return CT_PCLTools::staticConvertToPCLCloud(this);
    }
#endif

    virtual CT_AbstractCloud* copy() const;
};

#endif // CT_INTERNALPOINTCLOUD_H
