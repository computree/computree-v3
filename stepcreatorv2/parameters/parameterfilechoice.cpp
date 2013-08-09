#include "parameters/parameterfilechoice.h"
#include "tools.h"

ParameterFileChoice::ParameterFileChoice() : AbstractParameter()
{
    _widget = new WidgetFileChoice(this);
    onAliasChange();
}

void ParameterFileChoice::onAliasChange()
{
    setText(getName() + " (file choice)");
}

QString ParameterFileChoice::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "QString" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterFileChoice::getParameterInitialization()
{
       return Tools::getIndentation(1) + widget()->getAlias() + " = \"\";\n";
}

QString ParameterFileChoice::getParameterDialogCommands()
{
    return Tools::getIndentation(1) + "configDialog->addFileChoice("+
            "\"" + widget()->getBeforeText()                        + "\", " +
            "CT_FileChoiceButton::" + widget()->getNeededFileType() + ", " +
            "\"" + widget()->getAfterText()                         + "\", " +
            widget()->getAlias()                                    + ");\n";
}

QString ParameterFileChoice::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

