#ifndef CG_CUSTOMTREEITEM_H
#define CG_CUSTOMTREEITEM_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QHash>

class CG_CustomTreeItemModel;

class CG_CustomTreeItem : public QObject
{
    Q_OBJECT

public:
    explicit CG_CustomTreeItem();
    CG_CustomTreeItem(const QString &text);
    ~CG_CustomTreeItem();

    void appendRow(const QList<CG_CustomTreeItem *> &items);
    void removeRow(int r);
    void insertRow(int i, const QList<CG_CustomTreeItem *> &items);
    void replaceRow(int r, const QList<CG_CustomTreeItem *> &items);

    int rowCount() const;
    int nChildrens() const;
    bool hasChildren() const;

    int row() const;
    int column() const;

    CG_CustomTreeItem* parent() const;
    CG_CustomTreeItem* child(int row, int column = 0);

    QVariant data(int role = Qt::UserRole + 1) const;
    void setData(const QVariant &value, int role = Qt::UserRole + 1);

    void setText(const QString &text);

    void setFlags(Qt::ItemFlags flags);
    Qt::ItemFlags flags() const;

    void setSelectable(bool e);
    void setCheckable(bool e);
    void setTristate(bool e);
    void setDragEnabled(bool e);
    void setDropEnabled(bool e);
    void setEditable(bool e);
    void setEnabled(bool e);

    bool isCheckable() const;
    void setCheckState(Qt::CheckState c);
    Qt::CheckState checkState() const;

    void setFetchSize(int n);
    int fetchSize() const;

    bool canFetchMore() const;
    void fetchMore();

    void clear();

public slots:
    void setBoolData(bool e, Qt::ItemDataRole role = Qt::CheckStateRole);

private:
    QList< QList<CG_CustomTreeItem *> > m_items;
    int                                 m_count;
    CG_CustomTreeItem                   *m_parent;
    CG_CustomTreeItemModel              *m_model;
    int                                 m_column;
    QHash<int, QVariant>                m_datas;
    int                                 m_fetchSize;

    void changeFlags(bool enable, Qt::ItemFlags f);
    void setDataWithoutSignal(const QVariant &value, int role = Qt::UserRole + 1);

protected:

    friend class CG_CustomTreeItemModel;

    void setParent(const CG_CustomTreeItem *p);
    void setColumn(int c);
    void setModel(const CG_CustomTreeItemModel *m);

    CG_CustomTreeItemModel* model() const;

signals:
    void dataChanged(CG_CustomTreeItem *item, int role, QVariant value);
    void checkStateChanged(Qt::CheckState state);
};

#endif // CG_CUSTOMTREEITEM_H
