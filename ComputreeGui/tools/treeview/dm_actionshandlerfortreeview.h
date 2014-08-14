#ifndef DM_ACTIONSHANDLERFORTREEVIEW_H
#define DM_ACTIONSHANDLERFORTREEVIEW_H

#include "dm_actionshandler.h"

class DM_ActionsHandlerForTreeView : public DM_ActionsHandler
{
public:
    DM_ActionsHandlerForTreeView(const CDM_ActionsManager &actionManager);

    bool indexActivatedEvent(const QModelIndex &index);
    bool indexClickedEvent(const QModelIndex &index);
    bool indexDoubleClickedEvent(const QModelIndex &index);
    bool indexCollapsedEvent(const QModelIndex &index);
    bool indexExpandedEvent(const QModelIndex &index);
    bool itemDragEnterEvent(QDragEnterEvent *event);
    bool itemDragMoveEvent(QDragMoveEvent *event);
    bool itemDragLeaveEvent(QDragLeaveEvent *event);
    bool itemDropEvent(QDropEvent *event);
    bool showContextMenu(const QPoint &point);
    bool showContextMenuOnHorizontalHeader(const QPoint &point);
};

#endif // DM_ACTIONSHANDLERFORTREEVIEW_H
