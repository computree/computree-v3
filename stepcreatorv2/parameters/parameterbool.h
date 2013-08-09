#ifndef PARAMETERBOOL_H
#define PARAMETERBOOL_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetbool.h"

class ParameterBool : public AbstractParameter
{
public:
    ParameterBool();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetBool* widget() {return (WidgetBool*) _widget;}

};

#endif // PARAMETERBOOL_H
