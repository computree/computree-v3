#ifndef CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP
#define CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP

#include "ct_pointsattributesscalartemplated.h"

#include <typeinfo>

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated() : CT_AbstractPointAttributesScalar(), CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(const CT_OutAbstractSingularItemModel *model,
                                                                               const CT_AbstractResult *result,
                                                                               CT_PCIR pcir) : CT_AbstractPointAttributesScalar(model,
                                                                                                                                                                                            result,
                                                                                                                                                                                            pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(const CT_OutAbstractSingularItemModel *model,
                                                                               const CT_AbstractResult *result,
                                                                               CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractPointAttributesScalar(model,
                                                                                                                                                                  result,
                                                                                                                                                                  pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(const CT_OutAbstractSingularItemModel *model,
                                                                               const CT_AbstractResult *result,
                                                                               CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                                               const SCALAR &min,
                                                                               const SCALAR &max) : CT_AbstractPointAttributesScalar(model,
                                                                                                                                     result,
                                                                                                                                     pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(const QString &modelName,
                                                                               const CT_AbstractResult *result,
                                                                               CT_PCIR pcir) : CT_AbstractPointAttributesScalar(modelName,
                                                                                                                                                                                            result,
                                                                                                                                                                                            pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(const QString &modelName,
                                                                               const CT_AbstractResult *result,
                                                                               CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractPointAttributesScalar(modelName,
                                                                                                                                                                  result,
                                                                                                                                                                  pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(const QString &modelName,
                                                                               const CT_AbstractResult *result,
                                                                               CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                                               const SCALAR &min,
                                                                               const SCALAR &max) : CT_AbstractPointAttributesScalar(modelName,
                                                                                                                                     result,
                                                                                                                                     pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

template<typename SCALAR>
QString CT_PointsAttributesScalarTemplated<SCALAR>::getType() const
{
    return staticGetType();
}

template<typename SCALAR>
QString CT_PointsAttributesScalarTemplated<SCALAR>::staticGetType()
{
    return CT_AbstractPointAttributesScalar::staticGetType() + "/CT_PointsAttributesScalarTemplated<" + CT_TypeInfo::name<SCALAR>() + ">";
}

template<typename SCALAR>
QString CT_PointsAttributesScalarTemplated<SCALAR>::name() const
{
    return tr("Scalar(%1) point attributes").arg(CT_TypeInfo::name<SCALAR>());
}

template<typename SCALAR>
CT_AbstractItemDrawable* CT_PointsAttributesScalarTemplated<SCALAR>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_StandardCloudStdVectorT<SCALAR>* collect = this->collection();

    if (collect == NULL)
        return new CT_PointsAttributesScalarTemplated<SCALAR>((const CT_OutAbstractSingularItemModel *)model, result, getPointCloudIndexRegistered());
    else
        return new CT_PointsAttributesScalarTemplated<SCALAR>((const CT_OutAbstractSingularItemModel *)model, result, getPointCloudIndexRegistered(), dynamic_cast< CT_StandardCloudStdVectorT<SCALAR>* >(collect->copy()), this->min(), this->max());
}

#endif // CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP
