#ifndef MYTREEDELEGATE_H
#define MYTREEDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class MyTreeDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    explicit MyTreeDelegate(QObject *parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

signals:

public slots:

};

#endif // GTREEDELEGATE_H
