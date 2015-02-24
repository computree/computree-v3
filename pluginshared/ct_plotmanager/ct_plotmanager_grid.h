#ifndef CT_PLOTMANAGER_GRID_H
#define CT_PLOTMANAGER_GRID_H

#include "ct_plotmanager/abstract/ct_abstractplotmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_PlotManager_grid : public CT_AbstractPlotManager
{
    Q_OBJECT
public:
    enum Type {
        T_Circle,
        T_Square
    };

    CT_PlotManager_grid(double xRef, double yRef, double res, double size, CT_PlotManager_grid::Type type);
    ~CT_PlotManager_grid();

    inline virtual QString getPlotManagerType() const {return "Grid";}

    virtual QMap<QString, CT_AreaShape2DData*> getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max) const;
private:
    double _xRef;
    double _yRef;
    double _res;
    double _size;
    CT_PlotManager_grid::Type _type;
};

#endif // CT_PLOTMANAGER_GRID_H
