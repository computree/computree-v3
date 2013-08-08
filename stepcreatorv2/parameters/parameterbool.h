#ifndef PARAMETERBOOL_H
#define PARAMETERBOOL_H

#include "parameters/abstractparameter.h"

class ParameterBool : public AbstractParameter
{
public:
    ParameterBool();

    virtual QString getName();

    static int val;

};

#endif // PARAMETERBOOL_H
