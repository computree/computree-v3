#ifndef PB_ACTIONSELECTITEMDRAWABLEGV_H
#define PB_ACTIONSELECTITEMDRAWABLEGV_H

#include <QRect>

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

/**
 * @brief Action to select items, points, faces, edges, etc...
 *
 * You can use this action in your action if you want. Just call methods :
 *
 * init()
 * mousePressEvent()
 * etc.....
 *
 * in methods of your action.
 *
 * Dont forget to set the selection mode => setSelectionMode method
 */
class PB_ActionSelectItemDrawableGV : public CT_AbstractActionForGraphicsView
{
public:

//    enum SelectionDrawMode
//    {
//        SELECT_FAST_MODE = GraphicsViewInterface::FAST,         /* Set the graphics view to draw in fast mode and selects elements */
//        SELECT_NORMAL_MODE = GraphicsViewInterface::NORMAL,     /* Set the graphics view to draw in normal mode and selects elements */
//        SELECT_CURRENT_MODE = SELECT_NORMAL_MODE+10             /* Selects elements as they are in the graphical view */
//    };

    PB_ActionSelectItemDrawableGV();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    /**
     * @brief change the selection mode for this action. The mode will be set to view
     *        when the mousePressEvent() method is called
     * @return false if we are between mousePressEvent() and mouseReleaseEvent()
     */
    bool setSelectionMode(GraphicsViewInterface::SelectionMode mode);

//    /**
//     * @brief change the draw mode for this action. (SELECT_CURRENT_MODE by default)
//     */
//    bool setDrawMode(PB_ActionSelectItemDrawableGV::SelectionDrawMode mode);


    GraphicsViewInterface::SelectionMode selectionMode() const;
//    PB_ActionSelectItemDrawableGV::SelectionDrawMode drawMode() const;

private:
    QRect                                   m_selectionRectangle;
    int                                     m_status;
    GraphicsViewInterface::SelectionMode    m_selectionMode;
    bool                                    m_mousePressed;

//    SelectionDrawMode                       m_drawMode;
//    GraphicsViewInterface::DrawMode         m_backupDrawMode;

//    void setNewDrawMode();
//    void setBackupDrawMode();

    GraphicsViewInterface::SelectionMode selectionModeToBasic(GraphicsViewInterface::SelectionMode mode) const;
};

#endif // PB_ACTIONSELECTITEMDRAWABLEGV_H
