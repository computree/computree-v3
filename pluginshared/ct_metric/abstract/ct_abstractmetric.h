#ifndef CT_ABSTRACTMETRIC_H
#define CT_ABSTRACTMETRIC_H

#include "pluginShared_global.h"
#include <QObject>

class PLUGINSHAREDSHARED_EXPORT CT_AbstractMetric : public QObject
{
    Q_OBJECT
public:
    explicit CT_AbstractMetric();
    ~CT_AbstractMetric();

    virtual QString getBaseName() const = 0;
    virtual QString getParametersAsString() const = 0;
    virtual QString getShortDescription() const = 0;
    virtual QString getDetailledDescription() const = 0;
    virtual CT_AbstractMetric* copy() const  = 0;

};

#endif // CT_ABSTRACTMETRIC_H
