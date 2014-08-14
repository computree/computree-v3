#include "dm_asyncoperation.h"

DM_AsyncOperation::DM_AsyncOperation()
{
    m_progressDialog = NULL;
}

void DM_AsyncOperation::setProgressDialog(const IMainProgressDialog *dialog)
{
    if(m_progressDialog != NULL)
        disconnect(m_progressDialog, NULL, this, NULL);

    m_progressDialog = (IMainProgressDialog*)dialog;

    if(m_progressDialog != NULL)
        connect(m_progressDialog, SIGNAL(cancel()), this, SIGNAL(cancel()), Qt::QueuedConnection);
}

IMainProgressDialog* DM_AsyncOperation::progressDialog() const
{
    return m_progressDialog;
}

void DM_AsyncOperation::setProgress(const int &progress)
{
    m_progressDialog->setValue(progress);
}
