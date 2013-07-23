#include "ingroupmodel.h"
#include "widgets/ingroupwidget.h"
#include "tools.h"

INGroupModel::INGroupModel() : AbstractModel()
{
    _widget = new INGroupWidget(this);
    setText(getName());
}

QString INGroupModel::getName()
{
    return QString("group_%1").arg(getAlias());
}


QString INGroupModel::getInModelsDefinition()
{
    QString result = "";

    if (((INGroupWidget*) _widget)->getResultType() == INGroupWidget::G_ZeroOrMore)
    {
        result += Tools::getIndentation(1);
        result += "CT_InZeroOrMoreGroupModel *";
        result += getName();
        result += " = new CT_InZeroOrMoreGroupModel();";
        result += "\n";
    } else if (((INGroupWidget*) _widget)->getResultType() == INGroupWidget::G_OneOrMore)
    {
        result += Tools::getIndentation(1);
        result += "CT_InOneOrMoreGroupModel *";
        result += getName();
        result += " = new CT_InOneOrMoreGroupModel();";
        result += "\n";
    } else
    {
        QString resultTmp = "";
        resultTmp += Tools::getIndentation(1);
        resultTmp += "CT_InStandardGroupModel *";
        resultTmp += getName();
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
        AbstractModel* item = (AbstractModel*) child(i);
        result += Tools::getIndentation(1);
        result += getName();
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
    result += getName();
    result += ");";
    return result;
}

QString INGroupModel::getInComputeContent()
{
    QString result = "Compute Groupe";

    getChildrenInComputeContent(result);
    return result;
}
