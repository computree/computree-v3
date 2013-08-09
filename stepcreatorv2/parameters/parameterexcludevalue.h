#ifndef PARAMETEREXCLUDEVALUE_H
#define PARAMETEREXCLUDEVALUE_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetexcludevalue.h"

class ParameterExcludeValue : public AbstractParameter
{
public:
    ParameterExcludeValue();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetExcludeValue* widget() {return (WidgetExcludeValue*) _widget;}

};

#endif // PARAMETEREXCLUDEVALUE_H
