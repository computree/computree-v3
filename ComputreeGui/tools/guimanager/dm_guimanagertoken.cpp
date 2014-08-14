#include "dm_guimanagertoken.h"

DM_GuiManagerToken::DM_GuiManagerToken(const DM_AbstractAsyncOperationOptions *options) :
    QObject()
{
    m_options = (DM_AbstractAsyncOperationOptions*)options;

    connect(m_options, SIGNAL(passed()), this, SIGNAL(passed()), Qt::DirectConnection);
}

DM_AbstractAsyncOperationOptions *DM_GuiManagerToken::options() const
{
    return m_options;
}
