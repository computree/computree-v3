#ifndef GSTEPVIEWDEFAULT_H
#define GSTEPVIEWDEFAULT_H

#include <QWidget>
#include "view/Steps/gstepstreeviewdefault.h"
#include "view/Steps/dm_steptreeviewdefaultproxymodel.h"

namespace Ui {
class GStepViewDefault;
}

/**
 * @brief This class is a Widget that simplify the use of a treeview of steps that was available in a list of plugins. The user
 *        can set what to show or not and how to show it.
 */
class GStepViewDefault : public QWidget
{
    Q_OBJECT

public:

    enum DisplayNameConfig {
        DNC_Key = 1,
        DNC_StepName = 2,
        DNC_StepDisplayableName = 4
    };

    Q_DECLARE_FLAGS(DisplayNameConfigs, DisplayNameConfig)

    explicit GStepViewDefault(QWidget *parent = 0);
    ~GStepViewDefault();

    void init(const CDM_PluginManager &pManager);

    /**
     * @brief Returns the proxy used by the treeview
     */
    DM_StepTreeViewDefaultProxyModel* proxy() const;

    /**
     * @brief Returns the treeview used in this view
     */
    GStepsTreeViewDefault* treeView() const;

    /**
     * @brief Returns the constructor used to construct the model.
     */
    DM_StepsFromPluginsModelConstructor* constructor() const;

    /**
     * @brief Change the display configuration
     */
    void setDisplayConfiguration(DisplayNameConfigs configs);

    /**
     * @brief Set a context menu to a type defined in parameter
     */
    void setContextMenuOnType(QMenu *contextMenu, DM_StepsFromPluginsModelConstructor::ItemType type);

    /**
     * @brief Return the current step selected or NULL if it was not a step
     */
    CT_VirtualAbstractStep* currentStepSelected() const;

    /**
     * @brief Return the current level selected or NULL if it was not a level
     */
    CT_MenuLevel* currentLevelSelected() const;

public slots:
    void reconstruct();

    /**
     * @brief Search a step by it's name : displayable or not, extended or custom, etc... and expand the parent to let the user
     *        show immediately the step
     * @param anyName : the stepName per example the extended or the custom or the displable, etc...
     * @return true : if the step was found
     */
    bool searchStepByNameAndExpandParent(const QString &anyName);

private:
    Ui::GStepViewDefault                                            *ui;
    CDM_PluginManager                                               *m_pluginManager;
    DM_StepsFromPluginsModelConstructor                             *m_constructor;
    DM_StepTreeViewDefaultProxyModel                                *m_proxy;
    DisplayNameConfigs                                              m_nameConfig;
    QMap<DM_StepsFromPluginsModelConstructor::ItemType, QMenu*>     m_contextMenus; // must be sorted in ascending order !

    bool recursiveSearchStepByNameAndExpandParent(const QModelIndex &index, const QString &anyName);

    void resizeColumnsOfTreeView();

    QMenu* contextMenuForType(DM_StepsFromPluginsModelConstructor::ItemType type);

protected:

    void resizeEvent(QResizeEvent *e);

private slots:
    void on_pushButtonConfigStepName_clicked();

    void setStepName(bool enable);

    static bool staticStepsName(QString &name, const QModelIndex &index, void *context);

    void indexDoubleClicked(const QModelIndex &index);

    /**
     * @brief Called from treeView when the user right click
     */
    void showTreeViewContextMenu(const QPoint &point);

    void selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection);

signals:
    void levelSelected(CT_MenuLevel *level);
    void stepSelected(CT_VirtualAbstractStep *step);
    void stepDoubleClicked(CT_VirtualAbstractStep *step);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GStepViewDefault::DisplayNameConfigs)

#endif // GSTEPVIEWDEFAULT_H
