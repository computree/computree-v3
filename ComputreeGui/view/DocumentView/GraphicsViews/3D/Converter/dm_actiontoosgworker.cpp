#include "dm_actiontoosgworker.h"

DM_ActionToOsgWorker::DM_ActionToOsgWorker() : DM_AbstractWorker()
{
    m_action = NULL;
    m_gv = NULL;

    m_painter.setUseDisplayList(false);

    connect(this, SIGNAL(canceled()), this, SLOT(cancelEvent()), Qt::DirectConnection);
}

void DM_ActionToOsgWorker::setActionToConvert(CT_AbstractActionForGraphicsView *action)
{
    m_action = action;
}

void DM_ActionToOsgWorker::setGraphicsView(GraphicsViewInterface *gv)
{
    m_gv = gv;
}

DM_PainterToOsgElementsResult DM_ActionToOsgWorker::results() const
{
    return m_painter.results();
}

void DM_ActionToOsgWorker::apply()
{
    setProgress(0);

    if((m_action != NULL) && (m_gv != NULL)){

        m_action->draw(*m_gv, m_painter);

        m_painter.finish();
    }

    setProgress(100);

    setFinished();
}

void DM_ActionToOsgWorker::cancelEvent()
{
    m_painter.cancel();
}
