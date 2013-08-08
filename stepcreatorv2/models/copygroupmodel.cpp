#include "copygroupmodel.h"
#include "widgets/copygroupwidget.h"
#include "models/copyitemmodel.h"
#include "tools.h"
#include "assert.h"

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


void COPYGroupModel::getActionsIncludes(QSet<QString> &list)
{
    if (_status == AbstractCopyModel::S_Added)
    {
        list.insert("#include \"ct_tools/model/ct_outmodelcopyactionaddmodelgroupingroup.h\"");
    } else if (_status == AbstractCopyModel::S_DeletedCopy)
    {
        list.insert("#include \"ct_tools/model/ct_outmodelcopyactionremovemodelgroupingroup.h\"");
    }

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        item->getActionsIncludes(list);
    }
}

QString COPYGroupModel::getCopyModelsDefinitions(QString actionName)
{
    QString result = "";
    assert(parent()!=NULL);

    if (getStatus() == AbstractCopyModel::S_Added)
    {
        // Création du modèle de groupe
        result += "\n";
        result += Tools::getIndentation(1) + "// Create the model for " + getAutoRenameName() + "\n";
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

        // Action d'ajout
        result += "\n";
        result += Tools::getIndentation(1) + "// Create the action to add the group corresponding to " + getAutoRenameName() + "\n";
        QString str = Tools::getIndentation(1) + actionName + " << new CT_OutModelCopyActionAddModelGroupInGroup(";
        result += str + ((AbstractCopyModel*) parent())->getDef() + ", \n";
        result += Tools::getSpaceSequence(str.length()) + getModelName() + ", \n";
        result += Tools::getSpaceSequence(str.length()) + getAutoRenameName() + ");\n";
        result += "\n";
    }

    int count = rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result += item->getCopyModelsDefinitions(actionName);
    }

    if (getStatus() == AbstractCopyModel::S_DeletedCopy)
    {
        // Action de suppression
        result += Tools::getIndentation(1) + "// Create the action to delete the group corresponding to " + getDef() + "\n";
        result += Tools::getIndentation(1) + actionName + " << new CT_OutModelCopyActionRemoveModelGroupInGroup(";
        result += getDef() + ");\n";
        result += "\n";
    }

    return result;
}

QString COPYGroupModel::getCopyComputeLoops(int nbIndent, QString resultName = "")
{
    QString result = "";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* groupOrItem = (AbstractCopyModel*) child(i);

        if (groupOrItem->getStatus() == AbstractCopyModel::S_Copy)
        {
            if (groupOrItem->getModelType() == AbstractCopyModel::M_Group_COPY)
            {
                result += "\n";
                result += Tools::getIndentation(nbIndent) + "// Iterating on children groups corresponding to " + groupOrItem->getDef() + "\n";
                result += Tools::getIndentation(nbIndent) + "for ( CT_AbstractItemGroup *" + groupOrItem->getName();
                result += " = " + getName() + "->beginGroup(" + groupOrItem->getModelName() + ")\n";
                result += Tools::getIndentation(nbIndent+1) + "; " + groupOrItem->getName() + " != NULL  && !isStopped()\n";
                result += Tools::getIndentation(nbIndent+1) + "; " + groupOrItem->getName() + " = " + getName() + "->nextGroup() )\n";
                result += Tools::getIndentation(nbIndent) + "{\n";

                result += groupOrItem->getCopyComputeLoops(nbIndent+1, resultName);

                result += Tools::getIndentation(nbIndent) + "}\n";

            } else if (groupOrItem->getModelType() == AbstractCopyModel::M_Item_COPY)
            {
                QString type = ((COPYItemModel*) groupOrItem)->getItemType();
                result += Tools::getIndentation(nbIndent) + "// Gets the item corresponding to " + groupOrItem->getDef() + "\n";
                result += Tools::getIndentation(nbIndent) + "const " + type + "* " + groupOrItem->getName();
                result += " = (const " + type + "*) " + getName() + "->findFirstItem(" + groupOrItem->getModelName() + ");\n";

            }
        } else if (groupOrItem->getStatus() == AbstractCopyModel::S_Added)
        {
            if (groupOrItem->getModelType() == AbstractCopyModel::M_Group_COPY)
            {
                result += "\n";
                result += Tools::getIndentation(nbIndent+1) + "CT_StandardItemGroup* " + groupOrItem->getName();
                result += " = new CT_StandardItemGroup(" + groupOrItem->getModelName() + ", 0, " + resultName + ");\n";
                result += Tools::getIndentation(nbIndent+1) + getName() + ".addGroup(" + groupOrItem->getName() + ");\n";
                result += groupOrItem->getCopyComputeLoops(nbIndent+1, resultName);
            } else if (groupOrItem->getModelType() == AbstractCopyModel::M_Item_COPY)
            {
                result += "\n";
                result += Tools::getIndentation(nbIndent) + "// UNCOMMENT Following lines and complete parameters of the item's contructor\n";
                result += Tools::getIndentation(nbIndent) + "// " + ((COPYItemModel*)groupOrItem)->getItemType() + "* " + groupOrItem->getName();
                result += " = new " +  ((COPYItemModel*)groupOrItem)->getItemType() + "(" + groupOrItem->getModelName() + ", ID, " + resultName + ");\n";
                result += Tools::getIndentation(nbIndent+1) + "// " + getName() + ".addItem(" + groupOrItem->getName() + ");\n";
            }

            result += "\n";
        }

    }

    return result;
}

QString COPYGroupModel::getCopyModelDoc(int nbIndent)
{
    if (_status == AbstractCopyModel::S_DeletedCopy) {return "";}

    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}

    QString sign = "";
    if (_status == AbstractCopyModel::S_Added) {sign = "+";}
    result += " * " + Tools::getIndentation(nbIndent) + "- <em>cpy" + sign + " CT_StandardItemGroup" + desc + "...</em>\\n\n";

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        result += ((AbstractCopyModel*) child(i))->getCopyModelDoc(nbIndent + 1);
    }
    return result;
}
