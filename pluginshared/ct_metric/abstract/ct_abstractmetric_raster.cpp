#include "ct_abstractmetric_raster.h"

CT_AbstractMetric_Raster::CT_AbstractMetric_Raster() : CT_AbstractMetric()
{
    _inRaster = NULL;
    _plotArea = NULL;
}

CT_AbstractMetric_Raster::CT_AbstractMetric_Raster(const CT_AbstractMetric_Raster *other) : CT_AbstractMetric(other)
{
    _inRaster = other->_inRaster;
    _plotArea = other->_plotArea;
}

CT_AbstractMetric_Raster::~CT_AbstractMetric_Raster()
{
}

bool CT_AbstractMetric_Raster::initResultAndData(CT_ResultGroup *result, const CT_AbstractImage2D *inRaster, const CT_AreaShape2DData &plotArea)
{
    _result = NULL;
    _inRaster = NULL;
    _plotArea = NULL;

    if (result == NULL) {return false;}
    if (_inRaster == NULL) {return false;}

    _result = result;
    _inRaster = inRaster;
    _plotArea = &plotArea;

    return true;
}
