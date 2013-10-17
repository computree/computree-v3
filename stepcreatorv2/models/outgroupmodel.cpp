#include "outgroupmodel.h"
#include "widgets/outgroupwidget.h"
#include "models/outitemmodel.h"
#include "tools.h"

OUTGroupModel::OUTGroupModel() : AbstractOutModel()
{
    _widget = new OUTGroupWidget(this);
    setText(getName());
}

QString OUTGroupModel::getName()
{
    return QString("groupOut_%1").arg(getAlias());
}

QString OUTGroupModel::getModelName()
{
    return QString("groupOutModel_%1").arg(getAlias());
}

void OUTGroupModel::getOutModelsIncludesList(QSet<QString> &list)
{   
    list.insert("#include \"ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h\"");

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        item->getOutModelsIncludesList(list);
    }
}

void OUTGroupModel::getOutItemsTypesIncludesList(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        item->getOutItemsTypesIncludesList(list);
    }
}

QString OUTGroupModel::getOutModelsDefinition()
{
    QString result = "";

    QString resultTmp = "";
    resultTmp += Tools::getIndentation(1);
    resultTmp += "CT_OutStandardGroupModel *";
    resultTmp += getModelName();
    resultTmp += " = new CT_OutStandardGroupModel(";

    int indentSize = resultTmp.size();

    result += resultTmp;
    result += getDef();

    QString resultTmp2 = "";

    // Description
    QString description = ((OUTGroupWidget*) _widget)->getDescription();
    if ((description.size() > 0) || (resultTmp2.size() > 0))
    {
        resultTmp2.insert(0, QString("tr(\"%1\")").arg(description));
        resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp2.insert(0, ", \n");
    }

    // Displayable Name
    QString dispName = ((OUTGroupWidget*) _widget)->getDisplayableName();
    if ((dispName.size() > 0) || (resultTmp2.size() > 0))
    {
        resultTmp2.insert(0, QString("tr(\"%1\")").arg(dispName));
        resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp2.insert(0, ", \n");
    }

    // New CT_StandardItemGroup
    if (resultTmp2.size() > 0)
    {
        resultTmp2.insert(0, "new CT_StandardItemGroup()");
        resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp2.insert(0, ", \n");
    }

    result += resultTmp2;
    result += ");";
    result += "\n";

    getChildrenOutModelsDefinitions(result);
    return result;
}

QString OUTGroupModel::getOutModelsHierachy()
{
    QString result = "";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        result += Tools::getIndentation(1);
        result += getModelName();
        result += item->getOutModelAddingCommand();
        result += "\n";
    }

    result += "\n";
    getChildrenOutModelsHierachy(result);
    return result;
}

QString OUTGroupModel::getOutModelAddingCommand()
{
    QString result = "->addGroup(";
    result += getModelName();
    result += ");";
    return result;
}

QString OUTGroupModel::getOutComputeBeginning(int rank, QString resultName)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the group model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_OutStandardGroupModel* " + getModelName() + " = (CT_OutStandardGroupModel*)getOutModelForCreation" + "(" + resultName + ", " + getDef() + ");" + "\n";

    getChildrenOutComputeBeginning(result, resultName);
    return result;
}


QString OUTGroupModel::getOutComputeItemsCreations(QString resultName)
{
    QString result = "";

    result += Tools::getIndentation(1) + "CT_StandardItemGroup* " + getName() + " = new CT_StandardItemGroup(" + getModelName() + ", " + resultName + ");\n";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* groupOrItem = (AbstractOutModel*) child(i);

        result += groupOrItem->getOutComputeItemsCreations(resultName);

        if (groupOrItem->getModelType() == AbstractOutModel::M_Group_OUT)
        {
            result += Tools::getIndentation(1) + getName() + "->addGroup(" + groupOrItem->getName() + ");\n";
        } else if (groupOrItem->getModelType() == AbstractOutModel::M_Item_OUT)
        {
            result += Tools::getIndentation(1) + "// " + getName() + "->addItemDrawable(" + groupOrItem->getName() + ");\n";
        }
    }

    return result;
}

QString OUTGroupModel::getOutModelDoc(int nbIndent)
{
    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    result += " * " + Tools::getIndentation(nbIndent) + "- CT_StandardItemGroup" + desc + "...\\n\n";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        result += ((AbstractOutModel*) child(i))->getOutModelDoc(nbIndent + 1);
    }
    return result;
}
