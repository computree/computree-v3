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
    resultTmp += resultClass +" *" + getName();
    resultTmp += " = new " + resultClass +"(";

    int indentSize = resultTmp.size();
    result += resultTmp;

    result += getDef();
    result += ", \n";

    result += Tools::getSpaceSequence(indentSize);
    result += childGroup->getName();
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

    result += Tools::getIndentation(1) + "addInResultModel(" + getName() + ");";
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

QString INResultModel::getInComputeContent()
{
    QString result = "Compute Résultat";

    getChildrenInComputeContent(result);
    return result;
}
