#ifndef PARAMETERTEXTLINE_H
#define PARAMETERTEXTLINE_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgettextline.h"

class ParameterTextLine : public AbstractParameter
{
public:
    ParameterTextLine();

    virtual bool isValid();
    virtual bool isDataParameter();
    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetTextLine* widget() {return (WidgetTextLine*) _widget;}

};

#endif // PARAMETERTEXTLINE_H
