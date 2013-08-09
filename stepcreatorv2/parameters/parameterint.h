#ifndef PARAMETERINT_H
#define PARAMETERINT_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetint.h"

class ParameterInt : public AbstractParameter
{
public:
    ParameterInt();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetInt* widget() {return (WidgetInt*) _widget;}

};

#endif // PARAMETERINT_H
