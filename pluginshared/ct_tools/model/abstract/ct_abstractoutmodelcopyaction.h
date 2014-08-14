#ifndef CT_ABSTRACTOUTMODELCOPYACTION_H
#define CT_ABSTRACTOUTMODELCOPYACTION_H

#include "pluginShared_global.h"

#include <QString>

class CT_VirtualAbstractStep;
class CT_OutResultModelGroupToCopyPossibility;

/** \class  CT_AbstractOutModelCopyAction
    \brief  Abstract class of an action that can be used to execute some actions on a copy of a "CT_OutResultModelGroup".
*/
class PLUGINSHAREDSHARED_EXPORT CT_AbstractOutModelCopyAction
{
public:
    CT_AbstractOutModelCopyAction();
    virtual ~CT_AbstractOutModelCopyAction();

    virtual bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy) = 0;
    bool isSucceeded() const;
    QString errorMessage() const;

protected:

    void setSucceeded(bool success);
    void setErrorMessage(const QString &err);

private:

    bool    _success;
    QString _err;
};

#endif // CT_ABSTRACTOUTMODELCOPYACTION_H
