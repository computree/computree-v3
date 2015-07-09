#include "gstepviewdefault.h"
#include "ui_gstepviewdefault.h"

#include <QMenu>
#include <QAction>

GStepViewDefault::GStepViewDefault(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GStepViewDefault)
{
    ui->setupUi(this);

    m_constructor = NULL;
    m_proxy = NULL;
    m_nameConfig = DNC_Key;
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

void GStepViewDefault::reconstruct()
{
    m_constructor->construct();
    m_proxy->setSourceModel(m_constructor->model());

    if(m_proxy->rowCount() > 0)
        ui->treeView->expand(m_proxy->index(0, 0));
}

bool GStepViewDefault::searchStepByNameAndExpandParent(const QString &anyName)
{
    if(!anyName.trimmed().isEmpty())
        return recursiveSearchStepByNameAndExpandParent(m_proxy->index(0, 0), anyName);

    return false;
}

bool GStepViewDefault::recursiveSearchStepByNameAndExpandParent(const QModelIndex &index, const QString &anyName)
{
    if(index.isValid()) {
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

void GStepViewDefault::on_pushButtonConfigTypeOnTree_clicked()
{
    QMenu menu(this);

    QAction *act = new QAction(tr("Plugins"), &menu);
    act->setCheckable(true);
    act->setChecked(m_constructor->isTypeVisible(DM_StepsFromPluginsModelConstructor::IT_Plugin));
    act->setData((int)DM_StepsFromPluginsModelConstructor::IT_Plugin);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setTypeVisible(bool)), Qt::DirectConnection);

    menu.addAction(act);

    act = new QAction(tr("Séparateurs"), &menu);
    act->setCheckable(true);
    act->setChecked(m_constructor->isTypeVisible(DM_StepsFromPluginsModelConstructor::IT_Separator));
    act->setData((int)DM_StepsFromPluginsModelConstructor::IT_Separator);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setTypeVisible(bool)), Qt::DirectConnection);

    menu.addAction(act);

    menu.exec(ui->pushButtonConfigTypeOnTree->mapToGlobal(ui->pushButtonConfigTypeOnTree->rect().center()));
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

    act = new QAction(tr("Nom de l'étape"), &menu);
    act->setCheckable(true);
    act->setChecked(m_nameConfig.testFlag(DNC_StepName));
    act->setData((int)DNC_StepName);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

    menu.addAction(act);

    act = new QAction(tr("Nom affichable de l'étape"), &menu);
    act->setCheckable(true);
    act->setChecked(m_nameConfig.testFlag(DNC_StepDisplayableName));
    act->setData((int)DNC_StepDisplayableName);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(setStepName(bool)), Qt::DirectConnection);

    menu.addAction(act);

    menu.exec(ui->pushButtonConfigTypeOnTree->mapToGlobal(ui->pushButtonConfigTypeOnTree->rect().center()));
}

void GStepViewDefault::setTypeVisible(bool visible)
{
    QAction *act = (QAction*)sender();
    DM_StepsFromPluginsModelConstructor::ItemType type = (DM_StepsFromPluginsModelConstructor::ItemType)act->data().toInt();

    m_constructor->setTypeVisible(type, visible);

    reconstruct();
}

void GStepViewDefault::setStepName(bool enable)
{
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
