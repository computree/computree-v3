#include "inresultmodel.h"
#include "tools.h"
#include "assert.h"

INResultModel::INResultModel()
{
    _widget = new INResultWidget();
    setText("Result");
}

INResultModel::~INResultModel()
{
    delete _widget;
}

QString INResultModel::getName()
{
    return QString("result_%1").arg(getAlias());
}


QString INResultModel::getInModelsDefinition()
{
    QString result = "";
    getChildrenInModelsDefinitions(result);

    AbtractModel* childGroup = (AbtractModel*) child(0);
    assert(childGroup!=NULL);

    QString resultClass;
    if (_widget->getResultType()==INResultWidget::R_StandardResult)
    {
        resultClass = "CT_InResultModelGroup";
    } else {
        resultClass = "CT_InResultModelGroupToCopy";
    }

    result += "\n";
    result += "\n";
    result += Tools::getIndentation(1);
    result += resultClass +" *" + getName();
    result += "= new " + resultClass +"(" + getDef() + ", ";
    result += childGroup->getName();
    result += ", \"" + _widget->getDisplayableName() + "\", ";
    result += "\"" + _widget->getDescription() + "\", ";
    result += Tools::getBooleanText(_widget->getRecursive()) + ");";
    result += "\n";
    result += "\n";

    result += Tools::getIndentation(1) + "addInResultModel(" + getName() + ");";
    result += "\n";
    result += "\n";

    return result;
}


QString INResultModel::getInComputeContent()
{
    QString result = "Compute Résultat";

    getChildrenInComputeContent(result);
    return result;
}
