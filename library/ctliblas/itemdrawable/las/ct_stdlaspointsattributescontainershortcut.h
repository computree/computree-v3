#ifndef CT_STDLASPOINTSATTRIBUTESCONTAINERSHORTCUT_H
#define CT_STDLASPOINTSATTRIBUTESCONTAINERSHORTCUT_H

#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"

/**
 *  This class is a shortcut class for an existing standard container for LAS points attributes
 */
class CTLIBLAS_EXPORT CT_StdLASPointsAttributesContainerShortcut : public CT_StdLASPointsAttributesContainer
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StdLASPointsAttributesContainerShortcut, CT_StdLASPointsAttributesContainer)

public:     

    CT_StdLASPointsAttributesContainerShortcut();

    CT_StdLASPointsAttributesContainerShortcut(const CT_OutAbstractSingularItemModel *model,
                                               const CT_AbstractResult *result,
                                               CT_StdLASPointsAttributesContainer *container);

    CT_StdLASPointsAttributesContainerShortcut(const QString &modelName,
                                               const CT_AbstractResult *result,
                                               CT_StdLASPointsAttributesContainer *container);

    virtual QString name() const;


    void insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att);

    CT_AbstractPointsAttributes* pointsAttributesAt(const int &key) const;

    QList<CT_LasDefine::LASPointAttributesType> lasPointAttributesTypes() const;

    void getLASDataAt(const size_t i, CT_LASData &data) const;

    QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*> lasPointsAttributes() const;

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:

    CT_StdLASPointsAttributesContainer* _container;
};

#endif // CT_STDLASPOINTSATTRIBUTESCONTAINERSHORTCUT_H
