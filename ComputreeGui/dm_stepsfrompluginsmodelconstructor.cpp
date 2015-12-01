#include "dm_stepsfrompluginsmodelconstructor.h"

#include <QStandardItem>

#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "ct_stepseparator.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

DM_StepsFromPluginsModelConstructor::DM_StepsFromPluginsModelConstructor(const CDM_PluginManager &manager) : m_pluginManager(manager)
{
    m_model = new QStandardItemModel();
}

DM_StepsFromPluginsModelConstructor::~DM_StepsFromPluginsModelConstructor()
{
    delete m_model;
}

void DM_StepsFromPluginsModelConstructor::setTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type, bool enable)
{
    if(enable) {
        m_notVisible.removeAll(type);
        return;
    }

    m_notVisible.append(type);
}

bool DM_StepsFromPluginsModelConstructor::isTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type) const
{
    int i = m_notVisible.indexOf(type);

    if(i != -1)
        return false;

    QListIterator<DM_StepsFromPluginsModelConstructor::ItemType> it(m_notVisible);

    while(it.hasNext()) {
        if(it.next() & type)
            return false;
    }

    return true;
}

void DM_StepsFromPluginsModelConstructor::resetConfig()
{
    m_notVisible.clear();
}

void DM_StepsFromPluginsModelConstructor::construct()
{
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nom") << QObject::tr("Plugin"));

    QStandardItem *rootItem = m_model->invisibleRootItem();

    CT_StepsMenu *menu = m_pluginManager.stepsMenu();

    QList<CT_MenuLevel*> levels = menu->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext()) {
        CT_MenuLevel *level = it.next();
        QList<QStandardItem*> items = createItemsForLevelAndSubLevelRecursively(level);

        if(!items.isEmpty())
            rootItem->appendRow(items);
    }
}

QStandardItemModel* DM_StepsFromPluginsModelConstructor::model() const
{
    return m_model;
}

CT_VirtualAbstractStep *DM_StepsFromPluginsModelConstructor::stepFromIndex(const QModelIndex &index) const
{
    QStandardItem *item = m_model->itemFromIndex(index);

    if((item != NULL) && (item->data(DR_Type).toInt() & IT_Step))
        return (CT_VirtualAbstractStep*)item->data(DR_Pointer).value<void*>();

    return NULL;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForLevelAndSubLevelRecursively(const CT_MenuLevel *level, bool rootLevel)
{
    QList<QStandardItem*> l;

    QStandardItem *item = new QStandardItem(level->displayableName());
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)level), DR_Pointer);
    item->setData((int)rootLevel ? IT_RootLevel : IT_SubLevel, DR_Type);
    item->setData((int)rootLevel ? IT_RootLevel : IT_SubLevel, DR_SecondaryType);
    l << item;

    QStandardItem *colItem = new QStandardItem();
    colItem->setEditable(false);
    colItem->setData(qVariantFromValue((void*)level), DR_Pointer);
    colItem->setData((int)rootLevel ? IT_RootLevel : IT_SubLevel, DR_Type);
    colItem->setData((int)rootLevel ? IT_RootLevel : IT_SubLevel, DR_SecondaryType);
    l << colItem;

    // sub levels
    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext()) {
        QList<QStandardItem*> items = createItemsForLevelAndSubLevelRecursively(it.next(), false);

        if(!items.isEmpty())
            item->appendRow(items);
    }

    // steps
    QList<CT_VirtualAbstractStep*> steps = level->steps();
    QListIterator<CT_VirtualAbstractStep*> itS(steps);

    while(itS.hasNext()) {
        CT_VirtualAbstractStep *step = itS.next();
        QList<QStandardItem*> items;

        if(dynamic_cast<CT_AbstractStepLoadFile*>(step) != NULL)
            items = createItemsForStep(step, IT_StepLF);
        else if(dynamic_cast<CT_AbstractStepCanBeAddedFirst*>(step) != NULL)
            items = createItemsForStep(step, IT_StepCBAF);
        else
            items = createItemsForStep(step, IT_StepG);

        if(!items.isEmpty())
            item->appendRow(items);
    }

    // steps not founded
    QList<CT_MenuLevel::CT_NotFoundedStep> lnfs = level->stepsNotFounded();
    QListIterator<CT_MenuLevel::CT_NotFoundedStep> itNFS(lnfs);

    while(itNFS.hasNext()) {
        CT_MenuLevel::CT_NotFoundedStep nfs = itNFS.next();
        QList<QStandardItem*> items = createItemsForStep(nfs);

        if(!items.isEmpty())
            item->appendRow(items);
    }

    CT_MenuLevel *favoritesLevel = m_pluginManager.stepsMenu()->getOrCreateRootLevel(CT_StepsMenu::LO_Favorites);

    if((item->rowCount() == 0) && (level != favoritesLevel)) {
        qDeleteAll(l.begin(), l.end());
        l.clear();
    }

    return l;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForStep(CT_VirtualAbstractStep *step, ItemType stepType) const
{
    QList<QStandardItem*> l;

    QStandardItem *item = new QStandardItem(step->getPlugin()->getKeyForStep(*step));
    item->setEditable(false);
    item->setToolTip(QObject::tr("<html><p>%1</p><p>%2</p></html>").arg(step->getStepDescription()).arg(step->getStepDetailledDescription()));
    item->setData(qVariantFromValue((void*)step), DR_Pointer);
    item->setData((int)stepType, DR_Type);
    item->setData((int)stepType, DR_SecondaryType);
    l << item;

    QString pluginName = m_pluginManager.getPluginName(step->getPlugin());

    if (pluginName.startsWith("plug_"))
        pluginName.remove(0, 5);

    item = new QStandardItem(pluginName);
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)step->getPlugin()), DR_Pointer);
    item->setData((int)IT_Plugin, DR_Type);
    item->setData((int)stepType, DR_SecondaryType);
    l << item;

    return l;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForStep(CT_MenuLevel::CT_NotFoundedStep nfs) const
{
    QList<QStandardItem*> l;

    QStandardItem *item = new QStandardItem(nfs.stepKey);
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)NULL), DR_Pointer);
    item->setData((int)IT_StepNF, DR_Type);
    item->setData((int)IT_StepNF, DR_SecondaryType);
    l << item;

    QString pluginName = nfs.pluginName;

    if (pluginName.startsWith("plug_"))
        pluginName.remove(0, 5);

    item = new QStandardItem(pluginName);
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)NULL), DR_Pointer);
    item->setData((int)IT_Plugin, DR_Type);
    item->setData((int)IT_StepNF, DR_SecondaryType);
    l << item;

    return l;
}
