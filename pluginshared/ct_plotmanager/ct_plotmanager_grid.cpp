#include "ct_plotmanager_grid.h"

CT_PlotManager_grid::CT_PlotManager_grid(double xRef, double yRef, double res, double size, CT_PlotManager_grid::Type type) : CT_AbstractPlotManager()
{
    _xRef = xRef;
    _yRef = yRef;
    _res = res;
    _size = size;
    _type = type;
}

CT_PlotManager_grid::~CT_PlotManager_grid()
{

}

QMap<QString, CT_AreaShape2DData*> CT_PlotManager_grid::getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max) const
{
    QMap<QString, CT_AreaShape2DData*> map;

    return map;
}

