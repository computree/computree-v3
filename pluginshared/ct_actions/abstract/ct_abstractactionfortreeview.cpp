#include "ct_abstractactionfortreeview.h"

CT_AbstractActionForTreeView::CT_AbstractActionForTreeView() : CT_AbstractAction()
{
    m_treeView = NULL;
}

void CT_AbstractActionForTreeView::setTreeView(const TreeViewInterface *view)
{
    m_treeView = (TreeViewInterface*)view;
}

TreeViewInterface *CT_AbstractActionForTreeView::treeView() const
{
    return m_treeView;
}
