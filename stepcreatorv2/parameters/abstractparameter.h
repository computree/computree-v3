#ifndef ABSTRACTPARAMETER_H
#define ABSTRACTPARAMETER_H

#include "qstandarditemmodel.h"
#include "parameters/widgets/abstractparameterwidget.h"

class AbstractParameter : public QStandardItem
{
public:

    AbstractParameter();
    ~AbstractParameter();

    virtual AbstractParameterWidget* getWidget();
    virtual bool isValid();
    virtual bool isDataParameter();
    virtual QString getName();
    virtual QString getParameterDeclaration() = 0;
    virtual QString getParameterInitialization() = 0;
    virtual QString getParameterDialogCommands() = 0;
    virtual QString getParamaterDoc() = 0;

    void virtual onAliasChange() = 0;

protected:
    AbstractParameterWidget*  _widget;

};
#endif // ABSTRACTPARAMETER_H
