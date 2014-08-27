#include "dm_actionsmanager.h"

#include "ct_actions/abstract/ct_abstractaction.h"

DM_ActionsManager::DM_ActionsManager() : CDM_ActionsManager()
{
    m_docManager = NULL;

    connect(this, SIGNAL(actionToRemove(CT_AbstractAction*)), this, SLOT(slotActionToBeRemoved(CT_AbstractAction*)), Qt::DirectConnection);
}

void DM_ActionsManager::setDocumentManagerView(const DM_DocumentManagerView *docManager)
{
    m_docManager = (DM_DocumentManagerView*)docManager;
}

DM_DocumentManagerView *DM_ActionsManager::documentManagerView() const
{
    return m_docManager;
}

void DM_ActionsManager::slotActionToBeRemoved(CT_AbstractAction *action)
{
    if(m_docManager != NULL)
    {
        int size = m_docManager->nbDocumentView();

        for(int i=0; i<size; ++i)
            m_docManager->getDocumentView(i)->removeActions(action->uniqueName());
    }
}
