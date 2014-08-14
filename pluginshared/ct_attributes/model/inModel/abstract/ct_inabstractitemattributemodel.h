#ifndef CT_INABSTRACTITEMATTRIBUTEMODEL_H
#define CT_INABSTRACTITEMATTRIBUTEMODEL_H

#include "ct_attributes/model/inModel/abstract/def_ct_inabstractitemattributemodel.h"

class PLUGINSHAREDSHARED_EXPORT CT_InAbstractItemAttributeModel : public DEF_CT_InAbstractIAModel
{
public:
    /**
     * @brief Construct a model that represent a item attribute
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractItemAttributeModel(const QString &uniqueName,
                                    const QString &description,
                                    const QString &displayableName);
};

#endif // CT_INABSTRACTITEMATTRIBUTEMODEL_H
