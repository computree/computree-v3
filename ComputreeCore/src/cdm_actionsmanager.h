#ifndef CDM_ACTIONSMANAGER_H
#define CDM_ACTIONSMANAGER_H

#include "cdm_pluginmanager.h"
#include "cdm_stepmanager.h"

/**
 * @brief The actions manager is used to manage actions from step and plugins.
 *
 *        The methods "addAction(...)" and "removeAction(...)" is called from step. This
 *        impact only the list of actions from steps.
 */
class COMPUTREECORESHARED_EXPORT CDM_ActionsManager : public QObject, public ActionsManagerInterface
{
    Q_INTERFACES(ActionsManagerInterface)
    Q_OBJECT

public:
    CDM_ActionsManager();
    virtual ~CDM_ActionsManager();

    void setPluginManager(const CDM_PluginManager *pluginManager);
    CDM_PluginManager* pluginManager() const;

    void setStepManager(const CDM_StepManager *stepManager);

    /**
     * @brief Add a action. Return true if it was added.
     *
     * @warning Only call this method if you are a Step !
     */
    bool addAction(ActionInterface *action);

    /**
     * @brief Remove a action
     *
     * @warning Only call this method if you are a Step !
     */
    bool removeAction(ActionInterface *action);
    bool removeAction(const QString &uniqueName);

    /**
     * @brief Return the collection of actions (from steps + plugins)
     */
    QList<ActionInterface*> actions() const;

    /**
     * @brief Return true if the action exist in the collection (steps + plugins)
     */
    bool existAction(const ActionInterface *action) const;
    bool existAction(const QString &uniqueName) const;

    /**
     * @brief Return true if the action exist in the collection (steps + plugins). Compare
     *        the uniqueName AND the pointer adress
     */
    bool existActionCompareAddress(const ActionInterface *action) const;

    /**
     * @brief Return a action from is uniqueName
     */
    ActionInterface* action(const QString &uniqueName) const;

    /**
     * @brief Return the number of plugins loaded
     */
    int nPlugins() const;

    /**
     * @brief Return the collection of ActionsSeparator from plugin 'index'
     */
    QList<ActionsSeparator*> actionsFromPlugin(const int &index) const;

    /**
     * @brief Return the collection of actions from all plugins
     */
    QList<ActionInterface*> actionsFromPlugins() const;

    /**
     * @brief Return actions added by steps
     */
    QList<ActionInterface*> actionsFromSteps() const;

    /**
     * @brief Delete all actions added by steps
     */
    void clearActions();

private:
    QList<ActionInterface*>     m_actions;                  /*!< actions from steps */
    QList<ActionInterface*>     m_actionsAddedInManualMode; /*!< actions added in manual mode */
    CDM_PluginManager           *m_pluginManager;           /*!< plugin manager to get actions from plugins */
    bool                        m_manualModeEnabled;        /*!< if manual mode is enabled */
    Step*                       m_manualModeEnabledByStep;  /*!< the step who request the manual mode */

private slots:
    void stepRequiredManualMode(Step *step);
    void stepFinished(Step *step);

signals:

    /**
     * @brief Emitted when a action from step has been added
     */
    void actionAdded(ActionInterface *action);

    /**
     * @brief Emitted when an action from step will be deleted
     */
    void actionToRemove(ActionInterface *action);

    /**
     * @brief Emitted when a action has been added or has been deleted, or
     *        when a plugin has been loaded or unloaded
     */
    void actionsCollectionChanged();
};

#endif // CDM_ACTIONSMANAGER_H
