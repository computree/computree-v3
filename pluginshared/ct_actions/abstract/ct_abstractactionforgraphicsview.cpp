#include "ct_abstractactionforgraphicsview.h"

CT_AbstractActionForGraphicsView::CT_AbstractActionForGraphicsView() : CT_AbstractAction()
{
    m_gView = NULL;
}

void CT_AbstractActionForGraphicsView::setGraphicsView(const GraphicsViewInterface *view)
{
    if(m_gView != view)
    {
        m_gView = (GraphicsViewInterface*)view;

        graphicsViewChangedEvent();
    }
}

GraphicsViewInterface* CT_AbstractActionForGraphicsView::graphicsView() const
{
    return m_gView;
}
