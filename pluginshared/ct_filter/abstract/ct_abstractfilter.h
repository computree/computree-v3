#ifndef CT_ABSTRACTFILTER_H
#define CT_ABSTRACTFILTER_H

#include "pluginShared_global.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include <QObject>
#include <QStringList>

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFilter : public QObject
{
    Q_OBJECT
public:
    explicit CT_AbstractFilter();
    ~CT_AbstractFilter();

    inline QString getFilterName() {return _filterName;}

    bool configFilter();
    CT_StepConfigurableDialog* addConfigurationDialog();

    virtual QString getParametersAsString() const = 0;
    virtual QString getShortDescription() const = 0;
    virtual QString getDetailledDescription() const = 0;
    virtual CT_AbstractFilter* copy() const = 0;

    virtual void createConfigurationDialog() {}
    virtual void updateParamtersAfterConfiguration() {}

protected:
    QString     _filterName;
    CT_StepConfigurableDialog*  _configDialog;
};

#endif // CT_ABSTRACTFILTER_H
