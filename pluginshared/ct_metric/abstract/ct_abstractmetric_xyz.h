#ifndef CT_ABSTRACTMETRIC_XYZ_H
#define CT_ABSTRACTMETRIC_XYZ_H

#include "ct_metric/abstract/ct_abstractmetric.h"
#include  "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_shape2ddata/ct_areashape2ddata.h"


class PLUGINSHAREDSHARED_EXPORT CT_AbstractMetric_XYZ : public CT_AbstractMetric
{
    Q_OBJECT
public:

    CT_AbstractMetric_XYZ();
    CT_AbstractMetric_XYZ(const CT_AbstractMetric_XYZ *other);

    ~CT_AbstractMetric_XYZ();

    bool initResultAndData(CT_ResultGroup* result, const CT_AbstractPointCloudIndex* inCloud, const CT_AreaShape2DData& plotArea);

protected:
    const CT_AbstractPointCloudIndex* _inCloud;
    const CT_AreaShape2DData*         _plotArea;
};

#endif // CT_ABSTRACTMETRIC_XYZ_H
