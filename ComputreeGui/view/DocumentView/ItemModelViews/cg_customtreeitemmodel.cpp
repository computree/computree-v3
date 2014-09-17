#include "cg_customtreeitemmodel.h"

#include <QStringList>

CG_CustomTreeItemModel::CG_CustomTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_root = new CG_CustomTreeItem();
    m_root->setModel(this);
    m_columnCount = 0;
}

CG_CustomTreeItemModel::~CG_CustomTreeItemModel()
{
    m_root->setModel(NULL);
    delete m_root;
}

CG_CustomTreeItem* CG_CustomTreeItemModel::invisibleRootItem() const
{
    return m_root;
}

void CG_CustomTreeItemModel::appendRow(const QList<CG_CustomTreeItem *> &items)
{
    invisibleRootItem()->appendRow(items);
}

void CG_CustomTreeItemModel::finishAppendRows()
{
    if(invisibleRootItem()->canFetchMore())
        invisibleRootItem()->fetchMore();
}

bool CG_CustomTreeItemModel::hasChildren(const QModelIndex &parent) const
{
    CG_CustomTreeItem *item = itemFromIndex(parent);

    return item ? item->hasChildren() : false;
}

bool CG_CustomTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    itemFromIndex(index)->setData(value, role);
    return true;
}

QVariant CG_CustomTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CG_CustomTreeItem *parent = static_cast<CG_CustomTreeItem*>(index.internalPointer())->parent();

    CG_CustomTreeItem *item = parent;

    if(parent != NULL)
        item = parent->child(index.row(), index.column());

    return item->data(role);
}

Qt::ItemFlags CG_CustomTreeItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return itemFromIndex(index)->flags();
}

QModelIndex CG_CustomTreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CG_CustomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = invisibleRootItem();
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    CG_CustomTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex CG_CustomTreeItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CG_CustomTreeItem *childItem = static_cast<CG_CustomTreeItem*>(index.internalPointer());
    CG_CustomTreeItem *parentItem = childItem->parent();

    if (parentItem == invisibleRootItem() || (parentItem == NULL))
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CG_CustomTreeItemModel::rowCount(const QModelIndex &parent) const
{
    CG_CustomTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = static_cast<CG_CustomTreeItem*>(invisibleRootItem());
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    return parentItem->rowCount();
}

int CG_CustomTreeItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_columnCount;
}

CG_CustomTreeItem *CG_CustomTreeItemModel::itemFromIndex(const QModelIndex &index) const
{
    if(!index.isValid())
        return invisibleRootItem();

    return static_cast<CG_CustomTreeItem*>(index.internalPointer());
}

QModelIndex CG_CustomTreeItemModel::indexFromItem(const CG_CustomTreeItem *item) const
{
    if (item && item->parent()) {
        return createIndex(item->row(), item->column(), item->parent());
    }

    return QModelIndex();
}

QVariant CG_CustomTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    CG_CustomTreeItem *headerItem = 0;

    if (orientation == Qt::Horizontal)
        headerItem = m_columnHeaderItems.at(section);

    return (headerItem != NULL) ? headerItem->data(role) : QAbstractItemModel::headerData(section, orientation, role);
}

void CG_CustomTreeItemModel::setHorizontalHeaderLabels(const QStringList &headers)
{
    int s = headers.size();

    setColumnCount(s);

    for(int i=0; i<s; ++i)
    {
        CG_CustomTreeItem *item = horizontalHeaderItem(i);

        if(item == NULL)
        {
            item = new CG_CustomTreeItem();
            setHorizontalHeaderItem(i, item);
        }

        item->setText(headers.at(i));
    }
}

CG_CustomTreeItem* CG_CustomTreeItemModel::horizontalHeaderItem(int column) const
{
    if ((column < 0) || (column >= columnCount()))
        return NULL;

    return m_columnHeaderItems.at(column);
}

void CG_CustomTreeItemModel::setHorizontalHeaderItem(int column, CG_CustomTreeItem *item)
{
    if (column < 0)
        return;

    if (columnCount() <= column)
        setColumnCount(column + 1);

    CG_CustomTreeItem *oldItem = horizontalHeaderItem(column);

    if (item == oldItem)
        return;

    if (item != NULL) {
        if (item->model() == NULL) {
            item->setModel(this);
        }
    }

    if (oldItem != NULL)
        oldItem->setModel(NULL);
    delete oldItem;

    m_columnHeaderItems.replace(column, item);

    emit headerDataChanged(Qt::Horizontal, column, column);
}

void CG_CustomTreeItemModel::clear()
{
    invisibleRootItem()->clear();
}

bool CG_CustomTreeItemModel::canFetchMore(const QModelIndex &parent) const
{
    CG_CustomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = static_cast<CG_CustomTreeItem*>(invisibleRootItem());
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    return parentItem->canFetchMore();
}

void CG_CustomTreeItemModel::fetchMore(const QModelIndex &parent)
{
    CG_CustomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = static_cast<CG_CustomTreeItem*>(invisibleRootItem());
    else
        parentItem = static_cast<CG_CustomTreeItem*>(parent.internalPointer());

    return parentItem->fetchMore();
}

void CG_CustomTreeItemModel::setColumnCount(int c)
{
    m_columnCount = c;

    if(m_columnHeaderItems.size() < m_columnCount)
    {
        beginInsertColumns(QModelIndex(), m_columnHeaderItems.size(), m_columnCount - 1);

        while(m_columnHeaderItems.size() < m_columnCount)
            m_columnHeaderItems.append(NULL);

        endInsertColumns();
    }
    else if(m_columnHeaderItems.size() > m_columnCount)
    {
        beginRemoveColumns(QModelIndex(), m_columnCount, m_columnHeaderItems.size() - 1);

        while(m_columnHeaderItems.size() > m_columnCount)
        {
            CG_CustomTreeItem *item = m_columnHeaderItems.takeLast();
            item->setModel(NULL);
            delete item;
        }

        endRemoveColumns();
    }
}

void CG_CustomTreeItemModel::dataOfCustomItemChanged(CG_CustomTreeItem *item)
{
    emit dataChanged(indexFromItem(item), indexFromItem(item));
}
