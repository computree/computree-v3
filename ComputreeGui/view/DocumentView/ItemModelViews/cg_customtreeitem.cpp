#include "cg_customtreeitem.h"

#include "view/DocumentView/ItemModelViews/cg_customtreeitemmodel.h"
#include "dm_guimanager.h"

CG_CustomTreeItem::CG_CustomTreeItem()
{
    m_count = 0;
    m_parent = NULL;
    m_column = 0;
    m_model = NULL;
    m_fetchSize = 30;

    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
             |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}

CG_CustomTreeItem::CG_CustomTreeItem(const QString &text)
{
    m_count = 0;
    m_parent = NULL;
    m_column = 0;
    m_model = NULL;
    m_fetchSize = 30;

    setText(text);
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
             |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
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

bool CG_CustomTreeItem::hasChildren() const
{
    return !m_items.isEmpty();
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
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    return m_datas.value(role, QVariant());
}

void CG_CustomTreeItem::setData(const QVariant &value, int role)
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    setDataWithoutSignal(value, role);

    emit dataChanged(this, role, value);

    if(role == Qt::CheckStateRole)
        emit checkStateChanged(checkState());
}

void CG_CustomTreeItem::setText(const QString &text)
{
    setDataWithoutSignal(text, Qt::DisplayRole);
}

void CG_CustomTreeItem::setFlags(Qt::ItemFlags flags)
{
    setDataWithoutSignal((int)flags, Qt::UserRole - 1);
}

Qt::ItemFlags CG_CustomTreeItem::flags() const
{
    QVariant v = data(Qt::UserRole - 1);

    if (!v.isValid())
        return (Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable
                |Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);

    return Qt::ItemFlags(v.toInt());
}

void CG_CustomTreeItem::setSelectable(bool e)
{
    changeFlags(e, Qt::ItemIsSelectable);
}

void CG_CustomTreeItem::setCheckable(bool e)
{
    if (e && !isCheckable()) {
        // make sure there's data for the checkstate role
        if (!data(Qt::CheckStateRole).isValid())
            setDataWithoutSignal(Qt::Unchecked, Qt::CheckStateRole);
    }

    changeFlags(e, Qt::ItemIsUserCheckable);
}

void CG_CustomTreeItem::setTristate(bool e)
{
    changeFlags(e, Qt::ItemIsTristate);
}

void CG_CustomTreeItem::setDragEnabled(bool e)
{
    changeFlags(e, Qt::ItemIsDragEnabled);
}

void CG_CustomTreeItem::setDropEnabled(bool e)
{
    changeFlags(e, Qt::ItemIsDropEnabled);
}

void CG_CustomTreeItem::setEditable(bool e)
{
    changeFlags(e, Qt::ItemIsEditable);
}

void CG_CustomTreeItem::setEnabled(bool e)
{
    changeFlags(e, Qt::ItemIsEnabled);
}

bool CG_CustomTreeItem::isCheckable() const
{
    return (flags() & Qt::ItemIsUserCheckable) != 0;
}

void CG_CustomTreeItem::setCheckState(Qt::CheckState c)
{
    setDataWithoutSignal(c, Qt::CheckStateRole);
}

Qt::CheckState CG_CustomTreeItem::checkState() const
{
    return Qt::CheckState(qvariant_cast<int>(data(Qt::CheckStateRole)));
}

void CG_CustomTreeItem::setFetchSize(int n)
{
    if(n > 0)
        m_fetchSize = n;
}

int CG_CustomTreeItem::fetchSize() const
{
    return m_fetchSize;
}

int CG_CustomTreeItem::rowCount() const
{
    return m_count;
}

int CG_CustomTreeItem::nChildrens() const
{
    return m_items.size();
}

bool CG_CustomTreeItem::canFetchMore() const
{
    return m_count < m_items.size();
}

void CG_CustomTreeItem::fetchMore()
{
    int remainder = m_items.size() - m_count;
    int itemsToFetch = qMin(m_fetchSize, remainder);

    if(model() != NULL)
        model()->beginInsertRows(model()->indexFromItem(this), m_count, m_count + itemsToFetch - 1);

    m_count += itemsToFetch;

    if(model() != NULL)
        model()->endInsertRows();
}

void CG_CustomTreeItem::clear()
{
    if(!m_items.isEmpty())
    {
        int n = rowCount();

        if((model() != NULL) && (n > 0))
            model()->beginRemoveRows(model()->indexFromItem(this), 0, n-1);

        foreach (const QList<CG_CustomTreeItem*> &l, m_items) {
            qDeleteAll(l.begin(), l.end());
        }

        m_items.clear();
        m_count = 0;

        if((model() != NULL) && (n > 0))
            model()->endRemoveRows();
    }
}

void CG_CustomTreeItem::setBoolData(bool e, Qt::ItemDataRole role)
{
    setDataWithoutSignal(e ? Qt::Checked : Qt::Unchecked, role);
}

void CG_CustomTreeItem::changeFlags(bool enable, Qt::ItemFlags f)
{
    Qt::ItemFlags fl = flags();

    if (enable)
        fl |= f;
    else
        fl &= ~f;

    setFlags(fl);
}

void CG_CustomTreeItem::setDataWithoutSignal(const QVariant &value, int role)
{
    role = (role == Qt::EditRole) ? Qt::DisplayRole : role;

    m_datas.insert(role, value);

    if(model() != NULL)
        model()->dataOfCustomItemChanged(this);
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
