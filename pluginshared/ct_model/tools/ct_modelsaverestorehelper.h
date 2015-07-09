#ifndef CT_MODELSAVERESTOREHELPER_H
#define CT_MODELSAVERESTOREHELPER_H

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

/**
 * @brief Use this class to save a model in a SettingsGroup. You can use this class only if you save all elements (step, result, etc...) in a script
 *        and reload the script. The model of items and result must have the same uniqueNamePlusTurn to be founded.
 */
class PLUGINSHAREDSHARED_EXPORT CT_ModelSaveRestoreHelper
{
public:
    CT_ModelSaveRestoreHelper();

    /**
     * @brief Returns all values that must be saved to search an out model if you reload your script.
     * @param model : the model to save
     * @param groupName : the name of the root group (SettingsNodeGroup) that will be returned
     * @return A SettingsNodeGroup that contains all information needed to search the model when you reload the application. NULL if
     *         the model is not valid (not linked to a step and a result).
     */
    SettingsNodeGroup* saveToSearchOutModel(const CT_OutAbstractModel *model, const QString &groupName);

    /**
     * @brief Returns the model saved previously in a SettingsNodeGroup. The model of items and result must have the same uniqueNamePlusTurn to be founded.
     * @warning You can use this method only if you have saved all elements (step, result, etc...) in a script and you have reloaded the script.
     * @param node : the SettingsNodeGroup that contains all information to search the model
     * @param step : the step where to begin the research.
     * @return The model if it was founded. NULL if the SettingsNodeGroup contains invalid information or if the model was not founded.
     */
    CT_OutAbstractModel* searchModelFromSettings(const SettingsNodeGroup *node, const CT_VirtualAbstractStep *step);
};

#endif // CT_MODELSAVERESTOREHELPER_H
