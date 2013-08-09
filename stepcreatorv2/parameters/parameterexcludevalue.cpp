#include "parameters/parameterexcludevalue.h"
#include "tools.h"

ParameterExcludeValue::ParameterExcludeValue() : AbstractParameter()
{
    _widget = new WidgetExcludeValue(this);
    onAliasChange();
}

void ParameterExcludeValue::onAliasChange()
{
    setText(getName() + " (exclude values)");
}

QString ParameterExcludeValue::getParameterDeclaration()
{
    return Tools::getIndentation(1) + "int" + Tools::getIndentation(1) + widget()->getAlias() + ";" +
           Tools::getIndentation(1) + "/*!< " + widget()->getDescription() + " */\n";
}

QString ParameterExcludeValue::getParameterInitialization()
{
    QString value = QString("%1").arg(widget()->getDefaultValue());
    return Tools::getIndentation(1) + widget()->getAlias() + " = " + value + ";\n";
}

QString ParameterExcludeValue::getParameterDialogCommands()
{
    QString result = "";

    result += "\n";
    result += Tools::getIndentation(1) + "CT_ButtonGroup &bg" + widget()->getAlias() + " = configDialog->addButtonGroup(" + widget()->getAlias() + ");\n";
    result += "\n";

    QStringList list = widget()->getPossibleValues();
    for (int i = 0 ; i < list.size() ; i++)
    {
        result += Tools::getIndentation(1) + "configDialog->addExcludeValue(" +
                    "\"" + widget()->getBeforeText()   + "\", " +
                    "\"" + widget()->getAfterText()    + "\", " +
                    "\"" + list.at(i)                  + "\", " +
                    "bg" + widget()->getAlias()        + ", " +
                    QString("%1").arg(i)               + ");\n";
    }


    return result;
}

QString ParameterExcludeValue::getParamaterDoc()
{
    return " * \\param " + widget()->getAlias() + " " + widget()->getDescription() + "\n";
}

#ifdef TOTO
// Paramètre prenant une valeur de choix
// Création du groupe de valeurs exclusives dans la boite de dialogue (non visible)
CT_ButtonGroup &buttonGroup = configDialog->addButtonGroup(_choiceParam);

// Création des items séléctionnables de façon exclusive (visibles)
configDialog->addExcludeValue("Valeur exclusive 1", "labelAfter", "labelRadioButton", buttonGroup, 1);
configDialog->addExcludeValue("Valeur exclusive 2", "labelAfter", "labelRadioButton", buttonGroup, 2);
configDialog->addExcludeValue("Valeur exclusive 3", "labelAfter", "labelRadioButton", buttonGroup, 3);

#endif
