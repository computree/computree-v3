#ifndef CG_CUSTOMTREEITEM_H
#define CG_CUSTOMTREEITEM_H

#include <QString>
#include <QList>
#include <QVariant>

class CG_CustomTreeItemModel;

class CG_CustomTreeItem
{
public:
    explicit CG_CustomTreeItem();
    CG_CustomTreeItem(const QString &text);
    ~CG_CustomTreeItem();

    void appendRow(const QList<CG_CustomTreeItem *> &items);
    void removeRow(int row);
    void insertRow(int i, const QList<CG_CustomTreeItem *> &items);

    int rowCount() const;
    int columnCount() const;

    int row() const;
    int column() const;

    CG_CustomTreeItem* parent() const;
    CG_CustomTreeItem* child(int row, int column = 0);

    QVariant data(int role = Qt::UserRole + 1) const;
    void setData(const QVariant &value, int role = Qt::UserRole + 1);

    void setText(const QString &text);

    bool canFetchMore() const;
    void fetchMore();

    void clear();

private:
    QList< QList<CG_CustomTreeItem *> > m_items;
    int                                 m_count;
    CG_CustomTreeItem                   *m_parent;
    CG_CustomTreeItemModel              *m_model;
    int                                 m_column;

protected:

    friend class CG_CustomTreeItemModel;

    void setParent(const CG_CustomTreeItem *p);
    void setColumn(int c);
    void setModel(const CG_CustomTreeItemModel *m);

    CG_CustomTreeItemModel* model() const;
};

#endif // CG_CUSTOMTREEITEM_H
