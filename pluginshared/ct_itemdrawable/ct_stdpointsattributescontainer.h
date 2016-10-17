#ifndef CT_STDPOINTSATTRIBUTESCONTAINER_H
#define CT_STDPOINTSATTRIBUTESCONTAINER_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.h"

/**
 *  This class is a standard container for points attributes
 */
class PLUGINSHAREDSHARED_EXPORT CT_StdPointsAttributesContainer : public CT_AbstractPointsAttributesContainer
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StdPointsAttributesContainer, CT_AbstractPointsAttributesContainer, Point attributes list)

public:
    CT_StdPointsAttributesContainer();

    CT_StdPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                    const CT_AbstractResult *result);

    CT_StdPointsAttributesContainer(const QString &modelName,
                                    const CT_AbstractResult *result);

    /**
     * @brief Insert the CT_AbstractPointsAttributes at key 'key'. If an attribute already exist it will be replaced.
     *        The attribute is not deleted from memory, you are the owner.
     */
    void insertPointsAttributesAt(const int &key, const CT_AbstractPointsAttributes *att);

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_STDPOINTSATTRIBUTESCONTAINER_H
