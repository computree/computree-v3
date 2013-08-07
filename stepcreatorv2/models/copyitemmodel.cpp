#include "copyitemmodel.h"
#include "widgets/copyitemwidget.h"
#include "tools.h"

COPYItemModel::COPYItemModel() : AbstractCopyModel()
{
    _widget = new COPYItemWidget(this);
    setText(getName());
    _status = AbstractCopyModel::S_Added;
    setData(QVariant(QColor(Qt::blue)),Qt::ForegroundRole);
}


void COPYItemModel::init(QString itemType, QString alias, QString name, QString desc)
{
    ((COPYItemWidget*)_widget)->init(itemType, alias, name, desc);
    setText(getName() + " (cpy)");
    _status = AbstractCopyModel::S_Copy;
    setData(QVariant(QColor(Qt::black)),Qt::ForegroundRole);
}

void COPYItemModel::init(INItemModel *inModel)
{
    init(inModel->getItemType(), inModel->getAlias(), inModel->getDisplayableName(), inModel->getDescription());
}


QString COPYItemModel::getName()
{
    return QString("itemCopy_%1").arg(getAlias());
}

QString COPYItemModel::getModelName()
{
    return QString("itemCopyModel_%1").arg(getAlias());
}

QString COPYItemModel::getItemType()
{
    return ((COPYItemWidget*) _widget)->getItemType();
}


void COPYItemModel::getCopyModelsIncludesList(QSet<QString> &list)
{   
    list.insert("#include \"ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h\"");
}

void COPYItemModel::getCopyItemsTypesIncludesList(QSet<QString> &list)
{
    list.insert(QString("#include \"ct_itemdrawable/%1.h\"").arg(getItemType().toLower()));
}

QString COPYItemModel::getCopyModelsDefinition()
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
    result += ((COPYItemWidget*) _widget)->getItemType();
    result += "()";

    QString resultTmp = "";

    // Description
    QString description = ((COPYItemWidget*) _widget)->getDescription();
    if ((description.size() > 0) || (resultTmp.size() > 0))
    {
        resultTmp.insert(0, QString("tr(\"%1\")").arg(description));
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    // Displayable Name
    QString dispName = ((COPYItemWidget*) _widget)->getDisplayableName();
    if ((dispName.size() > 0) || (resultTmp.size() > 0))
    {
        resultTmp.insert(0, QString("tr(\"%1\")").arg(dispName));
        resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp.insert(0, ", \n");
    }

    result += resultTmp;
    result += ");";
    result += "\n";

    getChildrenCopyModelsDefinitions(result);
    return result;

}

QString COPYItemModel::getCopyModelsHierachy()
{
    // Un item n'a pas de hiérarchie sous-jacente
    return "";
}

QString COPYItemModel::getCopyModelAddingCommand()
{
    QString result = ".addItem(";
    result += getModelName();
    result += ");";
    return result;
}

QString COPYItemModel::getCopyComputeBeginning(int rank, QString resultName)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the item model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_OutStandardItemDrawableModel* " + getModelName() + " = (CT_OutStandardItemDrawableModel*)getOutModelForCreation" + "(" + resultName + ", " + getDef() + ");" + "\n";

    return result;

}

QString COPYItemModel::getCopyComputeItemsCreations(QString resultName)
{
    QString result = Tools::getIndentation(1) + "// UNCOMMENT Following lines and complete parameters of the item's contructor\n";
    result += Tools::getIndentation(1) + "// " + getItemType() + "* " + getName() + " = new " +  getItemType() + "(" + getModelName() + ", ID, " + resultName + ");\n";
    return result;
}
