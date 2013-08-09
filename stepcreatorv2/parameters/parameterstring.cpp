#include "parameters/parameterstring.h"
#include "tools.h"

ParameterString::ParameterString() : AbstractParameter()
{
    _widget = new WidgetString(this);
    onAliasChange();
}

void ParameterString::onAliasChange()
{
    setText(getName() + " (string)");
}

QString ParameterString::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "QString" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterString::getParameterInitialization()
{
    return Tools::getIndentation(1) + widget()->getAlias() + " = \"" + widget()->getDefaultValue() + "\";\n";
}

QString ParameterString::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addString("+
            "\"" + widget()->getBeforeText()                    + "\", " +
            "\"" + widget()->getAfterText()                     + "\", " +
            widget()->getAlias()                                + ");\n";
}

QString ParameterString::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

