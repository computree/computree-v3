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
    return Tools::getIndentation(1) + "QString" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterStringChoice::getParameterInitialization()
{
    return Tools::getIndentation(1) + widget()->getAlias() + " = \"" + widget()->getDefaultValue() + "\";\n";
}

QString ParameterStringChoice::getParameterDialogCommands()
{
    QString result = "";

    result += "\n";
    result += Tools::getIndentation(1) + "QStringList " + "list" + widget()->getAlias() + ";\n";

    QStringList list = widget()->getPossibleValues();
    for (int i = 0 ; i < list.size() ; i++)
    {
        result += Tools::getIndentation(1) + "list" + widget()->getAlias() + ".append(\"" + list.at(i) + "\");\n";
    }

    result += "\n";
    result += Tools::getIndentation(1) + "configDialog->addStringChoice(" +
                "\"" + widget()->getBeforeText()   + "\", " +
                "\"" + widget()->getAfterText()    + "\", " +
                "list" + widget()->getAlias()      + ", " +
                widget()->getAlias()               + ");\n";

    return result;
}

QString ParameterStringChoice::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

