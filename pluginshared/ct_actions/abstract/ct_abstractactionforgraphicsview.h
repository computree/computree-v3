#ifndef CT_ABSTRACTACTIONFORGRAPHICSVIEW_H
#define CT_ABSTRACTACTIONFORGRAPHICSVIEW_H

#include "ct_actions/abstract/ct_abstractaction.h"

#include <QPainter>

/**
 * @brief Represent an action for a 3D graphics view
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractActionForGraphicsView : public CT_AbstractAction
{
public:
    CT_AbstractActionForGraphicsView();

    /**
     * @brief Define the graphics view where occurs event. (A document can have multiple graphics view)
     *        Your action is copied and each copy is set to a GraphicsView
     */
    void setGraphicsView(const GraphicsViewInterface *view);

    /**
     * @brief Return the graphics view of the action
     */
    GraphicsViewInterface* graphicsView() const;

    /**
     * @brief This method is called when the action is active and when the graphics view is updated. You can
     *        overwrite them if you want to draw some ItemDrawable without add them in the document or
     *        other 3D elements.
     *
     *        Attention : elements draw with this methods can't be selected
     */
    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter) { Q_UNUSED(view) Q_UNUSED(painter) }

    /**
     * @brief This method is called (always after draw(...)) when the action is active and when the graphics
     *        view is updated. You can overwrite them if you want to draw some 2D element.
     *
     *        Attention : elements draw with this methods can't be selected
     */
    virtual void drawOverlay(GraphicsViewInterface &view, QPainter &painter) {  Q_UNUSED(view) Q_UNUSED(painter) }

protected:

    /**
     * @brief Called when the graphics view changed
     */
    virtual void graphicsViewChangedEvent() {}

private:
    GraphicsViewInterface   *m_gView;
};

#endif // CT_ABSTRACTACTIONFORGRAPHICSVIEW_H
