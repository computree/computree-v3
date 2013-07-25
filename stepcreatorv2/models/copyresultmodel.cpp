#include "models/copyresultmodel.h"
#include "widgets/copyresultwidget.h"
#include "tools.h"
#include "assert.h"

COPYResultModel::COPYResultModel() : AbstractCopyModel()
{
    _widget = new COPYResultWidget(this);
    setText(getName());
}


QString COPYResultModel::getName()
{
    return QString("resultCopy_%1").arg(getAlias());
}

QString COPYResultModel::getModelName()
{
    return QString("resultCopyModel_%1").arg(getAlias());
}

void COPYResultModel::getCopyModelsIncludesList(QSet<QString> &list)
{
    list.insert("#include \"ct_result/model/outModel/ct_outresultmodelgroup.h\"");

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        item->getCopyModelsIncludesList(list);
    }
}

void COPYResultModel::getCopyItemsTypesIncludesList(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        item->getCopyItemsTypesIncludesList(list);
    }
}

bool COPYResultModel::isValid()
{
    if (rowCount()!=1) {return false;}
    return AbstractCopyModel::isValid();
}

QString COPYResultModel::getCopyModelsDefinition()
{
    QString result = "";
    getChildrenCopyModelsDefinitions(result);
    result += "\n";
    result += getCopyModelsHierachy();

    AbstractCopyModel* childGroup = (AbstractCopyModel*) child(0);
    assert(childGroup!=NULL);

    result += "\n";

    QString resultTmp = "";

    resultTmp += Tools::getIndentation(1);
    resultTmp += "CT_OutResultModelGroup *" + getModelName();
    resultTmp += " = new CT_OutResultModelGroup(";

    int indentSize = resultTmp.size();
    result += resultTmp;

    result += getDef();
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += childGroup->getModelName();
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += "tr(\"" + ((COPYResultWidget*) _widget)->getDisplayableName() + "\")";
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += "tr(\"" + ((COPYResultWidget*) _widget)->getDescription() + "\")";
    result += ");";
    result += "\n";

    result += Tools::getIndentation(1) + "addOutResultModel(" + getModelName() + ");";
    result += "\n";

    return result;
}

QString COPYResultModel::getCopyModelsHierachy()
{
    QString result = "";
    getChildrenCopyModelsHierachy(result);
    return result;
}

QString COPYResultModel::getCopyModelAddingCommand()
{
    // Jamais appelé dans le cas d'un modèle de résultat
    return "";
}

QString COPYResultModel::getCopyComputeBeginning(int rank, QString resultName)
{
    QString result = "";

    result += "\n";
    result += Tools::getIndentation(1) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(1) + "// Get the result corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_ResultGroup* " + getName() + " = outResultList.at(" + QString("%1").arg(rank) + ");" + "\n";
    result += "\n";

    getChildrenCopyComputeBeginning(result, getName());
    return result;
}

QString COPYResultModel::getCopyComputeItemsCreations(QString resultName)
{
    QString result = "";
    AbstractCopyModel* group = (AbstractCopyModel*) child(0);

    result += "\n";
    result += Tools::getIndentation(1) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(1) + "// Works on the result corresponding to " + getDef() + "\n";
    result += group->getCopyComputeItemsCreations(getName());
    result += Tools::getIndentation(1) + getName() + ".addGroup(" + group->getName() + ");\n";

    return result;
}

