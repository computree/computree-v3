#ifndef CT_POINTSATTRIBUTESSCALARTEMPLATED_H
#define CT_POINTSATTRIBUTESSCALARTEMPLATED_H

#include "interfaces.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_attributes/ct_attributesscalart.h"

/**
 *  Points attributes of scalar type (int, float, double, etc...)
 */
template<typename SCALAR>
class CT_PointsAttributesScalarTemplated : public CT_AbstractPointAttributesScalar, public CT_AttributesScalarT<SCALAR>
{
public:

    CT_PointsAttributesScalarTemplated();

    /**
     * @brief Create a collection of SCALAR whose size is the same that the points cloud index passed as a parameter
     */
    CT_PointsAttributesScalarTemplated(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result,
                                       CT_PCIR pcir);

    /**
     * @brief Create a collection of SCALAR. Attention the size of the collection passed in parameter must be the same that the size of points cloud index.
     */
    CT_PointsAttributesScalarTemplated(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result,
                                       CT_PCIR pcir,
                                       CT_StandardCloudStdVectorT<SCALAR> *collection);

    /**
     * @brief Create a collection of SCALAR. Attention the size of the collection passed in parameter must be the same that the size of points cloud index.
     */
    CT_PointsAttributesScalarTemplated(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result,
                                       CT_PCIR pcir,
                                       CT_StandardCloudStdVectorT<SCALAR> *collection,
                                       const SCALAR &min,
                                       const SCALAR &max);

    CT_PointsAttributesScalarTemplated(const QString &modelName,
                                       const CT_AbstractResult *result,
                                       CT_PCIR pcir);

    CT_PointsAttributesScalarTemplated(const QString &modelName,
                                       const CT_AbstractResult *result,
                                       CT_PCIR pcir,
                                       CT_StandardCloudStdVectorT<SCALAR> *collection);

    CT_PointsAttributesScalarTemplated(const QString &modelName,
                                       const CT_AbstractResult *result,
                                       CT_PCIR pcir,
                                       CT_StandardCloudStdVectorT<SCALAR> *collection,
                                       const SCALAR &min,
                                       const SCALAR &max);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    virtual QString name() const;
    static QString staticName();

    double dMin() const { return CT_AttributesScalarT<SCALAR>::dMin(); }
    double dMax() const { return CT_AttributesScalarT<SCALAR>::dMax(); }

    double dValueAt(const size_t &index) const { return CT_AttributesScalarT<SCALAR>::dValueAt(index); }

    size_t attributesSize() const { return CT_AttributesScalarT<SCALAR>::attributesSize(); }

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#include "ct_itemdrawable/ct_pointsattributesscalartemplated.hpp"

#endif // CT_POINTSATTRIBUTESSCALARTEMPLATED_H
