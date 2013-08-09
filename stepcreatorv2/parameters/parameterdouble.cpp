#include "parameters/parameterdouble.h"
#include "tools.h"

ParameterDouble::ParameterDouble() : AbstractParameter()
{
    _widget = new WidgetDouble(this);
    onAliasChange();
}

void ParameterDouble::onAliasChange()
{
    setText(getName() + " (double)");
}

QString ParameterDouble::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "double" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterDouble::getParameterInitialization()
{
    QString value = QString("%1").arg(widget()->getDefaultValue()/widget()->getMult());
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterDouble::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addDouble(" +
            "\"" + widget()->getBeforeText()         + "\", " +
            "\"" + widget()->getAfterText()          + "\", " +
            QString("%1").arg(widget()->getMin())    + ", " +
            QString("%1").arg(widget()->getMax())    + ", " +
            QString("%1").arg(widget()->getDec())    + ", " +
            widget()->getAlias()                     + ", " +
            QString("%1").arg(widget()->getMult())    + ");\n";
}

QString ParameterDouble::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

