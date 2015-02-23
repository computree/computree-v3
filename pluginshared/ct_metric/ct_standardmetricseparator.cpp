#include "ct_standardmetricseparator.h"

CT_StandardMetricSeparator::CT_StandardMetricSeparator(QString title)
{
    m_title = title;
}

CT_StandardMetricSeparator::~CT_StandardMetricSeparator()
{
    qDeleteAll(m_metrics.begin(), m_metrics.end());
}

bool CT_StandardMetricSeparator::addMetric(CT_AbstractMetric *metric)
{
    m_metrics.append(metric);
    return true;
}

QString CT_StandardMetricSeparator::getTitle() const
{
    return m_title;
}

QList<CT_AbstractMetric*> CT_StandardMetricSeparator::metrics() const
{
    return m_metrics;
}
