#ifndef PARAMETERFILECHOICE_H
#define PARAMETERFILECHOICE_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetfilechoice.h"

class ParameterFileChoice : public AbstractParameter
{
public:
    ParameterFileChoice();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetFileChoice* widget() {return (WidgetFileChoice*) _widget;}

};

#endif // PARAMETERFILECHOICE_H
