#include "models/inresultmodel.h"
#include "widgets/inresultwidget.h"
#include "tools.h"
#include "assert.h"

INResultModel::INResultModel() : AbstractModel()
{
    _widget = new INResultWidget(this);
    setText(getName());
}


QString INResultModel::getName()
{
    return QString("result_%1").arg(getAlias());
}

bool INResultModel::isValid()
{
    if (rowCount()!=1) {return false;}
    return AbstractModel::isValid();
}

QString INResultModel::getInModelsDefinition()
{
    QString result = "";
    getChildrenInModelsDefinitions(result);

    AbstractModel* childGroup = (AbstractModel*) child(0);
    assert(childGroup!=NULL);

    QString resultClass;
    if (((INResultWidget*) _widget)->getResultType()==INResultWidget::R_StandardResult)
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
    result += ", \"" + ((INResultWidget*) _widget)->getDisplayableName() + "\", ";
    result += "\"" + ((INResultWidget*) _widget)->getDescription() + "\", ";
    result += Tools::getBooleanText(((INResultWidget*) _widget)->getRecursive()) + ");";
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
