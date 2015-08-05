#include "ct_abstractmetric_xyz.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_attributes/ct_stditemattributet.h"

CT_AbstractMetric_XYZ::CT_AbstractMetric_XYZ() : CT_AbstractMetric()
{
    _inCloud = NULL;
    _plotArea = NULL;
}

CT_AbstractMetric_XYZ::CT_AbstractMetric_XYZ(const CT_AbstractMetric_XYZ *other) : CT_AbstractMetric(other)
{
    _inCloud = other->_inCloud;
    _plotArea = other->_plotArea;
}

CT_AbstractMetric_XYZ::~CT_AbstractMetric_XYZ()
{
}

bool CT_AbstractMetric_XYZ::initResultAndData(CT_ResultGroup *result, const CT_AbstractPointCloudIndex *inCloud, const CT_AreaShape2DData &plotArea)
{
    _result = NULL;
    _inCloud = NULL;
    _plotArea = NULL;

    if (result == NULL) {return false;}
    if (inCloud == NULL) {return false;}

    _result = result;
    _inCloud = inCloud;
    _plotArea = &plotArea;

    return true;
}
