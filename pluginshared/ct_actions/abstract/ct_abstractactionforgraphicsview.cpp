#include "ct_abstractactionforgraphicsview.h"

#include "ct_view/actions/abstract/ct_gabstractactionforgraphicsviewoptions.h"

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

void CT_AbstractActionForGraphicsView::registerOption(CT_GAbstractActionOptions *options)
{
    CT_GAbstractActionForGraphicsViewOptions *gvOptions = dynamic_cast<CT_GAbstractActionForGraphicsViewOptions*>(options);

    if(gvOptions != NULL)
        connect(gvOptions, SIGNAL(drawing3DChanged()), this, SLOT(setDrawing3DChanged()));

    CT_AbstractAction::registerOption(options);
}

void CT_AbstractActionForGraphicsView::setDrawing3DChanged()
{
    emit drawing3DChanged();
}
