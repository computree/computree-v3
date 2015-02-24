#ifndef CT_PLOTMANAGER_PLOTSLIST_H
#define CT_PLOTMANAGER_PLOTSLIST_H

#include "ct_plotmanager/abstract/ct_abstractplotmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_PlotManager_plotsList : public CT_AbstractPlotManager
{
    Q_OBJECT
public:
    CT_PlotManager_plotsList();
    ~CT_PlotManager_plotsList();

    inline virtual QString getPlotManagerType() const {return "Plot List";}
    virtual QMap<QString, CT_AreaShape2DData *> getPlotsInBoundingBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max) const;

    void addPlot(const QString &idVal, CT_AreaShape2DData* shape);

private:
    QMap<QString, CT_AreaShape2DData*>  _plotList;
    mutable QList<QString>              _alreadyReturned;
};

#endif // CT_PLOTMANAGER_PLOTSLIST_H
