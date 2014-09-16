#include "cg_customtreeitemmodel.h"

CG_CustomTreeItemModel::CG_CustomTreeItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_root = new CG_CustomTreeItem();
    m_root->setModel(this);
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

QVariant CG_CustomTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    CG_CustomTreeItem *item = static_cast<CG_CustomTreeItem*>(index.internalPointer());

    return item->data(role);
}

Qt::ItemFlags CG_CustomTreeItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
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

    if (parentItem == invisibleRootItem())
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
    if (parent.isValid())
        return static_cast<CG_CustomTreeItem*>(parent.internalPointer())->columnCount();

    return invisibleRootItem()->columnCount();
}

CG_CustomTreeItem *CG_CustomTreeItemModel::itemFromIndex(const QModelIndex &index) const
{
    if(!index.isValid())
        return NULL;

    return static_cast<CG_CustomTreeItem*>(index.internalPointer());
}

QModelIndex CG_CustomTreeItemModel::indexFromItem(const CG_CustomTreeItem *item) const
{
    if (item && item->parent()) {
        return createIndex(item->row(), item->column(), item->parent());
    }

    return QModelIndex();
}

void CG_CustomTreeItemModel::setHorizontalHeaderLabels(const QStringList &headers)
{
}

void CG_CustomTreeItemModel::clear()
{
    invisibleRootItem()->clear();
}

bool CG_CustomTreeItemModel::canFetchMore(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return false;

    return static_cast<CG_CustomTreeItem*>(parent.internalPointer())->canFetchMore();
}

void CG_CustomTreeItemModel::fetchMore(const QModelIndex &parent)
{
    static_cast<CG_CustomTreeItem*>(parent.internalPointer())->fetchMore();
}
