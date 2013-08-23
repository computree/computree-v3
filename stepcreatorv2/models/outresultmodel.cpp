#include "models/outresultmodel.h"
#include "widgets/outresultwidget.h"
#include "tools.h"
#include "assert.h"

OUTResultModel::OUTResultModel() : AbstractOutModel()
{
    _widget = new OUTResultWidget(this);
    setText(getName());
}


QString OUTResultModel::getName()
{
    return QString("resultOut_%1").arg(getAlias());
}

QString OUTResultModel::getModelName()
{
    return QString("resultOutModel_%1").arg(getAlias());
}

void OUTResultModel::getOutModelsIncludesList(QSet<QString> &list)
{
    list.insert("#include \"ct_result/model/outModel/ct_outresultmodelgroup.h\"");

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        item->getOutModelsIncludesList(list);
    }
}

void OUTResultModel::getOutItemsTypesIncludesList(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) child(i);
        item->getOutItemsTypesIncludesList(list);
    }
}

bool OUTResultModel::isValid()
{
    if (rowCount()!=1) {return false;}
    return AbstractOutModel::isValid();
}

QString OUTResultModel::getOutModelsDefinition()
{
    QString result = "";
    getChildrenOutModelsDefinitions(result);
    result += "\n";
    result += getOutModelsHierachy();

    AbstractOutModel* childGroup = (AbstractOutModel*) child(0);
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
    result += "tr(\"" + ((OUTResultWidget*) _widget)->getDisplayableName() + "\")";
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += "tr(\"" + ((OUTResultWidget*) _widget)->getDescription() + "\")";
    result += ");";
    result += "\n";

    result += Tools::getIndentation(1) + "addOutResultModel(" + getModelName() + ");";
    result += "\n";

    return result;
}

QString OUTResultModel::getOutModelsHierachy()
{
    QString result = "";
    getChildrenOutModelsHierachy(result);
    return result;
}

QString OUTResultModel::getOutModelAddingCommand()
{
    // Jamais appelé dans le cas d'un modèle de résultat
    return "";
}

QString OUTResultModel::getOutComputeBeginning(int rank, QString resultName)
{
    QString result = "";

    result += "\n";
    result += Tools::getIndentation(1) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(1) + "// Get the result corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_ResultGroup* " + getName() + " = outResultList.at(" + QString("%1").arg(rank) + ");" + "\n";
    result += "\n";

    getChildrenOutComputeBeginning(result, getName());
    return result;
}

QString OUTResultModel::getOutComputeItemsCreations(QString resultName)
{
    QString result = "";
    AbstractOutModel* group = (AbstractOutModel*) child(0);

    result += "\n";
    result += Tools::getIndentation(1) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(1) + "// Works on the result corresponding to " + getDef() + "\n";
    result += group->getOutComputeItemsCreations(getName());
    result += "\n";
    result += Tools::getIndentation(1) + getName() + "->addGroup(" + group->getName() + ");\n";

    return result;
}

QString OUTResultModel::getOutModelDoc(int nbIndent)
{
    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    result += " * - CT_ResultGroup" + desc + "\\n\n";

    AbstractOutModel* group = (AbstractOutModel*) child(0);
    result += group->getOutModelDoc(nbIndent + 1);
    result += " *\n";
    return result;
}
