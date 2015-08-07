#ifndef CT_ABSTRACTFILTER_XYZ_H
#define CT_ABSTRACTFILTER_XYZ_H

#include "ct_filter/abstract/ct_abstractfilter.h"
#include  "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include  "ct_pointcloudindex/ct_pointcloudindexvector.h"


class PLUGINSHAREDSHARED_EXPORT CT_AbstractFilter_XYZ : public CT_AbstractFilter
{
    Q_OBJECT
public:
    CT_AbstractFilter_XYZ();
    CT_AbstractFilter_XYZ(const CT_AbstractFilter_XYZ *other);
    ~CT_AbstractFilter_XYZ();

    bool setPointCloudIndex(const CT_AbstractItemDrawableWithPointCloud* inItem);

    virtual CT_PointCloudIndexVector *filterPointCloudIndex() const;
    virtual void validatePoint(CT_PointIterator& pointIt) const = 0;

protected:

    const CT_AbstractItemDrawableWithPointCloud*    _inItem;
    const CT_AbstractPointCloudIndex* _inCloud;
    CT_PointCloudIndexVector* _outCloud;

};

#endif // CT_ABSTRACTFILTER_XYZ_H
