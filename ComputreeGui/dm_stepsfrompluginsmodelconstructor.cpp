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

void DM_StepsFromPluginsModelConstructor::setUseStepsOfPlugins(const QList<CT_AbstractStepPlugin *> &plugins)
{
    m_plugins = plugins;
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
    m_plugins.clear();
}

void DM_StepsFromPluginsModelConstructor::construct()
{
    m_model->clear();

    foreach (CT_AbstractStepPlugin *plugin, m_plugins) {

        QList<QStandardItem*> items = createItemsForPlugin(plugin);

        QStandardItem *rootItem = NULL;

        if(!items.isEmpty())
            rootItem = items.first();
        else
            rootItem = m_model->invisibleRootItem();

        QList<QStandardItem*> baseSepItems;

        QStandardItem *baseSepItem = NULL;

        if(isTypeVisible(IT_Separator)) {
            baseSepItem = new QStandardItem(QObject::tr("Autres"));
            baseSepItem->setEditable(false);
            baseSepItem->setData((int)IT_SeparatorOther, DR_Type);

            baseSepItems << baseSepItem;
        }

        createAndAddItems<CT_StepLoadFileSeparator, CT_AbstractStepLoadFile>(plugin->getOpenFileStepAvailable(), baseSepItem, rootItem, IT_StepLF);

        createAndAddItems<CT_StepCanBeAddedFirstSeparator, CT_AbstractStepCanBeAddedFirst>(plugin->getCanBeAddedFirstStepAvailable(), baseSepItem, rootItem, IT_StepCBAF);

        createAndAddItems<CT_StepSeparator, CT_VirtualAbstractStep>(plugin->getGenericsStepAvailable(), baseSepItem, rootItem, IT_StepG);

        if((baseSepItem != NULL) && (baseSepItem->rowCount() > 0))
            rootItem->appendRow(baseSepItems);
        else
            qDeleteAll(baseSepItems.begin(), baseSepItems.end());

        if(!items.isEmpty() && (rootItem->rowCount() > 0))
            m_model->invisibleRootItem()->appendRow(items);
        else
            qDeleteAll(items.begin(), items.end());
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

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForPlugin(CT_AbstractStepPlugin *plugin) const
{
    QList<QStandardItem*> l;

    if(!isTypeVisible(IT_Plugin))
        return l;

    QString pluginName = m_pluginManager.getPluginName(plugin);

    if (pluginName.startsWith("plug_"))
        pluginName.remove(0, 5);

    QStandardItem *item = new QStandardItem(pluginName);
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)plugin), DR_Pointer);
    item->setData((int)IT_Plugin, DR_Type);
    l << item;

    return l;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForSeparator(CT_StepSeparator *sep) const
{
    QList<QStandardItem*> l;

    if(isTypeVisible(IT_SeparatorG) && !sep->getTitle().isEmpty()) {
        QStandardItem *item = new QStandardItem(sep->getTitle());
        item->setEditable(false);
        item->setData(qVariantFromValue((void*)sep), DR_Pointer);
        item->setData((int)IT_SeparatorG, DR_Type);
        l << item;
    }

    return l;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForSeparator(CT_StepLoadFileSeparator *sep) const
{
    QList<QStandardItem*> l;

    if(isTypeVisible(IT_SeparatorLF) && !sep->typeOfFile().isEmpty()) {
        QStandardItem *item = new QStandardItem(sep->typeOfFile());
        item->setEditable(false);
        item->setData(qVariantFromValue((void*)sep), DR_Pointer);
        item->setData((int)IT_SeparatorLF, DR_Type);
        l << item;
    }

    return l;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForSeparator(CT_StepCanBeAddedFirstSeparator *sep) const
{
    QList<QStandardItem*> l;

    if(isTypeVisible(IT_SeparatorCBAF) && !sep->getTitle().isEmpty()) {
        QStandardItem *item = new QStandardItem(sep->getTitle());
        item->setEditable(false);
        item->setData(qVariantFromValue((void*)sep), DR_Pointer);
        item->setData((int)IT_SeparatorCBAF, DR_Type);
        l << item;
    }

    return l;
}

QList<QStandardItem *> DM_StepsFromPluginsModelConstructor::createItemsForStep(CT_VirtualAbstractStep *step, ItemType stepType) const
{
    QList<QStandardItem*> l;

    QStandardItem *item = new QStandardItem(step->getPlugin()->getKeyForStep(*step));
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)step), DR_Pointer);
    item->setData((int)stepType, DR_Type);
    l << item;

    return l;
}

template<typename SEPTYPE, typename STEPTYPE>
void DM_StepsFromPluginsModelConstructor::createAndAddItems(const QList<SEPTYPE *> &l, QStandardItem *baseSeparatorItem, QStandardItem *rootItem, ItemType stepType)
{
    foreach (SEPTYPE *sep, l) {

        QList<QStandardItem*> itemsSep = createItemsForSeparator(sep);
        QStandardItem *parentItem = baseSeparatorItem;

        if(!itemsSep.isEmpty())
            parentItem = itemsSep.first();

        if(parentItem == NULL)
            parentItem = rootItem;

        QList<STEPTYPE*> steps = sep->getStepList();

        foreach (STEPTYPE *step, steps) {
            QList<QStandardItem*> items2 = createItemsForStep(step, stepType);

            if(!items2.isEmpty())
                parentItem->appendRow(items2);
        }

        if(!itemsSep.isEmpty() && (parentItem == itemsSep.first()) && (parentItem->rowCount() > 0))
            rootItem->appendRow(itemsSep);
        else
            qDeleteAll(itemsSep.begin(), itemsSep.end());
    }
}
