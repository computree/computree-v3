#include "parameters/parameterstringchoice.h"
#include "tools.h"

ParameterStringChoice::ParameterStringChoice() : AbstractParameter()
{
    _widget = new WidgetStringChoice(this);
    onAliasChange();
}

void ParameterStringChoice::onAliasChange()
{
    setText(getName() + " (string choice)");
}

QString ParameterStringChoice::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "bool" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterStringChoice::getParameterInitialization()
{
    QString value = (widget()->getDefaultValue()) ? "true" : "false";
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterStringChoice::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addBool(" +
            "\"" + widget()->getBeforeText()   + "\", " +
            "\"" + widget()->getAfterText()    + "\", " +
            "\"" + widget()->getCheckboxText() + "\", " +
            widget()->getAlias()        + ");\n";
}

QString ParameterStringChoice::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

