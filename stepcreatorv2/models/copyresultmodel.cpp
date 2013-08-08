#include "models/copyresultmodel.h"
#include "widgets/copyresultwidget.h"
#include "tools.h"
#include "assert.h"

COPYResultModel::COPYResultModel() : AbstractCopyModel()
{
    _widget = new COPYResultWidget(this);
    setText(getName() + " (copie)");
}


void COPYResultModel::init(QString alias, QString name, QString desc)
{
    ((COPYResultWidget*)_widget)->init(alias, name, desc);
    setText(getName() + " (copie)");
}

void COPYResultModel::init(INResultModel *inModel)
{
    init(inModel->getAlias(), inModel->getDisplayableName(), inModel->getDescription());
}

QString COPYResultModel::getName()
{
    return QString("resultIn_%1").arg(getAlias());
}

QString COPYResultModel::getModelName()
{
    return QString("resultCopyModel_%1").arg(getAlias());
}

QString COPYResultModel::getAutoRenameName()
{
    return "";
}

bool COPYResultModel::isValid()
{
    if (rowCount()!=1) {return false;}
    return AbstractCopyModel::isValid();
}

void COPYResultModel::getActionsIncludes(QSet<QString> &list)
{
    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        item->getActionsIncludes(list);
    }
}

QString COPYResultModel::getCopyModelsDefinitions(QString actionName)
{
    QString result = "";

    actionName = "actions_" + getModelName();

    result += Tools::getIndentation(1) + "// Get IN model corresponding to " + getDef() + "\n";
    result += Tools::getIndentation(1) + "CT_InResultModelGroupToCopy *" + getModelName() + " = (CT_InResultModelGroupToCopy*)getInResultModel(" + getDef() + ");\n";
    result += "\n";
    result += Tools::getIndentation(1) + "// Create an action list to modify the in model (empty in no modifications)\n";
    result += Tools::getIndentation(1) + "QList<CT_AbstractOutModelCopyAction*> " + actionName + ";\n";
    result += "\n";

    int count = rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result += item->getCopyModelsDefinitions(actionName);
    }

    result += Tools::getIndentation(1) + "// Add IN model copy (eventually modified) to OUT results\n";
    result += Tools::getIndentation(1) + "addOutResultModelCopy(" + getModelName() + ", actions_" + getModelName() + ");\n";
    result += "\n";

    return result;
}

QString COPYResultModel::getCopyComputeLoops(int nbIndent, QString resultName)
{
    QString result = "";
    AbstractCopyModel* group = (AbstractCopyModel*) child(0);

    if (group->getStatus() == AbstractCopyModel::S_Copy)
    {
        result += "\n";
        result += Tools::getIndentation(nbIndent) + "// ----------------------------------------------------------------------------\n";
        result += Tools::getIndentation(nbIndent) + "// Works on the result corresponding to " + getDef() + "\n";
        result += Tools::getIndentation(nbIndent) + "// Iterating on groups situated at the root of the result (corresponding to " + group->getDef() + ")\n";
        result += Tools::getIndentation(nbIndent) + "for ( CT_AbstractItemGroup *" + group->getName() + " = " + getName() + "->beginGroup(" + group->getModelName() + ")\n";
        result += Tools::getIndentation(nbIndent+1) + "; " + group->getName() + " != NULL  && !isStopped()\n";
        result += Tools::getIndentation(nbIndent+1) + "; " + group->getName() + " = " + getName() + "->nextGroup() )\n";
        result += Tools::getIndentation(nbIndent) + "{\n";

        result += group->getCopyComputeLoops(nbIndent + 1, getName());

        result += Tools::getIndentation(nbIndent) + "}\n";
    }

    return result;
}

QString COPYResultModel::getCopyModelDoc(int nbIndent)
{
    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    result += " * - CT_ResultGroup" + desc + "\\n\n";

    AbstractCopyModel* group = (AbstractCopyModel*) child(0);
    result += group->getCopyModelDoc(nbIndent + 1);
    result += " *\n";
    return result;
}

