#include "copyitemmodel.h"
#include "widgets/copyitemwidget.h"
#include "tools.h"
#include "assert.h"

COPYItemModel::COPYItemModel() : AbstractCopyModel()
{
    _widget = new COPYItemWidget(this);
    _status = AbstractCopyModel::S_Added;
    setData(QVariant(QColor(Qt::blue)),Qt::ForegroundRole);
    setText(getName());
}


void COPYItemModel::init(QString itemType, QString alias, QString name, QString desc)
{
    ((COPYItemWidget*)_widget)->init(itemType, alias, name, desc);
    _status = AbstractCopyModel::S_Copy;
    setData(QVariant(QColor(Qt::black)),Qt::ForegroundRole);
    setText(getName() + " (copie)");
}

void COPYItemModel::init(INItemModel *inModel)
{
    init(inModel->getItemType(), inModel->getAlias(), inModel->getDisplayableName(), inModel->getDescription());
}


QString COPYItemModel::getName()
{
    if (_status == AbstractCopyModel::S_Added)
    {
        return QString("itemOut_%1").arg(getAlias());
    } else {
        return QString("itemIn_%1").arg(getAlias());
    }
}

QString COPYItemModel::getModelName()
{
    return QString("itemCopyModel_%1").arg(getAlias());
}

QString COPYItemModel::getItemType()
{
    return ((COPYItemWidget*) _widget)->getItemType();
}

void COPYItemModel::getActionsIncludes(QSet<QString> &list)
{
    if (_status == AbstractCopyModel::S_Added)
    {
        list.insert("#include \"ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h\"");
    } else if (_status == AbstractCopyModel::S_DeletedCopy)
    {
        list.insert("#include \"ct_tools/model/ct_outmodelcopyactionremovemodelitemingroup.h\"");
    }
}

QString COPYItemModel::getCopyModelsDefinitions(QString actionName)
{
    QString result = "";
    assert(parent()!=NULL);

    if (getStatus() == AbstractCopyModel::S_Added)
    {
        // Création du modèle d'item
        result += "\n";
        result += Tools::getIndentation(1) + "// Create the model for " + getAutoRenameName() + "\n";

        QString resultTmp = "";

        resultTmp += Tools::getIndentation(1);
        resultTmp += "CT_OutStandardItemDrawableModel *";
        resultTmp += getModelName();
        resultTmp += " = new CT_OutStandardItemDrawableModel(";

        int indentSize = resultTmp.size();

        result += resultTmp + getDef();

        // Item Type
        result += ", \n";
        result += Tools::getSpaceSequence(indentSize) + "new ";
        result += ((COPYItemWidget*) _widget)->getItemType();
        result += "()";

        resultTmp = "";

        // Description
        QString description = ((COPYItemWidget*) _widget)->getDescription();
        if ((description.size() > 0) || (resultTmp.size() > 0))
        {
            resultTmp.insert(0, QString("tr(\"%1\")").arg(description));
            resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp.insert(0, ", \n");
        }

        // Displayable Name
        QString dispName = ((COPYItemWidget*) _widget)->getDisplayableName();
        if ((dispName.size() > 0) || (resultTmp.size() > 0))
        {
            resultTmp.insert(0, QString("tr(\"%1\")").arg(dispName));
            resultTmp.insert(0, Tools::getSpaceSequence(indentSize));
            resultTmp.insert(0, ", \n");
        }

        result += resultTmp;
        result += ");";
        result += "\n";

        // Action d'ajout
        result += "\n";
        result += Tools::getIndentation(1) + "// Create the action to add the item corresponding to " + getAutoRenameName() + "\n";
        QString str = Tools::getIndentation(1) + actionName + " << new CT_OutModelCopyActionAddModelItemInGroup(";
        result += str + ((AbstractCopyModel*) parent())->getDef() + ", \n";
        result += Tools::getSpaceSequence(str.length()) + getModelName() + ", \n";
        result += Tools::getSpaceSequence(str.length()) + getAutoRenameName() + ");\n";
        result += "\n";

    } else if (getStatus() == AbstractCopyModel::S_DeletedCopy)
    {
        // Action de suppression
        result += Tools::getIndentation(1) + "// Create the action to delete the item corresponding to " + getDef() + "\n";
        result += Tools::getIndentation(1) + actionName + " << new CT_OutModelCopyActionRemoveModelItemInGroup(";
        result += getDef() + ");\n";
        result += "\n";
    }

    return result;
}

QString COPYItemModel::getCopyComputeLoops(int nbIndent, QString resultName)
{
    return "result""";
}

QString COPYItemModel::getCopyModelDoc(int nbIndent)
{
    if (_status == AbstractCopyModel::S_DeletedCopy) {return "";}

    QString result = "";
    QString desc = "";
    if (getDisplayableName().length()>0) {desc = " (" + getDisplayableName() + ")";}
    else {desc = "";}    

    QString sign = "";
    if (_status == AbstractCopyModel::S_Added) {sign = "+";}
    result += " * " + Tools::getIndentation(nbIndent) + "- <em>cpy" + sign + " " + getItemType() + desc + "</em>\\n\n";

    return result;
}

