#ifndef MYQACTION_H
#define MYQACTION_H

#include <QAction>

class Step;

class MyQAction : public QAction
{
public:
    MyQAction(Step *step, QObject *parent) : QAction(parent) { _step = step; }
    MyQAction(Step *step, const QString &text, QObject *parent) : QAction(text, parent) { _step = step; }
    MyQAction(Step *step, const QIcon &icon, const QString &text, QObject *parent) : QAction(icon, text, parent) { _step = step; }

    Step* step() { return _step; }

private:

    Step    *_step;
};

#endif // MYQACTION_H
