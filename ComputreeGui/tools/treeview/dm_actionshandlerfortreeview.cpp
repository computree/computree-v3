#include "dm_actionshandlerfortreeview.h"

DM_ActionsHandlerForTreeView::DM_ActionsHandlerForTreeView(const CDM_ActionsManager &actionManager) : DM_ActionsHandler(actionManager)
{
}

bool DM_ActionsHandlerForTreeView::indexActivatedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QModelIndex&>(&ActionForTreeViewInterface::indexActivatedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexClickedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QModelIndex&>(&ActionForTreeViewInterface::indexClickedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexDoubleClickedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QModelIndex&>(&ActionForTreeViewInterface::indexDoubleClickedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexCollapsedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QModelIndex&>(&ActionForTreeViewInterface::indexCollapsedEvent, index);
}

bool DM_ActionsHandlerForTreeView::indexExpandedEvent(const QModelIndex &index)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QModelIndex&>(&ActionForTreeViewInterface::indexExpandedEvent, index);
}

bool DM_ActionsHandlerForTreeView::itemDragEnterEvent(QDragEnterEvent *event)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, QDragEnterEvent*>(&ActionForTreeViewInterface::itemDragEnterEvent, event);
}

bool DM_ActionsHandlerForTreeView::itemDragMoveEvent(QDragMoveEvent *event)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, QDragMoveEvent*>(&ActionForTreeViewInterface::itemDragMoveEvent, event);
}

bool DM_ActionsHandlerForTreeView::itemDragLeaveEvent(QDragLeaveEvent *event)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, QDragLeaveEvent*>(&ActionForTreeViewInterface::itemDragLeaveEvent, event);
}

bool DM_ActionsHandlerForTreeView::itemDropEvent(QDropEvent *event)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, QDropEvent*>(&ActionForTreeViewInterface::itemDropEvent, event);
}

bool DM_ActionsHandlerForTreeView::showContextMenu(const QPoint &point)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QPoint&>(&ActionForTreeViewInterface::showContextMenu, point);
}

bool DM_ActionsHandlerForTreeView::showContextMenuOnHorizontalHeader(const QPoint &point)
{
    return callEventDynamicCastT<ActionForTreeViewInterface, const QPoint&>(&ActionForTreeViewInterface::showContextMenuOnHorizontalHeader, point);
}
