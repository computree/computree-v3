#include "gactionsmanager.h"
#include "ui_gactionsmanager.h"

#include "cdm_pluginmanager.h"
#include "dm_actionsmanager.h"
#include "gdocumentmanagerview.h"
#include "dm_actionshandler.h"

#include "ct_actions/abstract/ct_abstractaction.h"

#include <QToolButton>

GActionsManager::GActionsManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GActionsManager)
{
    ui->setupUi(this);

    ui->treeView->setModel(&m_model);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setFocusPolicy(Qt::NoFocus);

    m_actionsManager = NULL;
    m_docManager = NULL;
    m_currentAction = NULL;
    m_currentDoc = NULL;

    QString style = QString("background-color: qlineargradient(spread:pad,"
                                                               "x1:0, y1:0, x2:0, y2:1, "
                                                               "stop:0 rgba(242, 242, 242, 255), "
                                                               "stop:1 rgba(228, 228, 228, 255)); "
                            "border-top-width:1px; "
                            "border-top-style: solid; "
                            "border-top-color: rgb(160, 160, 160); "
                            "border-bottom-width:1px; "
                            "border-bottom-style: solid; "
                            "border-bottom-color: rgb(160, 160, 160);");

    ui->treeView->setStyleSheet("QTreeView::item:has-children {"
                                + style +
                                " }"
                                ""
                                "QTreeView::branch:has-children {"
                                + style +
                                " }"
                                ""
                                "QTreeView::branch:closed:has-children {"
                                   "image: url(:/Icones/Icones/stylesheet-branch-closed.png);"
                                "}"
                                "QTreeView::branch:open:has-children {"
                                   "image: url(:/Icones/Icones/stylesheet-branch-open.png);"
                                "}");

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

GActionsManager::~GActionsManager()
{
    delete ui;
}

void GActionsManager::setActionsManager(const DM_ActionsManager* actionsManager)
{
    if(m_actionsManager != NULL)
        disconnect(m_actionsManager, NULL, this, NULL);

    m_actionsManager = (DM_ActionsManager*)actionsManager;

    if(m_actionsManager != NULL)
    {
        connect(m_actionsManager, SIGNAL(actionsCollectionChanged()), this, SLOT(setDefaultActionToDocument()));
        connect(m_actionsManager, SIGNAL(actionsCollectionChanged()), this, SLOT(refreshView()));
    }
}

void GActionsManager::setDocumentManagerView(const GDocumentManagerView* docManager)
{
    if(m_docManager != NULL)
        disconnect(m_docManager, NULL, this, NULL);

    m_docManager = (GDocumentManagerView*)docManager;

    if(m_docManager != NULL)
    {
        connect(m_docManager, SIGNAL(documentActivated(DM_DocumentView*)), this, SLOT(documentActivated(DM_DocumentView*)));
    }
}

void GActionsManager::refreshView()
{
    // create a list of expanded menus
    QMap<QString, bool> states = menuExpandedState();

    m_model.clear();

    if(m_actionsManager != NULL)
    {
        QMap<QString, CT_AbstractAction*> byType;

        // Create a map of actions sorted by type. Actions added in the map is get from plugins.
        QList<CT_AbstractAction*> actions = m_actionsManager->actionsFromPlugins();
        QListIterator<CT_AbstractAction*> it(actions);

        while(it.hasNext())
        {
            CT_AbstractAction *ac = it.next();
            byType.insertMulti(ac->type(), ac);
        }

        // Create the model of the treeview from the map
        QList<QString> types = byType.uniqueKeys();
        QListIterator<QString> itK(types);

        while(itK.hasNext())
        {
            const QString &type = itK.next();

            QList<CT_AbstractAction*> actionsType = byType.values(type);

            QStandardItem *menu = new QStandardItem(type);
            menu->setEditable(false);
            menu->setTextAlignment(Qt::AlignHCenter);
            menu->setSelectable(false);
            menu->setData(type);

            QListIterator<CT_AbstractAction*> itA(actionsType);

            while(itA.hasNext())
                menu->appendRow(createItemForAction(itA.next()));

            m_model.appendRow(menu);

            ui->treeView->setExpanded(m_model.indexFromItem(menu), states.value(menu->data().toString(), true));
        }

        // Continue create the model for actions added by step
        actions = m_actionsManager->actionsFromSteps();

        if(!actions.isEmpty())
        {
            QStandardItem *menu = new QStandardItem(tr("Manual/Debug mode"));
            menu->setEditable(false);
            menu->setTextAlignment(Qt::AlignHCenter);
            menu->setSelectable(false);
            menu->setData(menu->text());

            QListIterator<CT_AbstractAction*> itS(actions);

            while(itS.hasNext())
                menu->appendRow(createItemForAction(itS.next()));

            m_model.appendRow(menu);

            ui->treeView->setExpanded(m_model.indexFromItem(menu), states.value(menu->data().toString(), true));
        }
    }
}

void GActionsManager::setDefaultActionToDocument()
{
    if(m_currentDoc != NULL)
    {
        CT_AbstractAction *ac = m_currentDoc->currentAction();

        if(ac == NULL)
        {
            ac = m_actionsManager->action("PB_ActionSelectItemDrawableGV");

            if(ac != NULL)
                m_currentDoc->setCurrentAction(ac);
        }
    }
}

QMap<QString, bool> GActionsManager::menuExpandedState() const
{
    QMap<QString, bool> list;

    QStandardItem *root = m_model.invisibleRootItem();

    int size = root->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *menu = root->child(i);
        list.insert(menu->data().toString(), ui->treeView->isExpanded(m_model.indexFromItem(menu)));
    }

    return list;
}

QList<QStandardItem*> GActionsManager::createItemForAction(CT_AbstractAction *ac) const
{
    QList<QStandardItem*> list;

    QStandardItem *item = new QStandardItem(ac->icon(), ac->title());
    item->setToolTip(ac->toolTip());
    item->setWhatsThis(ac->description());
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState((m_currentAction != NULL) ? ((ac->uniqueName() == m_currentAction->uniqueName()) ? Qt::Checked : Qt::Unchecked) : Qt::Unchecked);
    item->setData(qVariantFromValue((void*)ac));

    list << item;

    return list;
}

void GActionsManager::documentActivated(DM_DocumentView *view)
{
    if((m_currentDoc != NULL)
            && m_docManager->containsDocument(m_currentDoc))
        disconnect(m_currentDoc, NULL, this, NULL);

    m_currentDoc = view;
    m_currentAction = NULL;

    if(m_currentDoc != NULL)
    {
        connect(m_currentDoc, SIGNAL(currentActionChanged(CT_AbstractAction*)), this, SLOT(documentCurrentActionChanged(CT_AbstractAction*)));
        m_currentAction = view->currentAction();

        if(m_currentAction == NULL)
            setDefaultActionToDocument();
    }

    refreshView();
}

void GActionsManager::documentCurrentActionChanged(CT_AbstractAction *action)
{
    DM_DocumentView *doc = (DM_DocumentView*)sender();

    if(doc != m_currentDoc)
    {
        disconnect(doc, NULL, this, NULL);
    }
    else
    {
        m_currentAction = action;

        refreshView();
    }
}

void GActionsManager::itemClicked(const QModelIndex &index)
{
    QStandardItem *item = m_model.itemFromIndex(index);

    if(!item->hasChildren())
    {
        CT_AbstractAction *ac = (CT_AbstractAction*)item->data().value<void*>();

        if((m_currentDoc != NULL)
                && (((m_currentAction != NULL) && (m_currentAction->uniqueName() != ac->uniqueName()))
                    || (m_currentAction == NULL)))
        {
            if(!m_currentDoc->setCurrentAction(ac))
                item->setCheckState(Qt::Unchecked);

        }
        else
            item->setCheckState(Qt::Checked);
    }
}
