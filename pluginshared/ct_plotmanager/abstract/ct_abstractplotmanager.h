#ifndef CT_ABSTRACTPLOTMANAGER_H
#define CT_ABSTRACTPLOTMANAGER_H

#include "pluginShared_global.h"
#include "ct_shape2ddata/ct_areashape2ddata.h"

#include "eigen/Eigen/Core"

#include <QObject>
#include <QMap>
#include <QString>

class PLUGINSHAREDSHARED_EXPORT CT_AbstractPlotManager : public QObject
{
    Q_OBJECT
public:
    explicit CT_AbstractPlotManager();
    ~CT_AbstractPlotManager();

    virtual QString getPlotManagerType() const = 0;
    virtual size_t getPlotNumber () const = 0;

    virtual QMap<QString, CT_AreaShape2DData*> getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max) = 0;

};

#endif // CT_ABSTRACTPLOTMANAGER_H
