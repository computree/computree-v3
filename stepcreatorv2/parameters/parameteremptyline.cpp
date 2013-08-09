#include "parameters/parameteremptyline.h"
#include "tools.h"

ParameterEmptyLine::ParameterEmptyLine() : AbstractParameter()
{
    _widget = new WidgetEmptyLine(this);
    onAliasChange();
    setData(QVariant(QColor(Qt::lightGray)),Qt::ForegroundRole);
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
    setText(">");
}

QString ParameterEmptyLine::getParameterDeclaration()
{
    return "";
}

QString ParameterEmptyLine::getParameterInitialization()
{
    return "";
}

QString ParameterEmptyLine::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addEmpty();\n";
}

QString ParameterEmptyLine::getParamaterDoc()
{
    return "";
}

