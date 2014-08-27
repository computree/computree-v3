#ifndef DM_ACTIONSMANAGER_H
#define DM_ACTIONSMANAGER_H

#include "cdm_actionsmanager.h"
#include "dm_documentmanagerview.h"

class DM_ActionsManager : public CDM_ActionsManager
{
    Q_OBJECT

public:
    DM_ActionsManager();

    void setDocumentManagerView(const DM_DocumentManagerView *docManager);
    DM_DocumentManagerView* documentManagerView() const;

private:
    DM_DocumentManagerView      *m_docManager;      /*!< doc manager */

private slots:

    void slotActionToBeRemoved(CT_AbstractAction *action);
};

#endif // DM_ACTIONSMANAGER_H
