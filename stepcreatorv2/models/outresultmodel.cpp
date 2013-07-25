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
    return QString("result_%1").arg(getAlias());
}

QString OUTResultModel::getModelName()
{
    return QString("resultOutModel_%1").arg(getAlias());
}

void OUTResultModel::getOutModelsIncludesList(QSet<QString> &list)
{
    if (((OUTResultWidget*) _widget)->getResultType() == OUTResultWidget::R_StandardResult)
    {
        list.insert("#include \"ct_result/model/inModel/ct_inresultmodelgroup.h\"");
    } else
    {
        list.insert("#include \"ct_result/model/inModel/ct_inresultmodelgrouptocopy.h\"");
    }

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

    QString resultClass;
    if (((OUTResultWidget*) _widget)->getResultType()==OUTResultWidget::R_StandardResult)
    {
        resultClass = "CT_InResultModelGroup";
    } else {
        resultClass = "CT_InResultModelGroupToCopy";
    }

    result += "\n";

    QString resultTmp = "";

    resultTmp += Tools::getIndentation(1);
    resultTmp += resultClass +" *" + getModelName();
    resultTmp += " = new " + resultClass +"(";

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
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += Tools::getBooleanText(((OUTResultWidget*) _widget)->getRecursive()) + ");";
    result += "\n";
    result += "\n";

    result += Tools::getIndentation(1) + "addInResultModel(" + getModelName() + ");";
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
    // Jamais appel� dans le cas d'un mod�le de r�sultat
    return "";
}

QString OUTResultModel::getOutComputeBeginning(QString resultDef, QString useCopy)
{
    QString result = "";

    result += "\n";
    result += "\n";
    result += Tools::getIndentation(1) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(1) + "// Get the result corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_ResultGroup* " + getName() + " = getInputResultsForModel(" + getDef() + ").first();" + "\n";
    result += "\n";

    if (((OUTResultWidget*) _widget)->getResultType() == OUTResultWidget::R_StandardResult)
    {
        useCopy = "";
    } else
    {
        useCopy = "IfUseCopy";
    }

    getChildrenOutComputeBeginning(result, getDef(), useCopy);
    return result;
}

QString OUTResultModel::getOutComputeLoops(int nbIndent)
{
    QString result = "";
    AbstractOutModel* group = (AbstractOutModel*) child(0);

    result += "\n";
    result += Tools::getIndentation(nbIndent) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(nbIndent) + "// Works on the result corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(nbIndent) + "// Iterating on groups situated at the root of the result (corresponding to " + group->getDef() + ")\n";
    result += Tools::getIndentation(nbIndent) + "for ( CT_AbstractItemGroup *" + group->getName() + " = " + getName() + "->beginGroup(" + group->getModelName() + ")\n";
    result += Tools::getIndentation(nbIndent+1) + "; " + group->getName() + " != NULL  && !isStopped()\n";
    result += Tools::getIndentation(nbIndent+1) + "; " + group->getName() + " = " + getName() + "->nextGroup() )\n";
    result += Tools::getIndentation(nbIndent) + "{\n";

    result += group->getOutComputeLoops(nbIndent + 1);

    result += Tools::getIndentation(nbIndent) + "}\n";

    return result;
}

