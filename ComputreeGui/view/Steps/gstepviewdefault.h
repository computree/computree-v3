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
        DNC_StepKey = 1,
        DNC_StepShortDescription = 2,
        DNC_StepDisplayableName = 4
    };

    Q_DECLARE_FLAGS(DisplayNameConfigs, DisplayNameConfig)

    explicit GStepViewDefault(QWidget *parent = 0);
    ~GStepViewDefault();

    /**
     * @brief Init the view. Call it after the constructor or you will see nothing.
     */
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
     * @brief Returns the display configuration
     */
    DisplayNameConfigs displayConfiguration() const;

    /**
     * @brief Set if the checkbox is visible or not
     */
    void setShowAtLastPositionCheckboxVisible(bool e);

    /**
     * @brief Set if must show this widget at last position (don't do anything just check the checkbox or not)
     * @warning You must manage the functionnality yourself
     */
    void setShowAtLastPosition(bool v);

    /**
     * @brief Returns true if the checkbox is checked
     */
    bool showAtLastPosition() const;

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

    /**
     * @brief Return the configuration used to display the name of steps
     */
    DisplayNameConfigs stepNameConfiguration() const;

    /**
     * @brief Return the name of the step by using the configuration passed in parameter
     */
    static QString staticGetStepNameFromConfiguration(CT_VirtualAbstractStep *step, const DisplayNameConfigs &config);

public slots:
    /**
     * @brief Reconstruct the model of the tree view
     */
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

    /**
     * @brief Search a step recursively by it's name : displayable or not, extended or custom, etc... and expand the parent to let the user
     *        show immediately the step
     */
    bool recursiveSearchStepByNameAndExpandParent(const QModelIndex &index, const QString &anyName);

    /**
     * @brief Resize column of tree view in function of the size of the treeview and the maximum size
     *        of text that was in the last column. So the last column will always have the good size to
     *        show the entire maximum text.
     */
    void resizeColumnsOfTreeView();

    /**
     * @brief Returns the max size of the text contained in the column index passed in parameter (recursively)
     */
    void getMaxSizeOfColumnsRecursively(QStandardItem *item, const QString &text, const QFontMetrics &fm, const int &columnIndex, int &maxSize);

    /**
     * @brief Returns the context menu for the type passed in parameter
     */
    QMenu* contextMenuForType(DM_StepsFromPluginsModelConstructor::ItemType type);

    /**
     * @brief The function that returns the name of the step to show
     */
    static bool staticStepsName(QString &name, const QModelIndex &index, void *context);

protected:

    /**
     * @brief Overloaded to resize column of the treeview when the size changed
     */
    void resizeEvent(QResizeEvent *e);

private slots:

    /**
     * @brief Called when the user click on the push button that will configure the name of step shown
     */
    void on_pushButtonConfigStepName_clicked();

    /**
     * @brief Called when the user click on the push button that will configure the filter
     */
    void on_pushButtonConfigSearch_clicked();

    /**
     * @brief Called when the user change the text of the filter
     */
    void on_lineEditSearch_textChanged(const QString &text);

    /**
     * @brief Called by a QAction in the menu that show configuration of step name
     */
    void setStepName(bool enable);

    /**
     * @brief Called by a QAction in the menu that show configuration of filter
     */
    void setSearchConfiguration(bool enable);

    /**
     * @brief Called when a use double click on a row in the tree view
     */
    void indexDoubleClicked(const QModelIndex &index);

    /**
     * @brief Called from treeView when the user right click
     */
    void showTreeViewContextMenu(const QPoint &point);

    /**
     * @brief Called when the selection changed in the treeview
     */
    void selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection);

signals:
    /**
     * @brief Emitted when a level is selected
     */
    void levelSelected(CT_MenuLevel *level);

    /**
     * @brief Emitted when a step is selected
     */
    void stepSelected(CT_VirtualAbstractStep *step);

    /**
     * @brief Emitted when a step is double clicked
     */
    void stepDoubleClicked(CT_VirtualAbstractStep *step);

    /**
     * @brief Emmited when the configuration of the display of name for step has changed
     */
    void displayNameConfigurationChanged(GStepViewDefault::DisplayNameConfigs c);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GStepViewDefault::DisplayNameConfigs)

#endif // GSTEPVIEWDEFAULT_H
