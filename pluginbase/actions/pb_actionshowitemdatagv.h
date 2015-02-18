#ifndef PB_ACTIONSHOWITEMDATAGV_H
#define PB_ACTIONSHOWITEMDATAGV_H

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "actions/pb_actionselectitemdrawablegv.h"

class PB_ActionShowItemDataGV : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT

public:
    PB_ActionShowItemDataGV();
    ~PB_ActionShowItemDataGV();

    void init();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);

    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

private:
    bool                            m_use;
    QPoint                          m_mousePoint;
    PB_ActionSelectItemDrawableGV   *m_selectAction;
};

#endif // PB_ACTIONSHOWITEMDATAGV_H
