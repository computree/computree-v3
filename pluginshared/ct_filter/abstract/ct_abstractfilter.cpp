#include "ct_abstractfilter.h"

CT_AbstractFilter::CT_AbstractFilter() : QObject()
{
    _configDialog = NULL;
    _filterName = "";
}

CT_AbstractFilter::~CT_AbstractFilter()
{
    _configDialog = NULL;
}

bool CT_AbstractFilter::configFilter()
{
    if (_configDialog != NULL)
    {
        if (_configDialog->exec() == 1)
        {
            updateParamtersAfterConfiguration();
            return true;
        } else {return false;}
    }
    return true;
}

CT_StepConfigurableDialog *CT_AbstractFilter::addConfigurationDialog()
{
    _configDialog = new CT_StepConfigurableDialog();
    return _configDialog;
}

