#include "dm_actionshandlerfortreeview.h"

#include "ct_actions/abstract/ct_abstractactionfortreeview.h"

DM_ActionsHandlerForTreeView::DM_ActionsHandlerForTreeView(const CDM_ActionsManager &actionManager) : DM_ActionsHandler(actionManager)
{
}

bool DM_ActionsHandlerForTreeView::indexActivatedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QModelIndex&>(&CT_AbstractActionForTreeView::indexActivatedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexClickedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QModelIndex&>(&CT_AbstractActionForTreeView::indexClickedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexDoubleClickedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QModelIndex&>(&CT_AbstractActionForTreeView::indexDoubleClickedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexCollapsedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QModelIndex&>(&CT_AbstractActionForTreeView::indexCollapsedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexExpandedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QModelIndex&>(&CT_AbstractActionForTreeView::indexExpandedEvent, index);
}

bool DM_ActionsHandlerForTreeView::itemDragEnterEvent(QDragEnterEvent *event)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, QDragEnterEvent*>(&CT_AbstractActionForTreeView::itemDragEnterEvent, event);
}

bool DM_ActionsHandlerForTreeView::itemDragMoveEvent(QDragMoveEvent *event)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, QDragMoveEvent*>(&CT_AbstractActionForTreeView::itemDragMoveEvent, event);
}

bool DM_ActionsHandlerForTreeView::itemDragLeaveEvent(QDragLeaveEvent *event)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, QDragLeaveEvent*>(&CT_AbstractActionForTreeView::itemDragLeaveEvent, event);
}

bool DM_ActionsHandlerForTreeView::itemDropEvent(QDropEvent *event)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, QDropEvent*>(&CT_AbstractActionForTreeView::itemDropEvent, event);
}

bool DM_ActionsHandlerForTreeView::showContextMenu(const QPoint &point)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QPoint&>(&CT_AbstractActionForTreeView::showContextMenu, point);
}

bool DM_ActionsHandlerForTreeView::showContextMenuOnHorizontalHeader(const QPoint &point)
{
    return callEventDynamicCastT<CT_AbstractActionForTreeView, const QPoint&>(&CT_AbstractActionForTreeView::showContextMenuOnHorizontalHeader, point);
}
