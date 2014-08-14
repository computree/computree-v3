#include "ct_abstractoutmodelcopyaction.h"

CT_AbstractOutModelCopyAction::CT_AbstractOutModelCopyAction()
{
    _success = true;
}

CT_AbstractOutModelCopyAction::~CT_AbstractOutModelCopyAction()
{
}

bool CT_AbstractOutModelCopyAction::isSucceeded() const
{
    return _success;
}

QString CT_AbstractOutModelCopyAction::errorMessage() const
{
    return _err;
}

void CT_AbstractOutModelCopyAction::setSucceeded(bool success)
{
    _success = success;
}

void CT_AbstractOutModelCopyAction::setErrorMessage(const QString &err)
{
    _err = err;
}
