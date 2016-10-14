#ifndef CT_STDLASPOINTSATTRIBUTESCONTAINER_H
#define CT_STDLASPOINTSATTRIBUTESCONTAINER_H

#include "ctliblas/ctliblas_global.h"
#include "ctliblas/tools/las/ct_lasdefine.h"
#include "ctliblas/tools/las/ct_lasdata.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

/**
 *  This class is a standard container for LAS points attributes
 */
class CTLIBLAS_EXPORT CT_StdLASPointsAttributesContainer : public CT_AbstractPointsAttributesContainer
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StdLASPointsAttributesContainer, CT_AbstractPointsAttributesContainer)

public:     

    CT_StdLASPointsAttributesContainer();

    CT_StdLASPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result);

    CT_StdLASPointsAttributesContainer(const QString &modelName,
                                       const CT_AbstractResult *result);

    virtual QString name() const;


    /**
     * @brief Insert the CT_AbstractPointsAttributes at key 'key'. If an attribute already exist it will be replaced.
     *        The attribute is not deleted from memory, you are the owner.
     */
    virtual void insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att);

    /**
     * @brief Return the list of type of attributes present in this container
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> lasPointAttributesTypes() const;

    virtual void getLASDataAt(const size_t i, CT_LASData &data) const;

    /**
     * @brief Return the hash map of attributes
     */
    virtual QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*> lasPointsAttributes() const;

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_STDLASPOINTSATTRIBUTESCONTAINER_H
