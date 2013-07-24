#include "models/inresultmodel.h"
#include "widgets/inresultwidget.h"
#include "tools.h"
#include "assert.h"

INResultModel::INResultModel() : AbstractInModel()
{
    _widget = new INResultWidget(this);
    setText(getName());
}


QString INResultModel::getName()
{
    return QString("result_%1").arg(getAlias());
}

QString INResultModel::getModelName()
{
    return QString("resultInModel_%1").arg(getAlias());
}

void INResultModel::getInModelsIncludesList(QSet<QString> &list)
{
    if (((INResultWidget*) _widget)->getResultType() == INResultWidget::R_StandardResult)
    {
        list.insert("#include \"ct_result/model/inModel/ct_inresultmodelgroup.h\"");
    } else
    {
        list.insert("#include \"ct_result/model/inModel/ct_inresultmodelgrouptocopy.h\"");
    }

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        item->getInModelsIncludesList(list);
    }
}

void INResultModel::getInItemsTypesIncludesList(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractInModel* item = (AbstractInModel*) child(i);
        item->getInItemsTypesIncludesList(list);
    }
}

bool INResultModel::isValid()
{
    if (rowCount()!=1) {return false;}
    return AbstractInModel::isValid();
}

QString INResultModel::getInModelsDefinition()
{
    QString result = "";
    getChildrenInModelsDefinitions(result);
    result += "\n";
    result += getInModelsHierachy();

    AbstractInModel* childGroup = (AbstractInModel*) child(0);
    assert(childGroup!=NULL);

    QString resultClass;
    if (((INResultWidget*) _widget)->getResultType()==INResultWidget::R_StandardResult)
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
    result += "tr(\"" + ((INResultWidget*) _widget)->getDisplayableName() + "\")";
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += "tr(\"" + ((INResultWidget*) _widget)->getDescription() + "\")";
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += Tools::getBooleanText(((INResultWidget*) _widget)->getRecursive()) + ");";
    result += "\n";
    result += "\n";

    result += Tools::getIndentation(1) + "addInResultModel(" + getModelName() + ");";
    result += "\n";

    return result;
}

QString INResultModel::getInModelsHierachy()
{
    QString result = "";
    getChildrenInModelsHierachy(result);
    return result;
}

QString INResultModel::getInModelAddingCommand()
{
    // Jamais appelé dans le cas d'un modèle de résultat
    return "";
}

QString INResultModel::getInComputeBeginning(QString resultDef, QString useCopy)
{
    QString result = "";

    result += "\n";
    result += "\n";
    result += Tools::getIndentation(1) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(1) + "// Get the result corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_ResultGroup* " + getName() + " = getInputResultsForModel(" + getDef() + ").first();" + "\n";
    result += "\n";

    if (((INResultWidget*) _widget)->getResultType() == INResultWidget::R_StandardResult)
    {
        useCopy = "";
    } else
    {
        useCopy = "IfUseCopy";
    }

    getChildrenInComputeBeginning(result, getDef(), useCopy);
    return result;
}

QString INResultModel::getInComputeLoops(int nbIndent)
{
    QString result = "";
    AbstractInModel* group = (AbstractInModel*) child(0);

    result += "\n";
    result += Tools::getIndentation(nbIndent) + "// ----------------------------------------------------------------------------\n";
    result += Tools::getIndentation(nbIndent) + "// Works on the result corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(nbIndent) + "// Iterating on groups situated at the root of the result (corresponding to " + group->getDef() + ")\n";
    result += Tools::getIndentation(nbIndent) + "for ( CT_AbstractItemGroup *" + group->getName() + " = " + getName() + "->beginGroup(" + group->getModelName() + ")\n";
    result += Tools::getIndentation(nbIndent+1) + "; " + group->getName() + " != NULL  && !isStopped()\n";
    result += Tools::getIndentation(nbIndent+1) + "; " + group->getName() + " = " + getName() + "->nextGroup() )\n";
    result += Tools::getIndentation(nbIndent) + "{\n";

    result += group->getInComputeLoops(nbIndent + 1);

    result += Tools::getIndentation(nbIndent) + "}\n";

    return result;
}

