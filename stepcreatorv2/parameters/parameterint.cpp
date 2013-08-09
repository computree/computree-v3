#include "parameters/parameterint.h"
#include "tools.h"

ParameterInt::ParameterInt() : AbstractParameter()
{
    _widget = new WidgetInt(this);
    onAliasChange();
}

void ParameterInt::onAliasChange()
{
    setText(getName() + " (int)");
}

QString ParameterInt::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "int" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterInt::getParameterInitialization()
{
    QString value = QString("%1").arg(widget()->getDefaultValue());
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterInt::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addInt(" +
            "\"" + widget()->getBeforeText()         + "\", " +
            "\"" + widget()->getAfterText()          + "\", " +
            QString("%1").arg(widget()->getMin())    + ", " +
            QString("%1").arg(widget()->getMax())    + ", " +
            widget()->getAlias()                     + ");\n";
}

QString ParameterInt::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

