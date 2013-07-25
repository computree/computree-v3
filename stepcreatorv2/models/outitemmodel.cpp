#include "outitemmodel.h"
#include "widgets/outitemwidget.h"
#include "tools.h"

OUTItemModel::OUTItemModel() : AbstractOutModel()
{
    _widget = new OUTItemWidget(this);
    setText(getName());
}

QString OUTItemModel::getName()
{
    return QString("item_%1").arg(getAlias());
}

QString OUTItemModel::getModelName()
{
    return QString("itemOutModel_%1").arg(getAlias());
}

QString OUTItemModel::getItemType()
{
    return ((OUTItemWidget*) _widget)->getItemType();
}


void OUTItemModel::getOutModelsIncludesList(QSet<QString> &list)
{
    list.insert("#include \"ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h\"");
}

void OUTItemModel::getOutItemsTypesIncludesList(QSet<QString> &list)
{
    list.insert(QString("#include \"ct_itemdrawable/%1.h\"").arg(getItemType().toLower()));
}

QString OUTItemModel::getOutModelsDefinition()
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
    result += ((OUTItemWidget*) _widget)->getItemType();
    result += "::staticGetType()";

    QString resultTmp = "";

    // FinderMode
    if (((OUTItemWidget*) _widget)->getFinderMode() == OUTItemWidget::F_Optional)
    {
        resultTmp += ", \n";
        resultTmp += Tools::getSpaceSequence(indentSize);
        resultTmp += "CT_InStandardItemDrawableModel::FG_IsOptional";
    }

    // ChoiceMode
    if ((((OUTItemWidget*) _widget)->getChoiceMode() == OUTItemWidget::C_OneIfMultiple) && (resultTmp.size() > 0))
    {
        resultTmp.insert(0, "CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple");
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    } else if (((OUTItemWidget*) _widget)->getChoiceMode() == OUTItemWidget::C_MultipleIfMultiple)
    {
        resultTmp.insert(0, "CT_InStandardItemDrawableModel::C_ChooseMultipleIfMultiple");
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    // Description
    QString description = ((OUTItemWidget*) _widget)->getDescription();
    if ((description.size() > 0) || (resultTmp.size() > 0))
    {
        resultTmp.insert(0, QString("tr(\"%1\")").arg(description));
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    // Displayable Name
    QString dispName = ((OUTItemWidget*) _widget)->getDisplayableName();
    if ((dispName.size() > 0) || (resultTmp.size() > 0))
    {
        resultTmp.insert(0, QString("tr(\"%1\")").arg(dispName));
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    result += resultTmp;
    result += ");";
    result += "\n";

    getChildrenOutModelsDefinitions(result);
    return result;

}

QString OUTItemModel::getOutModelsHierachy()
{
    // Un item n'a pas de hiérarchie sous-jacente
    return "";
}

QString OUTItemModel::getOutModelAddingCommand()
{
    QString result = ".addItem(";
    result += getModelName();
    result += ");";
    return result;
}

QString OUTItemModel::getOutComputeBeginning(QString resultDef, QString useCopy)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the group model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_InAbstractItemDrawableModel* " + getModelName() + " = (CT_InAbstractItemDrawableModel*)getInModelForResearch" + useCopy + "(" + resultDef + ", " + getDef() + ");" + "\n";

    return result;

}

QString OUTItemModel::getOutComputeLoops(int nbIndent)
{
    return "";
}
