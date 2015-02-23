#include "ct_standardfilterseparator.h"

CT_StandardFilterSeparator::CT_StandardFilterSeparator(QString title)
{
    m_title = title;
}

CT_StandardFilterSeparator::~CT_StandardFilterSeparator()
{
    qDeleteAll(m_filters.begin(), m_filters.end());
}

bool CT_StandardFilterSeparator::addFilter(CT_AbstractFilter *filter)
{
    m_filters.append(filter);
    return true;
}

QString CT_StandardFilterSeparator::getTitle() const
{
    return m_title;
}

QList<CT_AbstractFilter *> CT_StandardFilterSeparator::filters() const
{
    return m_filters;
}
