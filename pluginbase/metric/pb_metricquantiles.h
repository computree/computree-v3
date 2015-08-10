#ifndef PB_METRICQUANTILES_H
#define PB_METRICQUANTILES_H

#include "ct_metric/abstract/ct_abstractmetric_xyz.h"

class PB_MetricQuantiles : public CT_AbstractMetric_XYZ
{
    Q_OBJECT
public:

    PB_MetricQuantiles();
    PB_MetricQuantiles(const PB_MetricQuantiles* other);

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



    static double computeQuantile(const QList<double> &values, double quantile);
private:
    double _quantMin;
    double _quantMax;
    double _quantStep;
    QString _prefix;
    bool _hmin;
    bool _hmed;
    bool _h99;
    bool _hmax;


    QString getQuantileString(double quantile);
};

#endif // PB_METRICQUANTILES_H
