#ifndef CT_STANDARDFILTERSEPARATOR_H
#define CT_STANDARDFILTERSEPARATOR_H

#include "ct_filter/abstract/ct_abstractfilter.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardFilterSeparator
{
public:
    CT_StandardFilterSeparator(QString title = "");
    virtual ~CT_StandardFilterSeparator();

    bool addFilter(CT_AbstractFilter *filter);

    QString getTitle() const;
    QList<CT_AbstractFilter*> filters() const;

private:

    QList<CT_AbstractFilter*>   m_filters;
    QString                     m_title;

};

#endif // CT_STANDARDFILTERSEPARATOR_H
