#ifndef CT_ABSTRACTFILTER_H
#define CT_ABSTRACTFILTER_H

#include "pluginShared_global.h"
#include <QObject>

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFilter : public QObject
{
    Q_OBJECT
public:
    explicit CT_AbstractFilter();
    ~CT_AbstractFilter();

    virtual QString getBaseName() const = 0;
    virtual QString getParametersAsString() const = 0;
    virtual QString getShortDescription() const = 0;
    virtual QString getDetailledDescription() const = 0;

};

#endif // CT_ABSTRACTFILTER_H
