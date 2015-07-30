#ifndef DM_ACTIONTOOSGWORKER_H
#define DM_ACTIONTOOSGWORKER_H

#include "tools/worker/dm_abstractworker.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"

#include <osg/Group>

/**
 * @brief A worker that convert an action to an osg::Group with the help of DM_PainterToOsgElements
 */
class DM_ActionToOsgWorker : public DM_AbstractWorker
{
    Q_OBJECT

public:
    DM_ActionToOsgWorker();

    /**
     * @brief Set the action to convert
     */
    void setActionToConvert(CT_AbstractActionForGraphicsView *action);

    /**
     * @brief Set the graphics view to pass to method "draw" of action
     */
    void setGraphicsView(GraphicsViewInterface *gv);

    /**
     * @brief Returns the result available when the worker has finished
     */
    DM_PainterToOsgElementsResult results() const;

public slots:
    void apply();

private:
    CT_AbstractActionForGraphicsView    *m_action;
    GraphicsViewInterface               *m_gv;
    DM_PainterToOsgElements             m_painter;

private slots:
    void cancelEvent();
};

#endif // DM_ACTIONTOOSGWORKER_H
