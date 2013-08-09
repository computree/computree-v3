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
    setText(widget()->getBeforeText() + " ...");
}

QString ParameterTextLine::getParameterDeclaration()
{
    return "";
}

QString ParameterTextLine::getParameterInitialization()
{
    return "";
}

QString ParameterTextLine::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addText(" +
            "\"" + widget()->getBeforeText() + "\", " +
            "\"" + widget()->getMiddleText() + "\", " +
            "\"" + widget()->getAfterText()  + ");\n";
}

QString ParameterTextLine::getParamaterDoc()
{
    return "";
}

