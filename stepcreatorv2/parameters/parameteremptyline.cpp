#include "parameters/parameteremptyline.h"
#include "tools.h"

ParameterEmptyLine::ParameterEmptyLine() : AbstractParameter()
{
    _widget = new WidgetEmptyLine(this);
    onAliasChange();
}

bool  ParameterEmptyLine::isValid()
{
    return true;
}

bool ParameterEmptyLine::isDataParameter()
{
    return false;
}

void ParameterEmptyLine::onAliasChange()
{
    setText("Ligne vide");
}

QString ParameterEmptyLine::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "bool" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterEmptyLine::getParameterInitialization()
{
    QString value = (widget()->getDefaultValue()) ? "true" : "false";
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterEmptyLine::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addBool(" +
            "\"" + widget()->getBeforeText()   + "\", " +
            "\"" + widget()->getAfterText()    + "\", " +
            "\"" + widget()->getCheckboxText() + "\", " +
            widget()->getAlias()        + ");\n";
}

QString ParameterEmptyLine::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

