#ifndef CT_STANDARDMETRICSEPARATOR_H
#define CT_STANDARDMETRICSEPARATOR_H

#include "ct_metric/abstract/ct_abstractmetric.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardMetricSeparator
{
public:
    CT_StandardMetricSeparator(QString title = "");
    virtual ~CT_StandardMetricSeparator();

    bool addMetric(CT_AbstractMetric *metric);

    QString getTitle() const;
    QList<CT_AbstractMetric*> metrics() const;

private:

    QList<CT_AbstractMetric*>   m_metrics;
    QString                     m_title;

};

#endif // CT_STANDARDMETRICSEPARATOR_H
