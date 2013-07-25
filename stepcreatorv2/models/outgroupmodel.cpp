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
    return QString("group_%1").arg(getAlias());
}

QString OUTGroupModel::getModelName()
{
    return QString("groupOutModel_%1").arg(getAlias());
}

void OUTGroupModel::getOutModelsIncludesList(QSet<QString> &list)
{
    if (((OUTGroupWidget*) _widget)->getResultType() == OUTGroupWidget::G_ZeroOrMore)
    {
        list.insert("#include \"ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h\"");
    } else if (((OUTGroupWidget*) _widget)->getResultType() == OUTGroupWidget::G_OneOrMore)
    {
        list.insert("#include \"ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h\"");
    } else
    {
        list.insert("#include \"ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h\"");
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

    if (((OUTGroupWidget*) _widget)->getResultType() == OUTGroupWidget::G_ZeroOrMore)
    {
        result += Tools::getIndentation(1);
        result += "CT_InZeroOrMoreGroupModel *";
        result += getModelName();
        result += " = new CT_InZeroOrMoreGroupModel();";
        result += "\n";
    } else if (((OUTGroupWidget*) _widget)->getResultType() == OUTGroupWidget::G_OneOrMore)
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
        if (((OUTGroupWidget*) _widget)->getFinderMode() == OUTGroupWidget::F_Optional)
        {
            resultTmp2 += ", \n";
            resultTmp2 += Tools::getSpaceSequence(indentSize);
            resultTmp2 += "CT_InAbstractGroupModel::FG_IsOptional";
        }

        // ChoiceMode
        if ((((OUTGroupWidget*) _widget)->getChoiceMode() == OUTGroupWidget::C_DontChoose) && (resultTmp2.size() > 0))
        {
            resultTmp2.insert(0, "CT_InAbstractModel::C_DontChoose");
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        } else if (((OUTGroupWidget*) _widget)->getChoiceMode() == OUTGroupWidget::C_OneIfMultiple)
        {
            resultTmp2.insert(0, "CT_InAbstractModel::C_ChooseOneIfMultiple");
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        } else if (((OUTGroupWidget*) _widget)->getChoiceMode() == OUTGroupWidget::C_MultipleIfMultiple)
        {
            resultTmp2.insert(0, "CT_InAbstractModel::C_ChooseMultipleIfMultiple");
            resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp2.insert(0, ", \n");
        }

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

        result += resultTmp2;
        result += ");";
        result += "\n";
    }

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
    QString result = ".addGroup(";
    result += getModelName();
    result += ");";
    return result;
}

QString OUTGroupModel::getOutComputeBeginning(QString resultDef, QString useCopy)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the group model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_InAbstractGroupModel* " + getModelName() + " = (CT_InAbstractGroupModel*)getInModelForResearch" + useCopy + "(" + resultDef + ", " + getDef() + ");" + "\n";

    getChildrenOutComputeBeginning(result, resultDef, useCopy);
    return result;
}


QString OUTGroupModel::getOutComputeLoops(int nbIndent)
{
    QString result = "";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* groupOrItem = (AbstractOutModel*) child(i);

        if (groupOrItem->getModelType() == AbstractOutModel::M_Group_OUT)
        {
            result += "\n";
            result += Tools::getIndentation(nbIndent) + "// Iterating on children groups corresponding to " + groupOrItem->getDef() + "\n";
            result += Tools::getIndentation(nbIndent) + "for ( CT_AbstractItemGroup *" + groupOrItem->getName() + " = " + getName() + "->beginGroup(" + groupOrItem->getModelName() + ")\n";
            result += Tools::getIndentation(nbIndent+1) + "; " + groupOrItem->getName() + " != NULL  && !isStopped()\n";
            result += Tools::getIndentation(nbIndent+1) + "; " + groupOrItem->getName() + " = " + getName() + "->nextGroup() )\n";
            result += Tools::getIndentation(nbIndent) + "{\n";

            result += groupOrItem->getOutComputeLoops(nbIndent+1);

            result += Tools::getIndentation(nbIndent) + "}\n";

        } else if (groupOrItem->getModelType() == AbstractOutModel::M_Item_OUT)
        {
            QString type = ((OUTItemModel*) groupOrItem)->getItemType();
            result += Tools::getIndentation(nbIndent) + "// Gets the item corresponding to " + groupOrItem->getDef() + "\n";
            result += Tools::getIndentation(nbIndent) + "const " + type + "* " + groupOrItem->getName();
            result += " = (const " + type + "*) " + getName() + "->findFirstItem(" + groupOrItem->getModelName() + ");\n";

        }
    }

    return result;
}
