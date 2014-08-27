#ifndef ITEMDRAWABLETEST_H
#define ITEMDRAWABLETEST_H

#include <QtTest>

class ItemDrawableTest : public QObject
{
    Q_OBJECT
public:
    explicit ItemDrawableTest(QObject *parent = 0);

private slots:
    void testTNodeGroup();
};

#endif // ITEMDRAWABLETEST_H
