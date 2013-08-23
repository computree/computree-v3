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
    return QString("itemOut_%1").arg(getAlias());
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
    list.insert("#include \"ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h\"");
}

void OUTItemModel::getOutItemsTypesIncludesList(QSet<QString> &list)
{
    list.insert(QString("#include \"ct_itemdrawable/%1.h\"").arg(getItemType().toLower()));
}

QString OUTItemModel::getOutModelsDefinition()
{
    QString result = "";

    result += Tools::getIndentation(1);
    result += "CT_OutStandardItemDrawableModel *";
    result += getModelName();
    result += " = new CT_OutStandardItemDrawableModel(";

    int indentSize = result.size();

    result += getDef();

    // Item Type
    result += ", \n";
    result += Tools::getSpaceSequence(indentSize) + "new ";
    result += ((OUTItemWidget*) _widget)->getItemType();
    result += "()";

    QString resultTmp = "";

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
    QString result = "->addItem(";
    result += getModelName();
    result += ");";
    return result;
}

QString OUTItemModel::getOutComputeBeginning(int rank, QString resultName)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the item model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_OutStandardItemDrawableModel* " + getModelName() + " = (CT_OutStandardItemDrawableModel*)getOutModelForCreation" + "(" + resultName + ", " + getDef() + ");" + "\n";

    return result;

}

QString OUTItemModel::getOutComputeItemsCreations(QString resultName)
{
    QString result = "\n";
    result += Tools::getIndentation(1) + "// UNCOMMENT Following lines and complete parameters of the item's contructor\n";
    result += Tools::getIndentation(1) + "// " + getItemType() + "* " + getName() + " = new " +  getItemType() + "(" + getModelName() + ", ID, " + resultName + ");\n";
    return result;
}

QString OUTItemModel::getOutModelDoc(int nbIndent)
{
    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    result += " * " + Tools::getIndentation(nbIndent) + "- " + getItemType() + desc + "\\n\n";

    return result;
}
