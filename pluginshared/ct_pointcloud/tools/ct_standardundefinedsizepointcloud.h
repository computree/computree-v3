#ifndef CT_STANDARDUNDEFINEDSIZEPOINTCLOUD_H
#define CT_STANDARDUNDEFINEDSIZEPOINTCLOUD_H

#include "ct_pointcloud/tools/abstract/ct_abstractundefinedsizepointcloud.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 * A cloud of points used by the GlobalPointCloudManager when you want to create a cloud of points and you
 * don't know it's size by advance.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardUndefinedSizePointCloud : public CT_AbstractUndefinedSizePointCloud
{
public:

    size_t beginIndex() const;
    size_t size() const;
    void addPoint(const CT_Point &point);
    CT_Point& addPoint();
    CT_Point& operator[](const size_t &i);
    CT_Point& pointAt(const size_t &i);
    CT_Point& operator[](const size_t &i) const;
    const CT_Point& constPointAt(const size_t &i) const;

private:

    size_t                                  m_bIndex;
    CT_StandardCloudStdVectorT<CT_Point>    *m_pc;

    friend class CT_GlobalPointCloudManager;

    CT_StandardUndefinedSizePointCloud(const size_t &beginIndex, const CT_StandardCloudStdVectorT<CT_Point> *pc);
};

#endif // CT_STANDARDUNDEFINEDSIZEPOINTCLOUD_H
