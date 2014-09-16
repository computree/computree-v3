#include "cg_customtreeitem.h"

#include "view/DocumentView/ItemModelViews/cg_customtreeitemmodel.h"

CG_CustomTreeItem::CG_CustomTreeItem()
{
    m_count = 0;
    m_parent = NULL;
    m_column = 0;
    m_model = NULL;
}

CG_CustomTreeItem::CG_CustomTreeItem(const QString &text)
{
    m_count = 0;
    m_parent = NULL;
    m_column = 0;
    m_model = NULL;
}

CG_CustomTreeItem::~CG_CustomTreeItem()
{
    clear();
}

void CG_CustomTreeItem::appendRow(const QList<CG_CustomTreeItem *> &items)
{
    m_items.append(items);

    int c = 0;

    foreach (CG_CustomTreeItem *i, items) {
        i->setParent(this);
        i->setColumn(c);
        i->setModel(model());
        ++c;
    }
}

void CG_CustomTreeItem::removeRow(int row)
{
    if(model() != NULL)
        model()->beginRemoveRows(model()->indexFromItem(this), row, row+1);

    QList<CG_CustomTreeItem *> items = m_items.takeAt(row);

    foreach (CG_CustomTreeItem *i, items) {
        i->setModel(NULL);
        delete i;
    }

    if(model() != NULL)
        model()->endRemoveRows();
}

void CG_CustomTreeItem::insertRow(int i, const QList<CG_CustomTreeItem *> &items)
{
    if(model() != NULL)
        model()->beginInsertRows(model()->indexFromItem(this), i+1, i+2);

    m_items.insert(i, items);

    if(model() != NULL)
        model()->endInsertRows();
}

int CG_CustomTreeItem::columnCount() const
{
    if(m_items.isEmpty())
        return 0;

    return m_items.first().size();
}

int CG_CustomTreeItem::row() const
{
    if(parent() == NULL)
        return 0;

    int row = 0;

    foreach (const QList<CG_CustomTreeItem*> l, parent()->m_items) {
        if(l.at(column()) == this)
            return row;

        ++row;
    }

    return 0;
}

int CG_CustomTreeItem::column() const
{
    return m_column;
}

CG_CustomTreeItem* CG_CustomTreeItem::parent() const
{
    return m_parent;
}

CG_CustomTreeItem* CG_CustomTreeItem::child(int row, int column)
{
    return m_items.at(row).at(column);
}

QVariant CG_CustomTreeItem::data(int role) const
{
    return "data";
}

void CG_CustomTreeItem::setData(const QVariant &value, int role)
{

}

void CG_CustomTreeItem::setText(const QString &text)
{
    setData(text, Qt::DisplayRole);
}

int CG_CustomTreeItem::rowCount() const
{
    return m_count;
}

bool CG_CustomTreeItem::canFetchMore() const
{
    return m_count < m_items.size();
}

void CG_CustomTreeItem::fetchMore()
{
    int remainder = m_items.size() - m_count;
    int itemsToFetch = qMin(10, remainder);

    m_count += itemsToFetch;
}

void CG_CustomTreeItem::clear()
{
    if(model() != NULL)
        model()->beginRemoveRows(model()->indexFromItem(this), 0,rowCount()-1);

    foreach (const QList<CG_CustomTreeItem*> &l, m_items) {
        qDeleteAll(l.begin(), l.end());
    }

    m_items.clear();
    m_count = 0;

    if(model() != NULL)
        model()->endRemoveRows();
}

void CG_CustomTreeItem::setParent(const CG_CustomTreeItem *p)
{
    m_parent = (CG_CustomTreeItem*)p;
}

void CG_CustomTreeItem::setColumn(int c)
{
    m_column = c;
}

void CG_CustomTreeItem::setModel(const CG_CustomTreeItemModel *m)
{
    m_model = (CG_CustomTreeItemModel*)m;
}

CG_CustomTreeItemModel* CG_CustomTreeItem::model() const
{
    return m_model;
}
