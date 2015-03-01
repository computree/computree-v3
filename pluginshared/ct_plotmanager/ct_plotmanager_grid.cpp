#include "ct_plotmanager_grid.h"

#include "ct_shape2ddata/ct_box2ddata.h"
#include "ct_shape2ddata/ct_circle2ddata.h"

CT_PlotManager_grid::CT_PlotManager_grid(double xRef, double yRef, double res, double plotSize, CT_PlotManager_grid::Type type) : CT_AbstractPlotManager()
{
    _xRef = xRef;
    _yRef = yRef;
    _res = res;
    _plotSize = plotSize;
    _type = type;

    _min << 0, 0;
    _max << 0, 0;
}

CT_PlotManager_grid::~CT_PlotManager_grid()
{
}

QMap<QString, CT_AreaShape2DData*> CT_PlotManager_grid::getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max)
{
    QMap<QString, CT_AreaShape2DData*> map;

    Eigen::Vector2d begin, end;
    double plotSizex2 = _plotSize * 2.0;

    begin(0) = min(0) - _plotSize;
    begin(1) = min(1) - _plotSize;

    end(0) = max(0) + _plotSize;
    end(1) = max(1) + _plotSize;

    begin(0) = _xRef - _res * ( roundUpValue((_xRef - begin(0)) / _res) );
    begin(1) = _yRef - _res * ( roundUpValue((_yRef - begin(1)) / _res) );

    end(0) = _xRef + _res * ( roundUpValue((end(0) - _xRef) / _res) );
    end(1) = _yRef + _res * ( roundUpValue((end(1) - _yRef) / _res) );

    if (begin(0) < _min(0)) {begin(0) = _min(0);}
    if (begin(1) < _min(1)) {begin(1) = _min(1);}

    if (end(0) < _max(0)) {end(0) = _max(0);}
    if (end(1) < _max(1)) {end(1) = _max(1);}


    for (double y = begin(1) ; y <= end(1) ; y += _res)
    {
        for (double x = begin(0) ; x <= end(0) ; x += _res)
        {
            bool found = false;
            QListIterator<Box> it(_alreadyReturnedAreas);
            while (it.hasNext() && !found)
            {
                const Box &box = it.next();
                if (!box.contains(x, y))
                {
                    size_t id = ((x - _min(0)) / _res) + ((y - _min(1)) / _res)*_nrow;

                    CT_AreaShape2DData* shape = NULL;

                    if (_type == CT_PlotManager_grid::T_Circle)
                    {
                        shape = new CT_Circle2DData(Eigen::Vector2d(x, y), _plotSize);
                    } else {
                        shape = new CT_Box2DData(Eigen::Vector2d(x, y), plotSizex2, plotSizex2);
                    }
                    map.insert(QString("%1").arg(id), shape);
                 } else {
                    found = true;
                }
            }
        }
    }

    _alreadyReturnedAreas.append(Box(begin, end));

    return map;
}

void CT_PlotManager_grid::setLimits(const Eigen::Vector3d &min, const Eigen::Vector3d &max)
{
    _alreadyReturnedAreas.clear();

    _min(0) = _res/2.0 + _xRef - _res * ( roundUpValue((_xRef - min(0)) / _res) );
    _min(1) = _res/2.0 + _yRef - _res * ( roundUpValue((_yRef - min(1)) / _res) );

    _max(0) = _res/2.0 + _xRef + _res * ( roundUpValue((max(0) - _xRef) / _res) );
    _max(1) = _res/2.0 + _yRef + _res * ( roundUpValue((max(1) - _yRef) / _res) );

    _ncol = size_t ((_max(0) - _min(0)) / _res);
    _nrow = size_t ((_max(1) - _min(1)) / _res);

}


