#ifndef PB_METRICCOMPUTESTATS_H
#define PB_METRICCOMPUTESTATS_H


#include "ct_metric/abstract/ct_abstractmetric_xyz.h"

class PB_MetricComputeStats : public CT_AbstractMetric_XYZ
{
    Q_OBJECT
public:

    PB_MetricComputeStats();
    PB_MetricComputeStats(const PB_MetricComputeStats* other);

    QString getName();

    void createConfigurationDialog();
    void updateParamtersAfterConfiguration();

    QString getShortDescription() const;
    QString getDetailledDescription() const;

    QString getParametersAsString() const;
    virtual bool setParametersFromString(QString parameters);

    void createAttributes();
    void computeMetric();


    CT_AbstractConfigurableElement* copy() const;

private:
    bool _computeHmean;
    bool _computeHsd;
    bool _computeHskew;
    bool _computeHkurt;
    bool _computeHcv;
};


#endif // PB_METRICCOMPUTESTATS_H
