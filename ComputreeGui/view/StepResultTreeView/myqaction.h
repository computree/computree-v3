#ifndef MYQACTION_H
#define MYQACTION_H

#include <QAction>

class CT_VirtualAbstractStep;

class MyQAction : public QAction
{
public:
    MyQAction(CT_VirtualAbstractStep *step, QObject *parent) : QAction(parent) { _step = step; }
    MyQAction(CT_VirtualAbstractStep *step, const QString &text, QObject *parent) : QAction(text, parent) { _step = step; }
    MyQAction(CT_VirtualAbstractStep *step, const QIcon &icon, const QString &text, QObject *parent) : QAction(icon, text, parent) { _step = step; }

    CT_VirtualAbstractStep* step() { return _step; }

private:

    CT_VirtualAbstractStep    *_step;
};

#endif // MYQACTION_H
