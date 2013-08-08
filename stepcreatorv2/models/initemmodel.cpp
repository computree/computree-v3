#include "initemmodel.h"
#include "widgets/initemwidget.h"
#include "tools.h"

INItemModel::INItemModel() : AbstractInModel()
{
    _widget = new INItemWidget(this);
    setText(getName());
}

QString INItemModel::getName()
{
    return QString("itemIn_%1").arg(getAlias());
}

QString INItemModel::getModelName()
{
    return QString("itemInModel_%1").arg(getAlias());
}

QString INItemModel::getItemType()
{
    return ((INItemWidget*) _widget)->getItemType();
}


void INItemModel::getInModelsIncludesList(QSet<QString> &list)
{
    list.insert("#include \"ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h\"");
}

void INItemModel::getInItemsTypesIncludesList(QSet<QString> &list)
{
    list.insert(QString("#include \"ct_itemdrawable/%1.h\"").arg(getItemType().toLower()));
}

QString INItemModel::getInModelsDefinition()
{
    QString result = "";

    result += Tools::getIndentation(1);
    result += "CT_InStandardItemDrawableModel *";
    result += getModelName();
    result += " = new CT_InStandardItemDrawableModel(";

    int indentSize = result.size();

    result += getDef();

    // Item Type
    result += ", \n";
    result += Tools::getSpaceSequence(indentSize);
    result += ((INItemWidget*) _widget)->getItemType();
    result += "::staticGetType()";

    QString resultTmp = "";

    // FinderMode
    if (((INItemWidget*) _widget)->getFinderMode() == INItemWidget::F_Optional)
    {
        resultTmp += ", \n";
        resultTmp += Tools::getSpaceSequence(indentSize);
        resultTmp += "CT_InStandardItemDrawableModel::FG_IsOptional";
    }

    // ChoiceMode
    if ((((INItemWidget*) _widget)->getChoiceMode() == INItemWidget::C_OneIfMultiple) && (resultTmp.size() > 0))
    {
        resultTmp.insert(0, "CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple");
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    } else if (((INItemWidget*) _widget)->getChoiceMode() == INItemWidget::C_MultipleIfMultiple)
    {
        resultTmp.insert(0, "CT_InStandardItemDrawableModel::C_ChooseMultipleIfMultiple");
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    // Description
    QString description = ((INItemWidget*) _widget)->getDescription();
    if ((description.size() > 0) || (resultTmp.size() > 0))
    {
        resultTmp.insert(0, QString("tr(\"%1\")").arg(description));
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    // Displayable Name
    QString dispName = ((INItemWidget*) _widget)->getDisplayableName();
    if ((dispName.size() > 0) || (resultTmp.size() > 0))
    {
        resultTmp.insert(0, QString("tr(\"%1\")").arg(dispName));
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    result += resultTmp;
    result += ");";
    result += "\n";

    getChildrenInModelsDefinitions(result);
    return result;

}

QString INItemModel::getInModelsHierachy()
{
    // Un item n'a pas de hiérarchie sous-jacente
    return "";
}

QString INItemModel::getInModelAddingCommand()
{
    QString result = ".addItem(";
    result += getModelName();
    result += ");";
    return result;
}

QString INItemModel::getInComputeBeginning(QString resultDef, QString useCopy)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the group model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_InAbstractItemDrawableModel* " + getModelName() + " = (CT_InAbstractItemDrawableModel*)getInModelForResearch" + useCopy + "(" + resultDef + ", " + getDef() + ");" + "\n";

    return result;

}

QString INItemModel::getInComputeLoops(int nbIndent)
{
    return "";
}

QString INItemModel::getInModelDoc(int nbIndent)
{
    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    result += " * " + Tools::getIndentation(nbIndent) + "- " + getItemType() + desc + "\\n\n";

    return result;
}
