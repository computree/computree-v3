#ifndef CT_INSTDITEMATTRIBUTEMODEL_H
#define CT_INSTDITEMATTRIBUTEMODEL_H

#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"

/**
 * @brief Use this class if you want to search a item attribute in a OUTPUT model
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdItemAttributeModel : public CT_InAbstractItemAttributeModel
{
public:
    /**
     * @brief Construct a model that represent a item attribute
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InStdItemAttributeModel(const QString &uniqueName,
                               const QString &description,
                               const QString &displayableName);

    /**
     * @brief Returns a copy of this model and its children.
     * @param withPossibilities : true if you want to copy possibilities in the copy of the model
     */
    CT_InAbstractModel* copy(bool withPossibilities) const;

    /**
     * @brief Return true if this model can be compared with the OUTPUT model passed in parameter
     */
    bool canBeComparedWith(const CT_OutAbstractModel &model) const;

protected:

    /**
     * @brief Must return true if the model passed in parameter can be added in possibilities
     */
    bool canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const;
};

#endif // CT_INSTDITEMATTRIBUTEMODEL_H
