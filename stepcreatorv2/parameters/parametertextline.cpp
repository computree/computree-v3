#include "parameters/parametertextline.h"
#include "tools.h"

ParameterTextLine::ParameterTextLine() : AbstractParameter()
{
    _widget = new WidgetTextLine(this);
    onAliasChange();
}

bool  ParameterTextLine::isValid()
{
    return true;
}

bool ParameterTextLine::isDataParameter()
{
    return false;
}

void ParameterTextLine::onAliasChange()
{
    setText("Ligne de texte");
}

QString ParameterTextLine::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "bool" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterTextLine::getParameterInitialization()
{
    QString value = (widget()->getDefaultValue()) ? "true" : "false";
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterTextLine::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addBool(" +
            "\"" + widget()->getBeforeText()   + "\", " +
            "\"" + widget()->getAfterText()    + "\", " +
            "\"" + widget()->getCheckboxText() + "\", " +
            widget()->getAlias()        + ");\n";
}

QString ParameterTextLine::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

