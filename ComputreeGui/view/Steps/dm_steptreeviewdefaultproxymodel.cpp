#include "dm_steptreeviewdefaultproxymodel.h"

DM_StepTreeViewDefaultProxyModel::DM_StepTreeViewDefaultProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setShowStepNotCompatible(true);
    setParentStep(NULL);
    setPluginsFilterEnabled(true);

    setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::IT_RootLevel, Qt::BackgroundRole, QColor(64, 64, 64));
    setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::IT_Step, Qt::BackgroundRole, QColor(175, 171, 171));
    setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::IT_SubLevel, Qt::BackgroundRole, QColor(91, 155, 213));

    setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::IT_All, Qt::ForegroundRole, QColor(255, 255, 255));

    //setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::IT_All, Qt::TextAlignmentRole, Qt::AlignCenter);

    setStepsNameFunction(NULL, NULL);
}

DM_StepTreeViewDefaultProxyModel::~DM_StepTreeViewDefaultProxyModel()
{
    qDeleteAll(m_values.begin(), m_values.end());
}

void DM_StepTreeViewDefaultProxyModel::setTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type, bool enable)
{
    if(enable) {
        m_typesNotVisible.removeAll(type);
        return;
    }

    m_typesNotVisible.append(type);

    invalidate();
}

bool DM_StepTreeViewDefaultProxyModel::isTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type) const
{
    int i = m_typesNotVisible.indexOf(type);

    if(i != -1)
        return false;

    QListIterator<DM_StepsFromPluginsModelConstructor::ItemType> it(m_typesNotVisible);

    while(it.hasNext()) {
        if(it.next() & type)
            return false;
    }

    return true;
}

void DM_StepTreeViewDefaultProxyModel::setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::ItemType type,
                                                              Qt::ItemDataRole role,
                                                              QVariant val)
{
    QHash<Qt::ItemDataRole, QVariant> *hash = m_values.value(type, NULL);

    if(hash == NULL) {
        hash = new QHash<Qt::ItemDataRole, QVariant>();
        m_values.insert(type, hash);
    }

    hash->insert(role, val);
}

QVariant DM_StepTreeViewDefaultProxyModel::getValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::ItemType type, Qt::ItemDataRole role) const
{
    QVariant v;

    QHash<Qt::ItemDataRole, QVariant> *hash = m_values.value(type, NULL);

    if(hash != NULL) {
        v = hash->value(role, QVariant());

        if(v.isValid())
            return v;
    }

    QMapIterator<DM_StepsFromPluginsModelConstructor::ItemType, QHash<Qt::ItemDataRole, QVariant> *> it(m_values);

    while(it.hasNext()) {
        it.next();

        if((it.key() != type) && (it.key() & type)) {
            v = it.value()->value(role, QVariant());

            if(v.isValid())
                return v;
        }
    }

    return v;
}

void DM_StepTreeViewDefaultProxyModel::setStepsNameFunction(functionGetStepName f, void *context)
{
    m_stepsNameF = f;
    m_stepsNameContext = context;
}

void DM_StepTreeViewDefaultProxyModel::setUseStepsOfPlugins(const QList<CT_AbstractStepPlugin *> &plugins)
{
    m_plugins = plugins;
    setPluginsFilterEnabled(true);
}

void DM_StepTreeViewDefaultProxyModel::setPluginsFilterEnabled(bool enable)
{
    m_pluginsFilterEnable = enable;

    invalidate();
}

void DM_StepTreeViewDefaultProxyModel::setShowStepNotCompatible(bool enable)
{
    m_showNotCompatible = enable;

    invalidate();
}

void DM_StepTreeViewDefaultProxyModel::setParentStep(CT_VirtualAbstractStep *parent)
{
    m_parentStep = parent;

    invalidate();
}

QVariant DM_StepTreeViewDefaultProxyModel::data(const QModelIndex &index, int role) const
{
    if(role < Qt::UserRole + 1) {
        DM_StepsFromPluginsModelConstructor::ItemType type = (DM_StepsFromPluginsModelConstructor::ItemType)index.data(DM_StepsFromPluginsModelConstructor::DR_Type).toInt();

        if((m_stepsNameF != NULL) && (role == Qt::DisplayRole) && (type & DM_StepsFromPluginsModelConstructor::IT_Step)) {

            QString str;

            if((*m_stepsNameF)(str, index, m_stepsNameContext))
                return str;
        }

        QVariant v = getValueForTypeAndRole((DM_StepsFromPluginsModelConstructor::ItemType)index.data(DM_StepsFromPluginsModelConstructor::DR_SecondaryType).toInt(), (Qt::ItemDataRole)role);

        if(v.isValid())
            return v;
    }

    return QSortFilterProxyModel::data(index, role);
}

bool DM_StepTreeViewDefaultProxyModel::pluginsFilterEnabled() const
{
    return m_pluginsFilterEnable;
}

QList<CT_AbstractStepPlugin *> DM_StepTreeViewDefaultProxyModel::pluginsForFiltering() const
{
    return m_plugins;
}

bool DM_StepTreeViewDefaultProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel() != NULL) {

        if(!recursiveAcceptRow(sourceModel()->index(source_row, 0, source_parent)))
            return false;
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool DM_StepTreeViewDefaultProxyModel::recursiveAcceptRow(const QModelIndex &index) const
{
    if(index.isValid())
    {
        int s = sourceModel()->rowCount(index);

        if(s > 0) {
            int i = 0;
            bool notEmpty = false;

            while((i < s) && !notEmpty) {
                if(recursiveAcceptRow(index.child(i, 0)))
                    notEmpty = true;

                ++i;
            }

            if(!notEmpty)
                return false;
        }

        DM_StepsFromPluginsModelConstructor::ItemType type = (DM_StepsFromPluginsModelConstructor::ItemType)index.data(DM_StepsFromPluginsModelConstructor::DR_SecondaryType).toInt();

        if(!isTypeVisible(type))
            return false;

        if(type & (int)DM_StepsFromPluginsModelConstructor::IT_Step)
        {
            if(!acceptStep(index))
                return false;
        }

        return true;
    }

    return false;
}

bool DM_StepTreeViewDefaultProxyModel::acceptStep(const QModelIndex &index) const
{
    CT_VirtualAbstractStep *step = (CT_VirtualAbstractStep*)index.data(DM_StepsFromPluginsModelConstructor::DR_Pointer).value<void*>();

    if(pluginsFilterEnabled()) {
        if(!m_plugins.contains(step->getPlugin()))
            return false;
    }

    bool acceptAfterStep = true;

    if(m_parentStep == NULL) {
        if(step->needInputResults())
            acceptAfterStep = false;
    } else {
        acceptAfterStep = step->acceptAddAfterThisStep(m_parentStep);
    }

    if(m_showNotCompatible && !acceptAfterStep)
        return true;

    return acceptAfterStep;
}
