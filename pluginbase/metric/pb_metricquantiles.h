#ifndef PB_METRICQUANTILES_H
#define PB_METRICQUANTILES_H

#include "ct_metric/abstract/ct_abstractmetric_xyz.h"

class PB_MetricQuantiles : public CT_AbstractMetric_XYZ
{
    Q_OBJECT
public:

    PB_MetricQuantiles();
    PB_MetricQuantiles(const PB_MetricQuantiles* other);

    void createConfigurationDialog();
    void updateParamtersAfterConfiguration();

    void updateName();

    QString getShortDescription() const;
    QString getDetailledDescription() const;

    QString getParametersAsString() const;
    virtual bool setParametersFromString(QString parameters);

    void createAttributes();
    void computeMetric();


    CT_AbstractConfigurableElement* copy() const;

private:
    QString _typeAsString;

};

#endif // PB_METRICQUANTILES_H
