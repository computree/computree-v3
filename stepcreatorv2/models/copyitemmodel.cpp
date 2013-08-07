#include "copyitemmodel.h"
#include "widgets/copyitemwidget.h"
#include "tools.h"

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


    return result;
}
