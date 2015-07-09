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
    explicit DM_StepTreeViewDefaultProxyModel(QObject *parent = 0);
    ~DM_StepTreeViewDefaultProxyModel();

    typedef bool (*functionGetStepName)(QString &, const QModelIndex&, void*);

    /**
     * @brief Set if the type passed in parameter must be visible or not in the view.
     * @warning If the type passed in parameter is not visible, childrens are not visible too !
     */
    void setTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type, bool enable);

    /**
     * @brief Returns if type passed in parameter is visible or not in the view
     */
    bool isTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type) const;

    /**
     * @brief Personalize the value for the role and type passed in parameter
     * @param type : the type to personalize
     * @param role : the role to personalize
     * @param val : the value to use, can be invalid to use default returned by QSortFilterProxyModel
     */
    void setValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::ItemType type, Qt::ItemDataRole role, QVariant val);

    /**
     * @brief Returns the personalized value for the role and type passed in parameter
     */
    QVariant getValueForTypeAndRole(DM_StepsFromPluginsModelConstructor::ItemType type, Qt::ItemDataRole role) const;

    /**
     * @brief Set the function to call to get the name of steps
     */
    void setStepsNameFunction(functionGetStepName f, void *context);

    /**
     * @brief Set if steps that were not compatible with the parent step (method "setParentStep") must be visible in the view
     * @param enable : true to view steps not compatible in "disabled mode", false to remove steps not compatible from the view
     */
    void setShowStepNotCompatible(bool enable);

    /**
     * @brief Set the parent step to use to check if a step can be added after this step or not. Can be NULL if you don't want to check
     *        and shows all steps.
     * @param parent : the step that will used to check if another step can be added after it
     */
    void setParentStep(CT_VirtualAbstractStep *parent);

    /**
     * @brief Returns the personalized value of the index for the role passed in parameter
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

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
    functionGetStepName                                                                         m_stepsNameF;
    void*                                                                                       m_stepsNameContext;

    /**
     * @brief Check recursively if this row is accepted.
     * @param index : index to check recursively
     * @return false if it was not accepted (because it's empty or just filtered)
     */
    bool recursiveAcceptRow(const QModelIndex &index) const;

    /**
     * @brief Check if the step is accepted
     * @param index : index that contains the step and other datas
     * @return true if the step is accepted (depending of parameters setShowXXXStep && setShowStepNotCompatible), false otherwise
     */
    bool acceptStep(const QModelIndex &index) const;

signals:

public slots:

};

#endif // DM_STEPTREEVIEWDEFAULTPROXYMODEL_H
