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
    return Tools::getIndentation(1) + "bool" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterInt::getParameterInitialization()
{
    QString value = (widget()->getDefaultValue()) ? "true" : "false";
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterInt::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addBool(" +
            "\"" + widget()->getBeforeText()   + "\", " +
            "\"" + widget()->getAfterText()    + "\", " +
            "\"" + widget()->getCheckboxText() + "\", " +
            widget()->getAlias()        + ");\n";
}

QString ParameterInt::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

