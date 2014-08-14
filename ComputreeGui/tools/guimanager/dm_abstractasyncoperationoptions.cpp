#include "dm_abstractasyncoperationoptions.h"

#include "dm_guimanager.h"

DM_AbstractAsyncOperationOptions::DM_AbstractAsyncOperationOptions()
{
    connect(GUI_MANAGER, SIGNAL(currentAsyncOperationReleased()), this, SLOT(slotTestPass()), Qt::QueuedConnection);
    connect(GUI_MANAGER->getStepManager(), SIGNAL(completed()), this, SLOT(slotTestPass()), Qt::QueuedConnection);

    setWaitUntilAsyncOperationReleased(true);
    setWaitUntilStepManagerIsFinished(false);
}

void DM_AbstractAsyncOperationOptions::setWaitUntilAsyncOperationReleased(bool enable)
{
    m_waitUntilAsyncOperationReleased = enable;
}

void DM_AbstractAsyncOperationOptions::setWaitUntilStepManagerIsFinished(bool enable)
{
    m_waitUntilStepManagerIsFinished = enable;
}

bool DM_AbstractAsyncOperationOptions::pass() const
{
    return (!m_waitUntilAsyncOperationReleased || !GUI_MANAGER->isAsyncOperationInProgress())
            && (!m_waitUntilStepManagerIsFinished || !GUI_MANAGER->getStepManager()->isRunning());
}

void DM_AbstractAsyncOperationOptions::slotTestPass()
{
    if(pass())
        emit passed();
}
