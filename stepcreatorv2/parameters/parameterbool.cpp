#include "parameters/parameterbool.h"
#include "parameters/widgets/widgetbool.h"
#include "tools.h"

int ParameterBool::val = 0;

ParameterBool::ParameterBool() : AbstractParameter()
{
    _widget = new WidgetBool(this);
    setText(getName());
}

QString ParameterBool::getName()
{
    return "toto_"+ QString("%1").arg(val++);
}
