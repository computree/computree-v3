#ifndef PB_FILTERREMOVEUPPEROUTLIERS_H
#define PB_FILTERREMOVEUPPEROUTLIERS_H


#include "ct_filter/abstract/ct_abstractfilter_xyz.h"
#include "ct_iterator/ct_pointiterator.h"


class PB_FilterRemoveUpperOutliers : public CT_AbstractFilter_XYZ
{
    Q_OBJECT
public:

    PB_FilterRemoveUpperOutliers();
    PB_FilterRemoveUpperOutliers(const PB_FilterRemoveUpperOutliers* other);

    QString getName();
    QString getCompleteName();

    void createConfigurationDialog();
    void updateParamtersAfterConfiguration();

    QString getShortDescription() const;
    QString getDetailledDescription() const;

    QString getParametersAsString() const;
    virtual bool setParametersFromString(QString parameters);

    virtual CT_PointCloudIndexVector *filterPointCloudIndex() const;
    virtual void validatePoint(CT_PointIterator& pointIt) const {Q_UNUSED(pointIt);}

    CT_AbstractConfigurableElement* copy() const;

private:
    double  _resolution;
    int     _threshold;
    int     _dc;

    double  mutable _minx;
    double  mutable _miny;
    double  mutable _minz;
    size_t  mutable _dimx;
    size_t  mutable _dimy;
    size_t  mutable _dimz;


    size_t gridIndex(const double &x, const double &y, const double &z) const;
    size_t gridIndex(const size_t &colx, const size_t &liny, const size_t &levz) const;
};


#endif // PB_FILTERREMOVEUPPEROUTLIERS_H
