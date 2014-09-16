#ifndef CT_OUTSTDGROUPMODEL_H
#define CT_OUTSTDGROUPMODEL_H

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

#include "ct_itemdrawable/ct_standarditemgroup.h"

/**
 * @brief Use this class if you will inform that you want create a group and add it to an
 *        other OUTPUT model that represent a group or a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutStdGroupModel : public CT_OutAbstractGroupModel
{
    Q_OBJECT

public:
    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param group : the real group that you will create in the method "compute" of your step. MUST NOT BE NULL !
     * @param displayableName : a displayable name
     * @param description : a description
     */
    CT_OutStdGroupModel(const QString &uniqueName,
                        CT_AbstractItemGroup *group = new CT_StandardItemGroup(),
                        const QString &displayableName = "",
                        const QString &description = "");

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const;

    /**
     * @brief Returns a copy of this model
     */
    virtual CT_OutAbstractModel* copy() const;

    // HERITE DE CT_AbstractGroupModelT //
    virtual DEF_CT_AbstractGroupModelOut* copyGroup() const;

protected:

    /**
     * @brief Inherit this method if you do not want to overload the "copy" method and returns
     *        a empty copy of your model. The "copy" method will add copy of groups and items in your copy.
     */
    virtual CT_OutStdGroupModel* newInstance() const;
};

#endif // CT_OUTSTDGROUPMODEL_H
