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

    class Box
    {
    public:
        Eigen::Vector2d _min;
        Eigen::Vector2d _max;

        Box(const Eigen::Vector2d &min, const Eigen::Vector2d &max)
        {
            _min(0) = min(0);
            _min(1) = min(1);
            _max(0) = max(0);
            _max(1) = max(1);
        }

        bool contains(const double &x, const double &y) const
        {
            if (x < _min(0)) {return false;}
            if (y < _min(1)) {return false;}
            if (x > _max(0)) {return false;}
            if (y > _max(1)) {return false;}
            return true;
        }
    };

    CT_PlotManager_grid(double xRef, double yRef, double res, double plotSize, CT_PlotManager_grid::Type type);
    ~CT_PlotManager_grid();

    inline virtual QString getPlotManagerType() const {return "Grid";}
    inline virtual size_t getPlotNumber () const {return _ncol*_nrow;}

    virtual QMap<QString, CT_AreaShape2DData*> getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max);

    void setLimits(const Eigen::Vector3d &min, const Eigen::Vector3d &max);

private:
    double _xRef;
    double _yRef;
    double _res;
    double _plotSize;
    CT_PlotManager_grid::Type _type;

    Eigen::Vector2d _min;
    Eigen::Vector2d _max;
    size_t          _ncol;
    size_t          _nrow;

    QList<CT_PlotManager_grid::Box> _alreadyReturnedAreas;


    inline static double roundUpValue(double val)
    {
        if (val >= 0) {return ceil(val);}
        return -ceil(-val);
    }
};

#endif // CT_PLOTMANAGER_GRID_H
