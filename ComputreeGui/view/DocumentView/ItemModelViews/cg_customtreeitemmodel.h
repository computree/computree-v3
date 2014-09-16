#ifndef CG_CUSTOMTREEITEMMODEL_H
#define CG_CUSTOMTREEITEMMODEL_H

#include <QAbstractItemModel>
#include "view/DocumentView/ItemModelViews/cg_customtreeitem.h"

class CG_CustomTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CG_CustomTreeItemModel(QObject *parent = 0);
    ~CG_CustomTreeItemModel();

    CG_CustomTreeItem* invisibleRootItem() const;

    void appendRow(const QList<CG_CustomTreeItem *> &items);
    void finishAppendRows();

    bool hasChildren(const QModelIndex & parent = QModelIndex()) const;

    bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    CG_CustomTreeItem* itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(const CG_CustomTreeItem *item) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setHorizontalHeaderLabels(const QStringList &headers);

    CG_CustomTreeItem* horizontalHeaderItem(int column) const;
    void setHorizontalHeaderItem(int column, CG_CustomTreeItem *item);

    void clear();

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

protected:

    friend class CG_CustomTreeItem;

    void setColumnCount(int c);
    void dataOfCustomItemChanged(CG_CustomTreeItem *item);

private:
    CG_CustomTreeItem           *m_root;
    int                         m_columnCount;
    QList<CG_CustomTreeItem*>   m_columnHeaderItems;
};

#endif // CG_CUSTOMTREEITEMMODEL_H
