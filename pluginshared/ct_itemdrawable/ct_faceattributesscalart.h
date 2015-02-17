#ifndef CT_FACEATTRIBUTESSCALART_H
#define CT_FACEATTRIBUTESSCALART_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.h"
#include "ct_attributes/ct_attributesscalart.h"

template<typename SCALAR>
class CT_FaceAttributesScalarT : public CT_AbstractFaceAttributesScalar, public CT_AttributesScalarT<SCALAR>
{
public:
    CT_FaceAttributesScalarT();

    CT_FaceAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                             const CT_AbstractResult *result,
                             CT_FCIR pcir);

    CT_FaceAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                             const CT_AbstractResult *result,
                             CT_FCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection);

    CT_FaceAttributesScalarT(const CT_OutAbstractSingularItemModel *model,
                             const CT_AbstractResult *result,
                             CT_FCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection,
                             const SCALAR &min,
                             const SCALAR &max);

    CT_FaceAttributesScalarT(const QString &modelName,
                             const CT_AbstractResult *result,
                             CT_FCIR pcir);

    CT_FaceAttributesScalarT(const QString &modelName,
                             const CT_AbstractResult *result,
                             CT_FCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection);

    CT_FaceAttributesScalarT(const QString &modelName,
                             const CT_AbstractResult *result,
                             CT_FCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection,
                             const SCALAR &min,
                             const SCALAR &max);
    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    virtual QString name() const;

    double dMin() const { return CT_AttributesScalarT<SCALAR>::dMin(); }
    double dMax() const { return CT_AttributesScalarT<SCALAR>::dMax(); }

    double dValueAt(const size_t &index) const { return CT_AttributesScalarT<SCALAR>::dValueAt(index); }

    size_t attributesSize() const { return CT_AttributesScalarT<SCALAR>::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#include "ct_itemdrawable/ct_faceattributesscalart.hpp"

#endif // CT_FACEATTRIBUTESSCALART_H
