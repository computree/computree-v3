#ifndef DM_STEPTREEVIEWDEFAULTPROXYMODEL_H
#define DM_STEPTREEVIEWDEFAULTPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "dm_stepsfrompluginsmodelconstructor.h"

/**
 * @brief Use this proxy model with a model constructed by "DM_StepsFromPluginsModelConstructor".
 *        This class can personnalize the QVariant(color, font type, alignement, etc...) of plugin/step/etc... in the view (method setValueForTypeAndRole)
 *        This class can filter the model to show only elements of certains type (method setTypeVisible)
 *        This class can set visible or not steps that was not compatible with a parent step (methods setShowStepNotCompatible and setParentStep)
 */
class DM_StepTreeViewDefaultProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

    enum FilterConfig {
        FC_StepKey = 1,
        FC_StepShortDescription = 2,
        FC_StepFullDescription = 4,
        FC_StepDisplayableName = 8
    };

    Q_DECLARE_FLAGS(FilterConfigs, FilterConfig)

    explicit DM_StepTreeViewDefaultProxyModel(QObject *parent = 0);
    ~DM_StepTreeViewDefaultProxyModel();

    typedef bool (*functionGetStepName)(QString &, const QModelIndex&, void*);

    /**
     * @brief Returns if type passed in parameter is visible or not in the view
     */
    bool isTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type) const;

    /**
     * @brief Returns the personalized value for the role and type passed in parameter
     */
    QVariant getValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::ItemType type, Qt::ItemDataRole role) const;

    /**
     * @brief Returns the personalized value of the index for the role passed in parameter
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief Returns true if the plugin filter is enabled
     */
    bool pluginsFilterEnabled() const;

    /**
     * @brief Returns the list of plugins used for filtering steps
     */
    QList<CT_AbstractStepPlugin*> pluginsForFiltering() const;

    /**
     * @brief Set the function to call to get the name of steps
     */
    void setStepsNameFunction(functionGetStepName f, void *context);

    /**
     * @brief Returns the configuration of the filter
     */
    FilterConfigs filterConfiguration() const;

public slots:
    /**
     * @brief The list of plugins you set will be used to remove steps that was not in plugin list. If you call this
     *        method the plugins filter will be enabled by default (setPluginsFilterEnabled(true))
     */
    void setUseStepsOfPlugins(const QList<CT_AbstractStepPlugin*> &plugins);

    /**
     * @brief Enable or not the plugins filters (setUseStepsOfPlugins)
     */
    void setPluginsFilterEnabled(bool enable);

    /**
     * @brief Set if the type passed in parameter must be visible or not in the view.
     * @warning If the type passed in parameter is not visible, childrens are not visible too !
     */
    void setTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type, bool enable);

    /**
     * @brief Personalize the value for the role and type passed in parameter
     * @param type : the type to personalize
     * @param role : the role to personalize
     * @param val : the value to use, can be invalid to use default returned by QSortFilterProxyModel
     */
    void setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::ItemType type, Qt::ItemDataRole role, QVariant val);

    /**
     * @brief Set if steps that were not compatible with the parent step (method "setParentStep") must be visible in the view
     * @param enable : true to view steps not compatible in "disabled mode", false to remove steps not compatible from the view
     */
    void setShowStepNotCompatible(bool enable);

    /**
     * @brief Change the filter configuration
     */
    void setFilterConfiguration(FilterConfigs configs);

    /**
     * @brief Set the parent step to use to check if a step can be added after this step or not. Can be NULL if you don't want to check
     *        and shows all steps.
     * @param parent : the step that will used to check if another step can be added after it
     */
    void setParentStep(CT_VirtualAbstractStep *parent);

protected:
    /**
     * @brief Check if the row is accepted or not
     */
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

private:
    bool                                                                                        m_showNotCompatible;
    CT_VirtualAbstractStep                                                                      *m_parentStep;
    QList<DM_StepsFromPluginsModelConstructor::ItemType>                                        m_typesNotVisible;
    QMap<DM_StepsFromPluginsModelConstructor::ItemType, QHash<Qt::ItemDataRole, QVariant>* >    m_values;   // must be sorted in ascendant order
    QList<CT_AbstractStepPlugin*>                                                               m_plugins;
    bool                                                                                        m_pluginsFilterEnable;
    functionGetStepName                                                                         m_stepsNameF;
    void*                                                                                       m_stepsNameContext;
    FilterConfigs                                                                               m_filterConfig;
    QFont                                                                                       m_manualStepFont;

    /**
     * @brief Check recursively if this row is accepted.
     * @param index : index to check recursively
     * @return false if it was not accepted (because it's empty or just filtered)
     */
    bool recursiveAcceptRow(const QModelIndex &index) const;

    /**
     * @brief Returns true if the index is the type passed in parameter. The datarole can be specified if you want to use another.
     */
    bool isIndexOfType(const QModelIndex &index, DM_StepsFromPluginsModelConstructor::ItemType type, DM_StepsFromPluginsModelConstructor::DataRole dataRole = DM_StepsFromPluginsModelConstructor::DR_Type) const;

    /**
     * @brief Returns the step from the index passed in parameter.
     * @warning This method don't test if the index was a DM_StepsFromPluginsModelConstructor::IT_Step !
     */
    CT_VirtualAbstractStep* getStepFromIndex(const QModelIndex &index) const;

    /**
     * @brief Returns the type of the index passed in parameter. The datarole can be specified if you want to use another.
     */
    DM_StepsFromPluginsModelConstructor::ItemType getTypeFromIndex(const QModelIndex &index, DM_StepsFromPluginsModelConstructor::DataRole dataRole = DM_StepsFromPluginsModelConstructor::DR_Type) const;

    /**
     * @brief Check if the step is accepted
     * @param index : index that contains the step and other datas
     * @return true if the step is accepted (depending of parameters setShowXXXStep && setShowStepNotCompatible), false otherwise
     */
    bool acceptStep(const QModelIndex &index, bool usePluginFiltered) const;

    /**
     * @brief Return true if the index that represent a step exist in the plugin collection (m_pluginsFilterEnable)
     * @warning This method don't test if the index was a DM_StepsFromPluginsModelConstructor::IT_Step !
     */
    bool existStepInPluginCollection(const QModelIndex &index) const;

    /**
     * @brief Return true if the index that represent a step is compatible with the parent step (m_parentStep)
     * @warning This method don't test if the index was a DM_StepsFromPluginsModelConstructor::IT_Step !
     */
    bool isStepCompatibleWithParent(const QModelIndex &index) const;

private slots:

    void parentStepDestroyed();

signals:

public slots:

};

Q_DECLARE_OPERATORS_FOR_FLAGS(DM_StepTreeViewDefaultProxyModel::FilterConfigs)

#endif // DM_STEPTREEVIEWDEFAULTPROXYMODEL_H
