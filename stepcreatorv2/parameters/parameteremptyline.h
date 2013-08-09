#ifndef PARAMETEREMPTYLINE_H
#define PARAMETEREMPTYLINE_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetemptyline.h"

class ParameterEmptyLine : public AbstractParameter
{
public:
    ParameterEmptyLine();

    virtual bool isValid();
    virtual bool isDataParameter();
    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetEmptyLine* widget() {return (WidgetEmptyLine*) _widget;}

};

#endif // PARAMETEREMPTYLINE_H
