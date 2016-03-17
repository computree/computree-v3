#include "ct_abstractmetric_raster.h"

CT_AbstractMetric_Raster::CT_AbstractMetric_Raster() : CT_AbstractMetricGeneric()
{
    _inRaster = NULL;
    _plotArea = NULL;
}

CT_AbstractMetric_Raster::CT_AbstractMetric_Raster(const CT_AbstractMetric_Raster &other) : CT_AbstractMetricGeneric(other)
{
    _inRaster = other._inRaster;
    _plotArea = other._plotArea;
}

CT_AbstractMetric_Raster::~CT_AbstractMetric_Raster()
{
}

bool CT_AbstractMetric_Raster::initDatas(const CT_AbstractImage2D *inRaster, const CT_AreaShape2DData &plotArea)
{
    _inRaster = inRaster;
    _plotArea = &plotArea;

    return (_inRaster != NULL);
}
