#include "parameters/parameterexcludevalue.h"
#include "tools.h"

ParameterExcludeValue::ParameterExcludeValue() : AbstractParameter()
{
    _widget = new WidgetExcludeValue(this);
    onAliasChange();
}

void ParameterExcludeValue::onAliasChange()
{
    setText(getName() + " (exclude value)");
}

QString ParameterExcludeValue::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "bool" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterExcludeValue::getParameterInitialization()
{
    QString value = (widget()->getDefaultValue()) ? "true" : "false";
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterExcludeValue::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addBool(" +
            "\"" + widget()->getBeforeText()   + "\", " +
            "\"" + widget()->getAfterText()    + "\", " +
            "\"" + widget()->getCheckboxText() + "\", " +
            widget()->getAlias()        + ");\n";
}

QString ParameterExcludeValue::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

