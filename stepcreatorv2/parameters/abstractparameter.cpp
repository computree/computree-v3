#include "abstractparameter.h"

AbstractParameter::AbstractParameter()
{
}

 AbstractParameter::~ AbstractParameter()
{
    delete _widget;
}
AbstractParameterWidget*  AbstractParameter::getWidget()
{
    return _widget;
}

bool  AbstractParameter::isValid()
{
    return _widget->isvalid();
}

bool AbstractParameter::isDataParameter()
{
    return true;
}

QString AbstractParameter::getName()
{
    return "_" + _widget->getName();
}

