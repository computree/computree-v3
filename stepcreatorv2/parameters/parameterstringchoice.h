#ifndef PARAMETERSTRINGCHOICE_H
#define PARAMETERSTRINGCHOICE_H

#include "parameters/abstractparameter.h"
#include "parameters/widgets/widgetstringchoice.h"

class ParameterStringChoice : public AbstractParameter
{
public:
    ParameterStringChoice();

    virtual QString getParameterDeclaration();
    virtual QString getParameterInitialization();
    virtual QString getParameterDialogCommands();
    virtual QString getParamaterDoc();

    void virtual onAliasChange();

private:
    inline WidgetStringChoice* widget() {return (WidgetStringChoice*) _widget;}

};

#endif // PARAMETERSTRINGCHOICE_H
