#ifndef CT_EDGEATTRIBUTESSCALART_HPP
#define CT_EDGEATTRIBUTESSCALART_HPP

#include "ct_itemdrawable/ct_edgeattributesscalart.h"

#include <typeinfo>

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT() : CT_AbstractEdgeAttributesScalar(), CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                                                           CT_AbstractResult *result,
                                                           CT_ECIR pcir) : CT_AbstractEdgeAttributesScalar(model,
                                                                                                                                                                      result,
                                                                                                                                                                      pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                                                           CT_AbstractResult *result,
                                                           CT_ECIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractEdgeAttributesScalar(model,
                                                                                                                                             result,
                                                                                                                                             pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                                                           CT_AbstractResult *result,
                                                           CT_ECIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : CT_AbstractEdgeAttributesScalar(model,
                                                                                                                result,
                                                                                                                pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}



template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(const QString &modelName,
                                                           CT_AbstractResult *result,
                                                           CT_ECIR pcir) : CT_AbstractEdgeAttributesScalar(modelName,
                                                                                                                                                                      result,
                                                                                                                                                                      pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(const QString &modelName,
                                                           CT_AbstractResult *result,
                                                           CT_ECIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractEdgeAttributesScalar(modelName,
                                                                                                                                             result,
                                                                                                                                             pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(const QString &modelName,
                                                           CT_AbstractResult *result,
                                                           CT_ECIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : CT_AbstractEdgeAttributesScalar(modelName,
                                                                                                                result,
                                                                                                                pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

template<typename SCALAR>
QString CT_EdgeAttributesScalarT<SCALAR>::getType() const
{
    return staticGetType();
}

template<typename SCALAR>
QString CT_EdgeAttributesScalarT<SCALAR>::staticGetType()
{
    return CT_AbstractEdgeAttributesScalar::staticGetType() + "/CT_EdgeAttributesScalarT<" + CT_TypeInfo::name<SCALAR>() + ">";
}

template<typename SCALAR>
QString CT_EdgeAttributesScalarT<SCALAR>::name() const
{
    return tr("Scalar(%1) edge attributes").arg(CT_TypeInfo::name<SCALAR>());
}

template<typename SCALAR>
CT_AbstractItemDrawable* CT_EdgeAttributesScalarT<SCALAR>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_StandardCloudStdVectorT<SCALAR>* collect = this->collection();

    if (collect == NULL)
        return new CT_EdgeAttributesScalarT<SCALAR>((const CT_OutAbstractSingularItemModel *)model, result, getEdgeCloudIndexRegistered());
    else
        return new CT_EdgeAttributesScalarT<SCALAR>((const CT_OutAbstractSingularItemModel *)model, result, getEdgeCloudIndexRegistered(), dynamic_cast< CT_StandardCloudStdVectorT<SCALAR>* >(collect->copy()), this->min(), this->max());
}

#endif // CT_EDGEATTRIBUTESSCALART_HPP
