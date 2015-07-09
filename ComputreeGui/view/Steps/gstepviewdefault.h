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
    Ui::GStepViewDefault                                    *ui;
    CDM_PluginManager                                       *m_pluginManager;
    DM_StepsFromPluginsModelConstructor                     *m_constructor;
    DM_StepTreeViewDefaultProxyModel                        *m_proxy;
    DisplayNameConfigs                                      m_nameConfig;

    bool recursiveSearchStepByNameAndExpandParent(const QModelIndex &index, const QString &anyName);

private slots:
    void on_pushButtonConfigTypeOnTree_clicked();
    void on_pushButtonConfigStepName_clicked();

    void setTypeVisible(bool visible);
    void setStepName(bool enable);

    static bool staticStepsName(QString &name, const QModelIndex &index, void *context);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GStepViewDefault::DisplayNameConfigs)

#endif // GSTEPVIEWDEFAULT_H
