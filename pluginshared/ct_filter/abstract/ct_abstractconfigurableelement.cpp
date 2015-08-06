#include "ct_abstractconfigurableelement.h"

CT_AbstractConfigurableElement::CT_AbstractConfigurableElement()
{
    _configDialog = NULL;
}

CT_AbstractConfigurableElement::~CT_AbstractConfigurableElement()
{
}

bool CT_AbstractConfigurableElement::configure()
{
    if (_configDialog == NULL)
    {
        createConfigurationDialog();
    }

    if (_configDialog != NULL)
    {
        _configDialog->exec();
        updateParamtersAfterConfiguration();
    }
    return true;
}

CT_StepConfigurableDialog *CT_AbstractConfigurableElement::addConfigurationDialog()
{
    _configDialog = new CT_StepConfigurableDialog();
    return _configDialog;
}
