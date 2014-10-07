#ifndef MYQMENU_H
#define MYQMENU_H

#include <QMenu>
#include <QKeyEvent>

class MyQMenu : public QMenu
{

public:
    MyQMenu(QWidget *parent = 0) : QMenu(parent) {}
    MyQMenu(const QString &title, QWidget *parent = 0) : QMenu(title, parent) {}

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYQMENU_H
