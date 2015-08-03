#ifndef CT_ABSTRACTCONFIGURABLEELEMENT_H
#define CT_ABSTRACTCONFIGURABLEELEMENT_H

#include "pluginShared_global.h"
#include "ct_view/ct_stepconfigurabledialog.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractConfigurableElement : public QObject
{
    Q_OBJECT
public:
    CT_AbstractConfigurableElement();
    ~CT_AbstractConfigurableElement();

    inline QString getName() {return _name;}
    virtual QString getShortDescription() const = 0;
    virtual QString getDetailledDescription() const = 0;
    virtual CT_AbstractConfigurableElement* copy() const = 0;

    bool configure();

protected:
    QString     _name;
    CT_StepConfigurableDialog*  _configDialog;

    CT_StepConfigurableDialog* addConfigurationDialog();

    virtual void updateName() {}
    virtual void createConfigurationDialog() {}
    virtual void updateParamtersAfterConfiguration() {}

};

#endif // CT_ABSTRACTCONFIGURABLEELEMENT_H
