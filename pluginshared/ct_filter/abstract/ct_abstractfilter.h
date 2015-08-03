#ifndef CT_ABSTRACTFILTER_H
#define CT_ABSTRACTFILTER_H

#include "pluginShared_global.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_filter/abstract/ct_abstractconfigurableelement.h"

#include <QObject>
#include <QStringList>

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFilter : public CT_AbstractConfigurableElement
{
    Q_OBJECT
public:
    explicit CT_AbstractFilter();
    ~CT_AbstractFilter();

    virtual QString getParametersAsString() const = 0;

};

#endif // CT_ABSTRACTFILTER_H
