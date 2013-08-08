#include "ingroupmodel.h"
#include "widgets/ingroupwidget.h"
#include "models/initemmodel.h"
#include "tools.h"

INGroupModel::INGroupModel() : AbstractInModel()
{
    _widget = new INGroupWidget(this);
    setText(getName());
}

QString INGroupModel::getName()
{
    return QString("groupIn_%1").arg(getAlias());
}

QString INGroupModel::getModelName()
{
    return QString("groupInModel_%1").arg(getAlias());
}

void INGroupModel::getInModelsIncludesList(QSet<QString> &list)
{
    if (((INGroupWidget*) _widget)->getResultType() == INGroupWidget::G_ZeroOrMore)
    {
        list.insert("#include \"ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h\"");
    } else if (((INGroupWidget*) _widget)->getResultType() == INGroupWidget::G_OneOrMore)
    {
        list.insert("#include \"ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h\"");
    } else
    {
        list.insert("#include \"ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h\"");
    }
}

void INGroupModel::getInItemsTypesIncludesList(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        item->getInItemsTypesIncludesList(list);
    }
}

QString INGroupModel::getInModelsDefinition()
{
    QString result = "";

    if (((INGroupWidget*) _widget)->getResultType() == INGroupWidget::G_ZeroOrMore)
    {
        result += Tools::getIndentation(1);
        result += "CT_InZeroOrMoreGroupModel *";
        result += getModelName();
        result += " = new CT_InZeroOrMoreGroupModel();";
        result += "\n";
    } else if (((INGroupWidget*) _widget)->getResultType() == INGroupWidget::G_OneOrMore)
    {
        result += Tools::getIndentation(1);
        result += "CT_InOneOrMoreGroupModel *";
        result += getModelName();
        result += " = new CT_InOneOrMoreGroupModel();";
        result += "\n";
    } else
    {
        QString resultTmp = "";
        resultTmp += Tools::getIndentation(1);
        resultTmp += "CT_InStandardGroupModel *";
        resultTmp += getModelName();
        resultTmp += " = new CT_InStandardGroupModel(";

        int indentSize = resultTmp.size();

        result += resultTmp;
        result += getDef();

        QString resultTmp2 = "";

        // FinderMode
        if (((INGroupWidget*) _widget)->getFinderMode() == INGroupWidget::F_Optional)
        {
            resultTmp2 += ", \n";
            resultTmp2 += Tools::getSpaceSequence(indentSize);
            resultTmp2 += "CT_InAbstractGroupModel::FG_IsOptional";
        }

        // ChoiceMode
        if ((((INGroupWidget*) _widget)->getChoiceMode() == INGroupWidget::C_DontChoose) && (resultTmp2.size() > 0))
        {
            resultTmp2.insert(0, "CT_InAbstractModel::C_DontChoose");
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        } else if (((INGroupWidget*) _widget)->getChoiceMode() == INGroupWidget::C_OneIfMultiple)
        {
            resultTmp2.insert(0, "CT_InAbstractModel::C_ChooseOneIfMultiple");
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        } else if (((INGroupWidget*) _widget)->getChoiceMode() == INGroupWidget::C_MultipleIfMultiple)
        {
            resultTmp2.insert(0, "CT_InAbstractModel::C_ChooseMultipleIfMultiple");
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        }

        // Description
        QString description = ((INGroupWidget*) _widget)->getDescription();
        if ((description.size() > 0) || (resultTmp2.size() > 0))
        {
            resultTmp2.insert(0, QString("tr(\"%1\")").arg(description));
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        }

        // Displayable Name
        QString dispName = ((INGroupWidget*) _widget)->getDisplayableName();
        if ((dispName.size() > 0) || (resultTmp2.size() > 0))
        {
            resultTmp2.insert(0, QString("tr(\"%1\")").arg(dispName));
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        }

        result += resultTmp2;
        result += ");";
        result += "\n";
    }

    getChildrenInModelsDefinitions(result);
    return result;
}

QString INGroupModel::getInModelsHierachy()
{
    QString result = "";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        result += Tools::getIndentation(1);
        result += getModelName();
        result += item->getInModelAddingCommand();
        result += "\n";
    }

    result += "\n";
    getChildrenInModelsHierachy(result);
    return result;
}

QString INGroupModel::getInModelAddingCommand()
{
    QString result = ".addGroup(";
    result += getModelName();
    result += ");";
    return result;
}

QString INGroupModel::getInComputeBeginning(QString resultDef, QString useCopy)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the group model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_InAbstractGroupModel* " + getModelName() + " = (CT_InAbstractGroupModel*)getInModelForResearch" + useCopy + "(" + resultDef + ", " + getDef() + ");" + "\n";

    getChildrenInComputeBeginning(result, resultDef, useCopy);
    return result;
}


QString INGroupModel::getInComputeLoops(int nbIndent)
{
    QString result = "";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* groupOrItem = (AbstractInModel*) child(i);

        if (groupOrItem->getModelType() == AbstractInModel::M_Group_IN)
        {
            result += "\n";
            result += Tools::getIndentation(nbIndent) + "// Iterating on children groups corresponding to " + groupOrItem->getDef() + "\n";
            result += Tools::getIndentation(nbIndent) + "for ( CT_AbstractItemGroup *" + groupOrItem->getName() + " = " + getName() + "->beginGroup(" + groupOrItem->getModelName() + ")\n";
            result += Tools::getIndentation(nbIndent+1) + "; " + groupOrItem->getName() + " != NULL  && !isStopped()\n";
            result += Tools::getIndentation(nbIndent+1) + "; " + groupOrItem->getName() + " = " + getName() + "->nextGroup() )\n";
            result += Tools::getIndentation(nbIndent) + "{\n";

            result += groupOrItem->getInComputeLoops(nbIndent+1);

            result += Tools::getIndentation(nbIndent) + "}\n";

        } else if (groupOrItem->getModelType() == AbstractInModel::M_Item_IN)
        {
            QString type = ((INItemModel*) groupOrItem)->getItemType();
            result += Tools::getIndentation(nbIndent) + "// Gets the item corresponding to " + groupOrItem->getDef() + "\n";
            result += Tools::getIndentation(nbIndent) + "const " + type + "* " + groupOrItem->getName();
            result += " = (const " + type + "*) " + getName() + "->findFirstItem(" + groupOrItem->getModelName() + ");\n";

        }
    }

    return result;
}

QString INGroupModel::getInModelDoc(int nbIndent)
{
    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    result += " * " + Tools::getIndentation(nbIndent) + "- CT_StandardItemGroup" + desc + "...\\n\n";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        result += ((AbstractInModel*) child(i))->getInModelDoc(nbIndent + 1);
    }
    return result;
}
