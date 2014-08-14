#ifndef CT_FACEATTRIBUTESSCALART_HPP
#define CT_FACEATTRIBUTESSCALART_HPP

#include "ct_itemdrawable/ct_faceattributesscalart.h"

#include <typeinfo>

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT() : CT_AbstractFaceAttributesScalar(), CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const CT_OutAbstractItemModel *model,
                                                           const CT_AbstractResult *result,
                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir) : CT_AbstractFaceAttributesScalar(model,
                                                                                                                                                                      result,
                                                                                                                                                                      pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const CT_OutAbstractItemModel *model,
                                                           const CT_AbstractResult *result,
                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractFaceAttributesScalar(model,
                                                                                                                                             result,
                                                                                                                                             pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const CT_OutAbstractItemModel *model,
                                                           const CT_AbstractResult *result,
                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : CT_AbstractFaceAttributesScalar(model,
                                                                                                                result,
                                                                                                                pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}



template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const QString &modelName,
                                                           const CT_AbstractResult *result,
                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir) : CT_AbstractFaceAttributesScalar(modelName,
                                                                                                                                                                      result,
                                                                                                                                                                      pcir), CT_AttributesScalarT<SCALAR>(pcir.data() != NULL ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const QString &modelName,
                                                           const CT_AbstractResult *result,
                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : CT_AbstractFaceAttributesScalar(modelName,
                                                                                                                                             result,
                                                                                                                                             pcir), CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(const QString &modelName,
                                                           const CT_AbstractResult *result,
                                                           CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : CT_AbstractFaceAttributesScalar(modelName,
                                                                                                                result,
                                                                                                                pcir), CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::getType() const
{
    return staticGetType();
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::staticGetType()
{
    return CT_AbstractFaceAttributesScalar::staticGetType() + "/CT_FaceAttributesScalarT<" + QString(typeid(SCALAR).name()) + ">";
}

template<typename SCALAR>
QString CT_FaceAttributesScalarT<SCALAR>::name() const
{
    return QString("CT_FaceAttributesScalarT<") + QString(typeid(SCALAR).name()) + QString(">");
}

template<typename SCALAR>
CT_AbstractItemDrawable* CT_FaceAttributesScalarT<SCALAR>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_StandardCloudStdVectorT<SCALAR>* collect = this->collection();

    if (collect == NULL)
        return new CT_FaceAttributesScalarT<SCALAR>(model, result, getFaceCloudIndexRegistered());
    else
        return new CT_FaceAttributesScalarT<SCALAR>(model, result, getFaceCloudIndexRegistered(), dynamic_cast< CT_StandardCloudStdVectorT<SCALAR>* >(collect->copy()), this->min(), this->max());
}

#endif // CT_FACEATTRIBUTESSCALART_HPP
