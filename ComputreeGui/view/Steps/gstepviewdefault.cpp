#include "gstepviewdefault.h"
#include "ui_gstepviewdefault.h"

#include <QMenu>
#include <QAction>
#include <QModelIndex>

GStepViewDefault::GStepViewDefault(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GStepViewDefault)
{
    ui->setupUi(this);

    m_constructor = NULL;
    m_proxy = NULL;
    m_nameConfig = DNC_Key;

    // To show custom context menu
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(indexDoubleClicked(QModelIndex)));
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTreeViewContextMenu(QPoint)), Qt::QueuedConnection);
}

GStepViewDefault::~GStepViewDefault()
{
    delete ui;

    delete m_proxy;
    delete m_constructor;
}

void GStepViewDefault::init(const CDM_PluginManager &pManager)
{
    if(m_constructor == NULL) {
        m_pluginManager = (CDM_PluginManager*)&pManager;
        m_constructor = new DM_StepsFromPluginsModelConstructor(pManager);
        m_proxy = new DM_StepTreeViewDefaultProxyModel(this);
        m_proxy->setStepsNameFunction(&staticStepsName, this);

        // the view use the proxy model
        ui->treeView->setModel(m_proxy);
        ui->treeView->setHeaderHidden(true);

        connect(ui->treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
    }
}

DM_StepTreeViewDefaultProxyModel *GStepViewDefault::proxy() const
{
    return m_proxy;
}

GStepsTreeViewDefault *GStepViewDefault::treeView() const
{
    return ui->treeView;
}

DM_StepsFromPluginsModelConstructor *GStepViewDefault::constructor() const
{
    return m_constructor;
}

void GStepViewDefault::setDisplayConfiguration(DisplayNameConfigs configs)
{
    m_nameConfig = configs;
    m_proxy->invalidate();
}

void GStepViewDefault::setContextMenuOnType(QMenu *contextMenu, DM_StepsFromPluginsModelConstructor::ItemType type)
{
    m_contextMenus.insert(type, contextMenu);
}

CT_VirtualAbstractStep *GStepViewDefault::currentStepSelected() const
{
    QModelIndexList selected = treeView()->selectionModel()->selectedRows();

    if(!selected.isEmpty()) {
        QModelIndex newIndex = selected.first();

        if(newIndex.isValid()) {
            if(newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt() & DM_StepsFromPluginsModelConstructor::IT_Step)
                return (CT_VirtualAbstractStep*)newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>();
        }
    }

    return NULL;
}

CT_MenuLevel *GStepViewDefault::currentLevelSelected() const
{
    QModelIndexList selected = treeView()->selectionModel()->selectedRows();

    if(!selected.isEmpty()) {
        QModelIndex newIndex = selected.first();

        if(newIndex.isValid()) {
            if(newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt() & DM_StepsFromPluginsModelConstructor::IT_Level)
                return (CT_MenuLevel*)newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>();
        }
    }

    return NULL;
}

void GStepViewDefault::reconstruct()
{
    if(m_constructor != NULL && m_proxy != NULL) {
        m_constructor->construct();
        m_proxy->setSourceModel(m_constructor->model());

        if(m_proxy->rowCount() > 0)
            ui->treeView->expand(m_proxy->index(0, 0));

        resizeColumnsOfTreeView();
    }
}

bool GStepViewDefault::searchStepByNameAndExpandParent(const QString &anyName)
{
    if(m_proxy != NULL && !anyName.trimmed().isEmpty())
        return recursiveSearchStepByNameAndExpandParent(m_proxy->index(0, 0), anyName);

    return false;
}

bool GStepViewDefault::recursiveSearchStepByNameAndExpandParent(const QModelIndex &index, const QString &anyName)
{
    if(m_proxy != NULL && index.isValid()) {
        int n = m_proxy->rowCount(index);

        for(int i=0; i<n; ++i)
        {
            if(recursiveSearchStepByNameAndExpandParent(index.child(i, 0), anyName))
                return true;
        }

        if(n == 0) {
            QStandardItem *item = ((QStandardItemModel*)m_proxy->sourceModel())->itemFromIndex(m_proxy->mapToSource(index));

            if(item != NULL) {
                if(item->data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt() & DM_StepsFromPluginsModelConstructor::IT_Step) {

                    CT_VirtualAbstractStep *step = ((CT_VirtualAbstractStep*)item->data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>());

                    if((step->getStepName() == anyName)
                            || (step->getPlugin()->getKeyForStep(*step) == anyName)
                            || (step->getStepCustomName() == anyName)
                            || (step->getStepExtendedName() == anyName)
                            || (step->getStepDisplayableName() == anyName)) {

                        ui->treeView->expand(index.parent());
                        ui->treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
                        ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

                        if(step->getStepName() == anyName) {
                            if(!(m_nameConfig & DNC_StepName)) {
                                m_nameConfig |= DNC_StepName;
                                m_proxy->invalidate();
                            }
                        } else if(step->getStepDisplayableName() == anyName) {
                            if(!(m_nameConfig & DNC_StepDisplayableName)) {
                                m_nameConfig |= DNC_StepDisplayableName;
                                m_proxy->invalidate();
                            }
                        } else if(step->getPlugin()->getKeyForStep(*step) == anyName) {
                            if(!(m_nameConfig & DNC_Key)) {
                                m_nameConfig |= DNC_Key;
                                m_proxy->invalidate();
                            }
                        }

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void GStepViewDefault::resizeColumnsOfTreeView()
{
    if(m_constructor != NULL) {
        int n = m_constructor->model()->columnCount();

        if(n > 0) {
            int width = ui->treeView->width()-20;

            int lastColumnsCount = n-1;
            int wForLastColumns = 0;

            if(lastColumnsCount > 0)
                wForLastColumns = ((width*(lastColumnsCount*10))/100)/lastColumnsCount;

            int wForFirstColumn = width-(wForLastColumns*lastColumnsCount);

            ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Fixed);
            ui->treeView->header()->resizeSection(0, wForFirstColumn);

            for(int i=1; i<n; ++i) {
                ui->treeView->header()->setSectionResizeMode(i, QHeaderView::Fixed);
                ui->treeView->header()->resizeSection(i, wForLastColumns);
            }
        }
    }
}

QMenu *GStepViewDefault::contextMenuForType(DM_StepsFromPluginsModelConstructor::ItemType type)
{
    QMenu *menu = m_contextMenus.value(type, NULL);

    if(menu != NULL)
        return menu;

    QMapIterator<DM_StepsFromPluginsModelConstructor::ItemType, QMenu*> it(m_contextMenus);

    while(it.hasNext()) {
        it.next();

        if((it.key() != type) && (it.key() & type)) {
            return it.value();
        }
    }

    return NULL;
}

void GStepViewDefault::resizeEvent(QResizeEvent *e)
{
    resizeColumnsOfTreeView();

    QWidget::resizeEvent(e);
}

void GStepViewDefault::on_pushButtonConfigStepName_clicked()
{
    QMenu menu(this);

    QAction *act = new QAction(tr("Clé de l'étape au sein du plugin ou d'un script"), &menu);
    act->setCheckable(true);
    act->setChecked(m_nameConfig.testFlag(DNC_Key));
    act->setData((int)DNC_Key);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

    menu.addAction(act);

    /*act = new QAction(tr("Nom de l'étape"), &menu);
    act->setCheckable(true);
    act->setChecked(m_nameConfig.testFlag(DNC_StepName));
    act->setData((int)DNC_StepName);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

    menu.addAction(act);*/

    act = new QAction(tr("Nom de l'étape"), &menu);
    act->setCheckable(true);
    act->setChecked(m_nameConfig.testFlag(DNC_StepDisplayableName));
    act->setData((int)DNC_StepDisplayableName);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

    menu.addAction(act);

    menu.exec(ui->pushButtonConfigStepName->mapToGlobal(ui->pushButtonConfigStepName->rect().center()));
}

void GStepViewDefault::setStepName(bool enable)
{
    if(m_proxy != NULL) {
        QAction *act = (QAction*)sender();
        DisplayNameConfig c = (DisplayNameConfig)act->data().toInt();

        if(enable)
            m_nameConfig |= c;
        else
            m_nameConfig &= ~c;

        if(m_nameConfig == 0)
            m_nameConfig = DNC_Key;

        m_proxy->invalidate();
    }
}

bool GStepViewDefault::staticStepsName(QString &name, const QModelIndex &index, void *context)
{
    GStepViewDefault *thisPtr = (GStepViewDefault*)context;

    CT_VirtualAbstractStep *step = thisPtr->constructor()->stepFromIndex(thisPtr->proxy()->mapToSource(index));

    if(step == NULL)
        return false;

    name = "";

    if(thisPtr->m_nameConfig.testFlag(DNC_Key))
        name += " " + step->getPlugin()->getKeyForStep(*step) + " ";

    if(thisPtr->m_nameConfig.testFlag(DNC_StepName) && !name.contains(" " + step->getStepName() + " ")) {
        if(!name.isEmpty())
            name += "/";

        name += " " + step->getStepName() + " ";
    }

    if(thisPtr->m_nameConfig.testFlag(DNC_StepDisplayableName) && !name.contains(" " + step->getStepDisplayableName()+ " ")) {
        if(!name.isEmpty())
            name += "/";

        name += " " + step->getStepDisplayableName() + " ";
    }

    return true;
}

void GStepViewDefault::indexDoubleClicked(const QModelIndex &index)
{
    if(index.isValid()) {
        QModelIndex newIndex = index.model()->sibling(index.row(), 0, index);

        if(newIndex.isValid()) {
            if(newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt() & DM_StepsFromPluginsModelConstructor::IT_Step)
                emit stepDoubleClicked((CT_VirtualAbstractStep*)newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>());
        }
    }
}

void GStepViewDefault::showTreeViewContextMenu(const QPoint &point)
{
    QModelIndexList l = ui->treeView->selectionModel()->selectedRows();

    if(!l.isEmpty()) {
        QMenu *menu = contextMenuForType((DM_StepsFromPluginsModelConstructor::ItemType)l.first().data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt());

        if(menu != NULL)
            menu->exec(ui->treeView->viewport()->mapToGlobal(point));
    }
}

void GStepViewDefault::selectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    QModelIndexList l = ui->treeView->selectionModel()->selectedRows();

    if(!l.isEmpty()) {
        QModelIndex newIndex = l.first();

        if(newIndex.isValid()) {
            int type = newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt();

            if(type & DM_StepsFromPluginsModelConstructor::IT_Step) {
                emit stepSelected((CT_VirtualAbstractStep*)newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>());
                return;
            } else if(type & DM_StepsFromPluginsModelConstructor::IT_Level) {
                emit levelSelected((CT_MenuLevel*)newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>());
                return;
            }
        }
    }

    emit stepSelected((CT_VirtualAbstractStep*)NULL);
}
