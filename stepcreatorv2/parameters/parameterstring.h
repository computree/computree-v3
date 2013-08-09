#ifndef PARAMETERSTRING_H
#define PARAMETERSTRING_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetstring.h"

class ParameterString : public AbstractParameter
{
public:
    ParameterString();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetString* widget() {return (WidgetString*) _widget;}

};

#endif // PARAMETERSTRING_H
