#ifndef CT_ATTRIBUTESSCALART_H
#define CT_ATTRIBUTESSCALART_H

#include "ct_attributes/abstract/ct_abstractattributesscalar.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 *  Attributes of scalar type (int, float, double, etc...)
 */
template<typename SCALAR>
class CT_AttributesScalarT : public CT_AbstractAttributesScalar
{
public:
    /**
     * @brief Create a collection of SCALAR.
     */
    CT_AttributesScalarT(const size_t &size = 0);

    /**
     * @brief Create a collection of SCALAR and compute the min and max values from the colection passed in parameter.
     */
    CT_AttributesScalarT(CT_StandardCloudStdVectorT<SCALAR> *collection);

    /**
     * @brief Create a collection of SCALAR and use the min and max passed in parameter.
     */
    CT_AttributesScalarT(CT_StandardCloudStdVectorT<SCALAR> *collection,
                         const SCALAR &min,
                         const SCALAR &max);

    ~CT_AttributesScalarT();

    double dMin() const;
    double dMax() const;

    double dValueAt(const size_t &index) const;

    SCALAR min() const;
    SCALAR max() const;

    void setMin(const SCALAR &min);
    void setMax(const SCALAR &max);

    const SCALAR& valueAt(const size_t &index) const;
    void setValueAt(const size_t &index, const SCALAR& value);

    size_t attributesSize() const;

private:
    SCALAR                              m_min;
    SCALAR                              m_max;
    CT_StandardCloudStdVectorT<SCALAR> *m_collection;

protected:

    void initMinMax();
    CT_StandardCloudStdVectorT<SCALAR>* collection() const { return m_collection; }

    static bool staticCompareScalar(const SCALAR &a, const SCALAR &b);
};

template<>
PLUGINSHAREDSHARED_EXPORT void CT_AttributesScalarT<bool>::initMinMax();

#include "ct_attributes/ct_attributesscalart.hpp"

#endif // CT_ATTRIBUTESSCALART_H
