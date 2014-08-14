#include "ct_outstditemattributemodel.h"

CT_OutStdItemAttributeModel::CT_OutStdItemAttributeModel(const QString &uniqueName,
                                                         CT_AbstractItemAttribute *attribute,
                                                         const QString &displayableName,
                                                         const QString &description) : CT_OutAbstractItemAttributeModel(uniqueName,
                                                                                                                                 description,
                                                                                                                                 displayableName,
                                                                                                                                 attribute)
{
}

CT_OutAbstractModel* CT_OutStdItemAttributeModel::copy() const
{
    CT_OutStdItemAttributeModel *cpy = new CT_OutStdItemAttributeModel(uniqueName(),
                                                                     NULL,
                                                                     displayableName(),
                                                                     description());

    if(itemAttribute() != NULL)
        cpy->setItemAttribute(itemAttribute()->copy(cpy, result()));

    cpy->setOriginalModel(this);

    return cpy;
}
