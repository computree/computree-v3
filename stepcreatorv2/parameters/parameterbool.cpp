#include "parameters/parameterbool.h"
#include "tools.h"

ParameterBool::ParameterBool() : AbstractParameter()
{
    _widget = new WidgetBool(this);
    onAliasChange();
}

void ParameterBool::onAliasChange()
{
    setText(getName() + " (bool)");
}

QString ParameterBool::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "bool" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterBool::getParameterInitialization()
{
    QString value = (widget()->getDefaultValue()) ? "true" : "false";
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterBool::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addBool(" +
            "\"" + widget()->getBeforeText()   + "\", " +
            "\"" + widget()->getAfterText()    + "\", " +
            "\"" + widget()->getCheckboxText() + "\", " +
            widget()->getAlias()        + ");\n";
}

QString ParameterBool::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

