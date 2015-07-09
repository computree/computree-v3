#ifndef DM_STEPSFROMPLUGINSMODELCONSTRUCTOR_H
#define DM_STEPSFROMPLUGINSMODELCONSTRUCTOR_H

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "cdm_pluginmanager.h"
#include "ct_abstractstepplugin.h"

#include <QStandardItemModel>
/**
 * @brief Use this class to construct a tree (QStandardItemModel) of steps available in a list of plugins
 */
class DM_StepsFromPluginsModelConstructor
{
public:
    enum DataRole {
        DR_Type = Qt::UserRole + 1,             // use this data role index to get the type of pointer contained in this item => data(DR_Type) return an int that was a ItemType
        DR_Pointer = Qt::UserRole + 2           // use this data role index to get a pointer to a "XXX", check type of pointer with DR_Type to get an ItemType
    };

    enum ItemType {
        IT_Plugin = 1,                          // call data(DR_Type) and check if return == IT_Plugin to check if the pointer is "CT_AbstractStepPlugin*"

        // IT_StepG, IT_StepLF and IT_StepCBAF is IT_Step

            IT_StepG = 2,                       // call data(DR_Type) and check if return && IT_StepG to check if the pointer is "CT_VirtualAbstractStep*"
            IT_StepLF = 4,                      // call data(DR_Type) and check if return && IT_StepLF to check if the pointer is "CT_AbstractStepLoadFile*"
            IT_StepCBAF = 8,                    // call data(DR_Type) and check if return && IT_StepCBAF to check if the pointer is "CT_AbstractStepCanBeAddedFirst*"

        IT_Step = 14,                           // call data(DR_Type) and check if return && IT_Step to check if the pointer is "CT_VirtualAbstractStep*"


        // IT_SeparatorG, IT_SeparatorLF, IT_SeparatorCBAF and IT_SeparatorOther is IT_SeparatorOldVersion and is IT_Separator

                IT_SeparatorG = 16,             // call data(DR_Type) and check if return && IT_SeparatorG to check if the pointer is "CT_StepSeparator*"
                IT_SeparatorLF = 32,            // call data(DR_Type) and check if return && IT_SeparatorLF to check if the pointer is "CT_StepLoadFileSeparator*"
                IT_SeparatorCBAF = 64,          // call data(DR_Type) and check if return && IT_SeparatorCBAF to check if the pointer is "CT_StepCanBeAddedFirstSeparator*"
                IT_SeparatorOther = 128,        // call data(DR_Type) and check if return && IT_SeparatorOther to check if the separator is just text (Autres)

            IT_SeparatorOldVersion = 240,       // call data(DR_Type) and check if return && IT_SeparatorOldVersion to check if the pointer is a separator of type IT_SeparatorG or IT_SeparatorLF or IT_SeparatorCBAF

        // IT_SeparatorNewVersion is IT_Separator

            IT_SeparatorNewVersion = 256,       // call data(DR_Type) and check if return && IT_SeparatorNewVersion to check if the separator is a new version (the new version is not a pointer to a separator, is just text)

        IT_Separator = 496,                     // call data(DR_Type) and check if return && IT_Separator to check if the pointer is a separator (be carreful that you can not get a pointer for IT_SeparatorNewVersion)

        IT_Text = 512,                          // no pointer available, just text

        IT_All =  1023                          // all types
    };

    DM_StepsFromPluginsModelConstructor(const CDM_PluginManager &manager);
    ~DM_StepsFromPluginsModelConstructor();

    /**
     * @brief Set plugins that will be used to create the tree
     * @param plugins : plugins to use
     */
    void setUseStepsOfPlugins(const QList<CT_AbstractStepPlugin*> &plugins);

    /**
     * @brief Set if the model must contains rows for type passed in parameter or not. if the type is not visible but it contains children, these latter
     *        will still be added to the model, but not the type of course.
     *
     * @param type : the type to set visible or not. If you pass IT_Separator and set to false : all separators like IT_SeparatorG, IT_SeparatorLF, etc... will not be added to the model
     * @param enable : true to add type in model (true by default for all types), false otherwise (the row for type is not added but childrens was added)
     */
    void setTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type, bool enable);

    /**
     * @brief Returns true if type passed in parameter will be added to the model, false otherwise. If you pass IT_SeparatorG and IT_Separator has been set to false the method
     *        will return false because IT_SeparatorG is a IT_Separator.
     */
    bool isTypeVisible(DM_StepsFromPluginsModelConstructor::ItemType type) const;

    /**
     * @brief Reset the configuration to default. After call of this method you must set the types visible, the plugins to use, etc...
     * @warning The model will not be cleared by this method.
     */
    void resetConfig();

    /**
     * @brief (Re)construct the tree. The tree will only have names of plugin and steps. But the pointer to the plugin or to the step can be
     *         get with a call to method "data(...)" to a QStandardItem (show method "model" to get more information)
     */
    void construct();

    /**
     * @brief Returns the model constructed
     *
     * The model is a tree of QStandardItem that contains such information in "data()" method that you can extract to personalize the view.
     * Use enum "DM_StepsFromPluginsModelConstructor::DataRole" to get the index of the data to get and the enum "DM_StepsFromPluginsModelConstructor::ItemType"
     * to get the type of item if you call "DM_StepsFromPluginsModelConstructor::data(DR_Type)"
     */
    QStandardItemModel* model() const;

    /**
     * @brief Convert an index to a step if this index contains a step, otherwise returns NULL.
     */
    CT_VirtualAbstractStep* stepFromIndex(const QModelIndex &index) const;

private:
    const CDM_PluginManager         &m_pluginManager;
    QList<CT_AbstractStepPlugin*>   m_plugins;
    QStandardItemModel              *m_model;
    QList<ItemType>                 m_notVisible;

    template<typename SEPTYPE, typename STEPTYPE>
    void createAndAddItems(const QList<SEPTYPE*> &l, QStandardItem *baseSeparatorItem, QStandardItem *rootItem, ItemType stepType);

    /**
     * @brief Create and return a list of QStandardItem that will be added to the tree for a plugin
     */
    QList<QStandardItem*> createItemsForPlugin(CT_AbstractStepPlugin *plugin) const;

    /**
     * @brief Create and return a list of QStandardItem that will be added to the tree for a separator
     */
    QList<QStandardItem*> createItemsForSeparator(CT_StepSeparator *sep) const;

    /**
     * @brief Create and return a list of QStandardItem that will be added to the tree for a separator
     */
    QList<QStandardItem*> createItemsForSeparator(CT_StepLoadFileSeparator *sep) const;

    /**
     * @brief Create and return a list of QStandardItem that will be added to the tree for a separator
     */
    QList<QStandardItem*> createItemsForSeparator(CT_StepCanBeAddedFirstSeparator *sep) const;

    /**
     * @brief Create and return a list of QStandardItem that will be added to the tree for a step
     */
    QList<QStandardItem*> createItemsForStep(CT_VirtualAbstractStep *step, ItemType stepType) const;
};

#endif // DM_STEPSFROMPLUGINSMODELCONSTRUCTOR_H
