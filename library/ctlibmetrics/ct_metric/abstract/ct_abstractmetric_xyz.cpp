#include "ct_abstractmetric_xyz.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_attributes/ct_stditemattributet.h"

CT_AbstractMetric_XYZ::CT_AbstractMetric_XYZ() : CT_AbstractMetricGeneric()
{
    _inCloud = NULL;
    _plotArea = NULL;
}

CT_AbstractMetric_XYZ::CT_AbstractMetric_XYZ(const CT_AbstractMetric_XYZ &other) : CT_AbstractMetricGeneric(other)
{
    _inCloud = other._inCloud;
    _plotArea = other._plotArea;
}

CT_AbstractMetric_XYZ::~CT_AbstractMetric_XYZ()
{
}

bool CT_AbstractMetric_XYZ::initDatas(const CT_AbstractPointCloudIndex* inCloud, const CT_AreaShape2DData* plotArea)
{
    _inCloud = inCloud;
    _plotArea = plotArea;

    return (_inCloud != NULL);
}

const CT_AreaShape2DData *CT_AbstractMetric_XYZ::plotArea() const
{
    return _plotArea;
}

const CT_AbstractPointCloudIndex *CT_AbstractMetric_XYZ::pointCloud() const
{
    return _inCloud;
}
