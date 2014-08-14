#ifndef GACTIONSMANAGER_H
#define GACTIONSMANAGER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelection>

#include "interfaces.h"

class CDM_PluginManager;
class DM_ActionsManager;
class GDocumentManagerView;
class DM_DocumentView;
class DM_ActionsHandler;

namespace Ui {
class GActionsManager;
}

class GActionsManager : public QWidget
{
    Q_OBJECT

public:
    explicit GActionsManager(QWidget *parent = 0);
    ~GActionsManager();

    /**
     * @brief Set the action manager that can have actions by step
     *        when she go in manual mode or debug mode.
     */
    void setActionsManager(const DM_ActionsManager *actionsManager);

    /**
     * @brief Set the document manager to change the selected action
     *        when a document is activated
     */
    void setDocumentManagerView(const GDocumentManagerView *docManager);

private:
    Ui::GActionsManager     *ui;
    DM_ActionsManager       *m_actionsManager;
    GDocumentManagerView    *m_docManager;
    QStandardItemModel      m_model;
    ActionInterface         *m_currentAction;
    DM_DocumentView         *m_currentDoc;

    QMap<QString, bool> menuExpandedState() const;
    QList<QStandardItem*> createItemForAction(ActionInterface *ac) const;

private slots:
    void documentActivated(DM_DocumentView *view);
    void documentCurrentActionChanged(ActionInterface *action);
    void itemClicked(const QModelIndex &index);
    void refreshView();

    void setDefaultActionToDocument();
};

#endif // GACTIONSMANAGER_H
