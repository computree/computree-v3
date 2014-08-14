#ifndef PB_ACTIONSHOWITEMDATAGV_H
#define PB_ACTIONSHOWITEMDATAGV_H

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_itemdrawable/tools/ct_standarditemdatavalue.h"
#include "actions/pb_actionselectitemdrawablegv.h"

#include <QVector3D>

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

    ActionInterface* copy() const;

private:
    bool                            m_use;
    QPoint                          m_mousePoint;
    QVector3D                       m_3dPoint;
    CT_StandardItemDataValue        m_value;
    PB_ActionSelectItemDrawableGV   *m_selectAction;
};

#endif // PB_ACTIONSHOWITEMDATAGV_H
