#ifndef CT_EDGEATTRIBUTESSCALART_H
#define CT_EDGEATTRIBUTESSCALART_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.h"
#include "ct_attributes/ct_attributesscalart.h"

template<typename SCALAR>
class CT_EdgeAttributesScalarT : public CT_AbstractEdgeAttributesScalar, public CT_AttributesScalarT<SCALAR>
{
public:
    CT_EdgeAttributesScalarT();

    CT_EdgeAttributesScalarT(const CT_OutAbstractItemModel *model,
                             CT_AbstractResult *result,
                             CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir);

    CT_EdgeAttributesScalarT(const CT_OutAbstractItemModel *model,
                             CT_AbstractResult *result,
                             CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection);

    CT_EdgeAttributesScalarT(const CT_OutAbstractItemModel *model,
                             CT_AbstractResult *result,
                             CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection,
                             const SCALAR &min,
                             const SCALAR &max);

    CT_EdgeAttributesScalarT(const QString &modelName,
                             CT_AbstractResult *result,
                             CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir);

    CT_EdgeAttributesScalarT(const QString &modelName,
                             CT_AbstractResult *result,
                             CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection);

    CT_EdgeAttributesScalarT(const QString &modelName,
                             CT_AbstractResult *result,
                             CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR pcir,
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

#include "ct_itemdrawable/ct_edgeattributesscalart.hpp"

#endif // CT_EDGEATTRIBUTESSCALART_H
