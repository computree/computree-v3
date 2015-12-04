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
    m_nameConfig = DNC_StepKey;

    // To show custom context menu
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->checkBoxShowAtLastPosition->setVisible(false);

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

    emit displayNameConfigurationChanged(m_nameConfig);
    m_proxy->invalidate();
}

GStepViewDefault::DisplayNameConfigs GStepViewDefault::displayConfiguration() const
{
    return m_nameConfig;
}

void GStepViewDefault::setShowAtLastPositionCheckboxVisible(bool e)
{
    ui->checkBoxShowAtLastPosition->setVisible(e);
}

void GStepViewDefault::setShowAtLastPosition(bool v)
{
    ui->checkBoxShowAtLastPosition->setChecked(v);
}

bool GStepViewDefault::showAtLastPosition() const
{
    return ui->checkBoxShowAtLastPosition->isChecked();
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

GStepViewDefault::DisplayNameConfigs GStepViewDefault::stepNameConfiguration() const
{
    return m_nameConfig;
}

QString GStepViewDefault::staticGetStepNameFromConfiguration(CT_VirtualAbstractStep *step, const GStepViewDefault::DisplayNameConfigs &config)
{
    QString name;

    QString shortDescription = " " + step->getStepDescription() + " ";
    QString key = " " + step->getPlugin()->getKeyForStep(*step) + " ";
    QString displayableName = " " + step->getStepDisplayableName()+ " ";

    if(config.testFlag(DNC_StepShortDescription) )
        name += shortDescription;

    if(config.testFlag(DNC_StepKey) && !name.contains(key)) {
        if(!name.isEmpty())
            name += "/";

        name += key;
    }

    if(config.testFlag(DNC_StepDisplayableName) && !name.contains(displayableName)) {
        if(!name.isEmpty())
            name += "/";

        name += displayableName;
    }

    return name;
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

bool GStepViewDefault::searchStepByNameAndExpandParent(const QString &anyName, bool changeDisplayConfigIfNameFoundedIsNotDisplayed)
{
    if(m_proxy != NULL && !anyName.trimmed().isEmpty()) {

        int n = m_proxy->rowCount();

        for(int i=0; i<n; ++i) {
            if(recursiveSearchStepByNameAndExpandParent(m_proxy->index(i, 0), anyName, changeDisplayConfigIfNameFoundedIsNotDisplayed))
                return true;
        }
    }

    return false;
}

bool GStepViewDefault::searchOriginalStepAndExpandParent(CT_VirtualAbstractStep *step)
{
    if(step != NULL)
        return searchStepByNameAndExpandParent(step->getPlugin()->getKeyForStep(*step), false);

    return false;
}

bool GStepViewDefault::recursiveSearchStepByNameAndExpandParent(const QModelIndex &index, const QString &anyName, bool changeDisplayConfigIfNameFoundedIsNotDisplayed)
{
    if(m_proxy != NULL && index.isValid()) {
        int n = m_proxy->rowCount(index);

        for(int i=0; i<n; ++i)
        {
            if(recursiveSearchStepByNameAndExpandParent(index.child(i, 0), anyName, changeDisplayConfigIfNameFoundedIsNotDisplayed))
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
                            || (step->getStepDisplayableName() == anyName)
                            || (step->getStepDescription() == anyName)) {

                        ui->treeView->expand(index.parent());
                        ui->treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
                        ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

                        if(changeDisplayConfigIfNameFoundedIsNotDisplayed) {
                            if(step->getStepDescription() == anyName) {
                                if(!(m_nameConfig & DNC_StepShortDescription)) {
                                    m_nameConfig |= DNC_StepShortDescription;
                                    emit displayNameConfigurationChanged(m_nameConfig);
                                    m_proxy->invalidate();
                                }
                            } else if(step->getStepDisplayableName() == anyName) {
                                if(!(m_nameConfig & DNC_StepDisplayableName)) {
                                    m_nameConfig |= DNC_StepDisplayableName;
                                    emit displayNameConfigurationChanged(m_nameConfig);
                                    m_proxy->invalidate();
                                }
                            } else if(step->getPlugin()->getKeyForStep(*step) == anyName) {
                                if(!(m_nameConfig & DNC_StepKey)) {
                                    m_nameConfig |= DNC_StepKey;
                                    emit displayNameConfigurationChanged(m_nameConfig);
                                    m_proxy->invalidate();
                                }
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
            int wForLastColumns = 0;
            int lastColumnIndex = n-1;
            QFontMetrics fm = ui->treeView->fontMetrics();

            if(lastColumnIndex > 0)
                getMaxSizeOfColumnsRecursively(m_constructor->model()->invisibleRootItem(), "", fm, lastColumnIndex, wForLastColumns);

            int wForFirstColumn = (ui->treeView->width()-20)-wForLastColumns;

            ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Fixed);
            ui->treeView->header()->resizeSection(0, wForFirstColumn);

            for(int i=1; i<n; ++i) {
                ui->treeView->header()->setSectionResizeMode(i, QHeaderView::Fixed);
                ui->treeView->header()->resizeSection(i, wForLastColumns);
            }
        }
    }
}

void GStepViewDefault::getMaxSizeOfColumnsRecursively(QStandardItem *item, const QString &text, const QFontMetrics &fm, const int &columnIndex, int &maxSize)
{
    maxSize = qMax(maxSize, fm.width(text));

    int m = item->rowCount();

    for(int i=0; i<m; ++i)
        getMaxSizeOfColumnsRecursively(item->child(i, 0), item->child(i, columnIndex)->text(), fm, columnIndex, maxSize);
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
    if(m_proxy != NULL) {
        QMenu menu(this);

        QAction *act = new QAction(tr("Clé de l'étape au sein du plugin ou d'un script"), &menu);
        act->setCheckable(true);
        act->setChecked(m_nameConfig.testFlag(DNC_StepKey));
        act->setData((int)DNC_StepKey);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

        menu.addAction(act);

        act = new QAction(tr("Nom de l'étape"), &menu);
        act->setCheckable(true);
        act->setChecked(m_nameConfig.testFlag(DNC_StepDisplayableName));
        act->setData((int)DNC_StepDisplayableName);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

        menu.addAction(act);

        act = new QAction(tr("Description courte"), &menu);
        act->setCheckable(true);
        act->setChecked(m_nameConfig.testFlag(DNC_StepShortDescription));
        act->setData((int)DNC_StepShortDescription);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

        menu.addAction(act);

        menu.exec(ui->pushButtonConfigStepName->mapToGlobal(ui->pushButtonConfigStepName->rect().center()));
    }
}

void GStepViewDefault::on_pushButtonConfigSearch_clicked()
{
    if(m_proxy != NULL) {
        QMenu menu(this);

        QAction *act = new QAction(tr("Clé de l'étape au sein du plugin ou d'un script"), &menu);
        act->setCheckable(true);
        act->setChecked(m_proxy->filterConfiguration().testFlag(DM_StepTreeViewDefaultProxyModel::FC_StepKey));
        act->setData((int)DM_StepTreeViewDefaultProxyModel::FC_StepKey);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setSearchConfiguration(bool)), Qt::DirectConnection);

        menu.addAction(act);

        act = new QAction(tr("Nom de l'étape"), &menu);
        act->setCheckable(true);
        act->setChecked(m_proxy->filterConfiguration().testFlag(DM_StepTreeViewDefaultProxyModel::FC_StepDisplayableName));
        act->setData((int)DM_StepTreeViewDefaultProxyModel::FC_StepDisplayableName);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setSearchConfiguration(bool)), Qt::DirectConnection);

        menu.addAction(act);

        act = new QAction(tr("Description courte"), &menu);
        act->setCheckable(true);
        act->setChecked(m_proxy->filterConfiguration().testFlag(DM_StepTreeViewDefaultProxyModel::FC_StepShortDescription));
        act->setData((int)DM_StepTreeViewDefaultProxyModel::FC_StepShortDescription);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setSearchConfiguration(bool)), Qt::DirectConnection);

        menu.addAction(act);

        act = new QAction(tr("Description détaillée"), &menu);
        act->setCheckable(true);
        act->setChecked(m_proxy->filterConfiguration().testFlag(DM_StepTreeViewDefaultProxyModel::FC_StepFullDescription));
        act->setData((int)DM_StepTreeViewDefaultProxyModel::FC_StepFullDescription);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(setSearchConfiguration(bool)), Qt::DirectConnection);

        menu.addAction(act);

        menu.exec(ui->pushButtonConfigSearch->mapToGlobal(ui->pushButtonConfigSearch->rect().center()));
    }
}

void GStepViewDefault::on_lineEditSearch_textChanged(const QString &text)
{
    QString userText = text;
    QRegExp regFromString(".*" + text + ".*", Qt::CaseInsensitive);
    QRegExp userReg(userText.remove("r:").remove("i:"));

    if(text.contains("i:"))
        userReg.setCaseSensitivity(Qt::CaseInsensitive);

    if(text.isEmpty())
        m_proxy->setFilterRegExp(".*");
    else if(text.startsWith("r:"))
        m_proxy->setFilterRegExp(userReg);
    else
        m_proxy->setFilterRegExp(regFromString);

    m_proxy->invalidate();
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
            m_nameConfig = DNC_StepKey;

        m_proxy->invalidate();
        emit displayNameConfigurationChanged(m_nameConfig);
    }
}

void GStepViewDefault::setSearchConfiguration(bool enable)
{
    if(m_proxy != NULL) {
        QAction *act = (QAction*)sender();
        DM_StepTreeViewDefaultProxyModel::FilterConfig c = (DM_StepTreeViewDefaultProxyModel::FilterConfig)act->data().toInt();
        DM_StepTreeViewDefaultProxyModel::FilterConfigs cs = m_proxy->filterConfiguration();

        if(enable)
            cs |= c;
        else
            cs &= ~c;

        if(cs == 0)
            cs = DM_StepTreeViewDefaultProxyModel::FC_StepDisplayableName;

        m_proxy->setFilterConfiguration(cs);
    }
}

bool GStepViewDefault::staticStepsName(QString &name, const QModelIndex &index, void *context)
{
    GStepViewDefault *thisPtr = (GStepViewDefault*)context;

    CT_VirtualAbstractStep *step = thisPtr->constructor()->stepFromIndex(thisPtr->proxy()->mapToSource(index));

    if(step == NULL)
        return false;

    name = staticGetStepNameFromConfiguration(step, thisPtr->m_nameConfig);

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
                emit levelSelected((CT_MenuLevel*)newIndex.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>(), type);
                return;
            }
        }
    }

    emit stepSelected((CT_VirtualAbstractStep*)NULL);
}

void GStepViewDefault::on_pb_replaceDefualt_clicked()
{
    emit replaceToDefault();
}
