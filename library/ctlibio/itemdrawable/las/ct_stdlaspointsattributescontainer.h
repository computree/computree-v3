#ifndef CT_STDLASPOINTSATTRIBUTESCONTAINER_H
#define CT_STDLASPOINTSATTRIBUTESCONTAINER_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

#include "ctlibio/tools/las/ct_lasdefine.h"
#include "ctlibio/tools/las/ct_lasdata.h"

/**
 *  This class is a standard container for LAS points attributes
 */
class CTLIBIO_EXPORT CT_StdLASPointsAttributesContainer : public CT_AbstractPointsAttributesContainer
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StdLASPointsAttributesContainer, CT_AbstractPointsAttributesContainer)

public:     

    CT_StdLASPointsAttributesContainer();

    CT_StdLASPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result);

    CT_StdLASPointsAttributesContainer(const QString &modelName,
                                       const CT_AbstractResult *result);

    /**
     * @brief Insert the CT_AbstractPointsAttributes at key 'key'. If an attribute already exist it will be replaced.
     *        The attribute is not deleted from memory, you are the owner.
     */
    void insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att);

    /**
     * @brief Return the list of type of attributes present in this container
     */
    QList<CT_LasDefine::LASPointAttributesType> lasPointAttributesTypes() const;

    void getLASDataAt(const size_t i, CT_LASData &data) const;

    /**
     * @brief Return the hash map of attributes
     */
    QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*> lasPointsAttributes() const;

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_STDLASPOINTSATTRIBUTESCONTAINER_H
