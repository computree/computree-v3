#include "initemmodel.h"
#include "widgets/initemwidget.h"
#include "tools.h"

INItemModel::INItemModel() : AbstractModel()
{
    _widget = new INItemWidget(this);
    setText(getName());
}

QString INItemModel::getName()
{
    return QString("item_%1").arg(getAlias());
}

QString INItemModel::getInModelsDefinition()
{
    QString result = "";

    result += Tools::getIndentation(1);
    result += "CT_InStandardItemDrawableModel *";
    result += getName();
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
    result += getName();
    result += ");";
    return result;
}

QString INItemModel::getInComputeContent()
{
    QString result = "Compute Item";

    getChildrenInComputeContent(result);
    return result;
}
