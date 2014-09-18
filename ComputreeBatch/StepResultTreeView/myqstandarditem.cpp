#include "myqstandarditem.h"

MyQStandardItem::MyQStandardItem(CT_VirtualAbstractStep *step,
                                CT_AbstractResult *res,
                                MyQStandardItem::ColumnType type,
                                const QString &text) : QStandardItem(text)
{
    _step = step;
    _result = res;
    _columnType = type;

    setData(type, ColumnTypeRole);
}

MyQStandardItem::MyQStandardItem(CT_VirtualAbstractStep *step,
                                CT_AbstractResult *res,
                                MyQStandardItem::ColumnType type,
                                const QVariant &value)
{
    _step = step;
    _result = res;
    _columnType = type;
    setData(value, Qt::DisplayRole);

    setData(type, ColumnTypeRole);
}

void MyQStandardItem::setData(const QVariant &value, int role)
{
    QStandardItem::setData(value, role);

    if((role == Qt::DisplayRole)
        || (role == Qt::EditRole)
        || (role == Qt::CheckStateRole)
        || (role == InvisibleBoolTypeRole))
    {
        emit dataChanged(this);

        if(role == Qt::CheckStateRole)
        {
            emit checkStateChanged(checkState());
        }
    }
}

void MyQStandardItem::setIntData(int data, Qt::ItemDataRole role)
{
    setData(data, role);
}

void MyQStandardItem::setFloatData(float data, Qt::ItemDataRole role)
{
    setData(data, role);
}

void MyQStandardItem::setBoolData(bool data, Qt::ItemDataRole role)
{
    setCheckState(data ? Qt::Checked : Qt::Unchecked);
}

void MyQStandardItem::setBoolDataInvisible(bool data)
{
    setData(data, InvisibleBoolTypeRole);
}

void MyQStandardItem::setColorData(QColor data, Qt::ItemDataRole role)
{
    setData(data, role);
}

void MyQStandardItem::slotSetEnabled(bool enable)
{
    setEnabled(enable);
}
