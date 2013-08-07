#include "copygroupmodel.h"
#include "widgets/copygroupwidget.h"
#include "models/copyitemmodel.h"
#include "tools.h"

COPYGroupModel::COPYGroupModel() : AbstractCopyModel()
{
    _widget = new COPYGroupWidget(this);
    _status = AbstractCopyModel::S_Added;
    setData(QVariant(QColor(Qt::blue)),Qt::ForegroundRole);
    setText(getName());
}

void COPYGroupModel::init(QString alias, QString name, QString desc)
{
    ((COPYGroupWidget*)_widget)->init(alias, name, desc);
    _status = AbstractCopyModel::S_Copy;
    setData(QVariant(QColor(Qt::black)),Qt::ForegroundRole);
    setText(getName() + " (copie)");
}

void COPYGroupModel::init(INGroupModel *inModel)
{
    init(inModel->getAlias(), inModel->getDisplayableName(), inModel->getDescription());
}


QString COPYGroupModel::getName()
{
    if (_status == AbstractCopyModel::S_Added)
    {
        return QString("groupOut_%1").arg(getAlias());
    } else {
        return QString("groupIn_%1").arg(getAlias());
    }
}

QString COPYGroupModel::getModelName()
{
    return QString("groupCopyModel_%1").arg(getAlias());
}

void COPYGroupModel::getCopyModelsIncludesList(QSet<QString> &list)
{   
    list.insert("#include \"ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h\"");
}

void COPYGroupModel::getCopyItemsTypesIncludesList(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        item->getCopyItemsTypesIncludesList(list);
    }
}

QString COPYGroupModel::getCopyModelsDefinition()
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
    QString description = ((COPYGroupWidget*) _widget)->getDescription();
    if ((description.size() > 0) || (resultTmp2.size() > 0))
    {
        resultTmp2.insert(0, QString("tr(\"%1\")").arg(description));
        resultTmp2.insert(0, Tools::getSpaceSequence(indentSize));
        resultTmp2.insert(0, ", \n");
    }

    // Displayable Name
    QString dispName = ((COPYGroupWidget*) _widget)->getDisplayableName();
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

    getChildrenCopyModelsDefinitions(result);
    return result;
}

QString COPYGroupModel::getCopyModelsHierachy()
{
    QString result = "";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result += Tools::getIndentation(1);
        result += getModelName();
        result += item->getCopyModelAddingCommand();
        result += "\n";
    }

    result += "\n";
    getChildrenCopyModelsHierachy(result);
    return result;
}

QString COPYGroupModel::getCopyModelAddingCommand()
{
    QString result = ".addGroup(";
    result += getModelName();
    result += ");";
    return result;
}

QString COPYGroupModel::getCopyComputeBeginning(int rank, QString resultName)
{
    QString result = "";

    result += Tools::getIndentation(1) + "// Get the group model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_OutStandardGroupModel* " + getModelName() + " = (CT_OutStandardGroupModel*)getOutModelForCreation" + "(" + resultName + ", " + getDef() + ");" + "\n";

    getChildrenCopyComputeBeginning(result, resultName);
    return result;
}


QString COPYGroupModel::getCopyComputeItemsCreations(QString resultName)
{
    QString result = "";

    result += Tools::getIndentation(1) + "CT_StandardItemGroup* " + getName() + " = new CT_StandardItemGroup(" + getModelName() + ", 0, " + resultName + ");\n";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* groupOrItem = (AbstractCopyModel*) child(i);

        result += groupOrItem->getCopyComputeItemsCreations(resultName);

        if (groupOrItem->getModelType() == AbstractCopyModel::M_Group_COPY)
        {
            result += Tools::getIndentation(1) + getName() + ".addGroup(" + groupOrItem->getName() + ");\n";
        } else if (groupOrItem->getModelType() == AbstractCopyModel::M_Item_COPY)
        {
            result += Tools::getIndentation(1) + "// " + getName() + ".addItem(" + groupOrItem->getName() + ");\n";
        }
    }

    return result;
}
