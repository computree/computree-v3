#ifndef PB_ACTIONPICKITEMSINLIST_H
#define PB_ACTIONPICKITEMSINLIST_H


#include "views/actions/pb_actionpickitemsinlistoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include <QRect>

class PB_ActionPickItemsInList : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionPickItemsInList(const QList<CT_AbstractItemDrawable*> &itemList, QList<CT_AbstractItemDrawable*> *selectedItems, float maxDist = 1);

    ~PB_ActionPickItemsInList();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    CT_AbstractItemDrawable* addItemToSelection(const QPoint &point);
    CT_AbstractItemDrawable* removeItemFromSelection(const QPoint &point);

public slots:
    void redraw();
    void selectAll();
    void selectNone();

private:
    float       _maxDist;
    QPoint      _oldPos;
    Qt::MouseButtons  _buttonsPressed;

    QList<CT_AbstractItemDrawable*> _itemList;
    QList<CT_AbstractItemDrawable*> *_selectedItems;
};


#endif // PB_ACTIONPICKITEMSINLIST_H
