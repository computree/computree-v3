#ifndef PARAMETERDOUBLE_H
#define PARAMETERDOUBLE_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetdouble.h"

class ParameterDouble : public AbstractParameter
{
public:
    ParameterDouble();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetDouble* widget() {return (WidgetDouble*) _widget;}

};

#endif // PARAMETERDOUBLE_H
