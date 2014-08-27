#include "gitemmodelview.h"
#include "dm_guimanager.h"

#include "dm_actionshandler.h"

#include "ct_actions/abstract/ct_abstractactionfortreeview.h"

GItemModelView::GItemModelView()
{
    m_doc = NULL;
    m_syncGroup = NULL;
}

GItemModelView::~GItemModelView()
{
}

void GItemModelView::setDocument(const GDocumentView *doc)
{
    m_doc = (GDocumentView*)doc;
}

void GItemModelView::setCurrentAction(CT_AbstractActionForTreeView *action)
{
    if(action != NULL)
    {
        action->setDocument(document());
        action->setTreeView(this);
    }

    actionsHandler()->setCurrentAction(action);
}

void GItemModelView::setDefaultAction(CT_AbstractActionForTreeView *action)
{
    if(action != NULL)
    {
        action->setDocument(document());
        action->setTreeView(this);
    }

    actionsHandler()->setDefaultAction(action);
}

void GItemModelView::setSyncGroup(const DM_ItemModelViewSynchronizedGroup *group)
{
    m_syncGroup = (DM_ItemModelViewSynchronizedGroup*)group;
}

DM_ItemModelViewSynchronizedGroup* GItemModelView::syncGroup() const
{
    return m_syncGroup;
}

GDocumentView* GItemModelView::document() const
{
    return m_doc;
}
