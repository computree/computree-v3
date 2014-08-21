#ifndef CT_ABSTRACTITEMATTRIBUTET_H
#define CT_ABSTRACTITEMATTRIBUTET_H

#include "ct_attributes/abstract/ct_abstractitemattribute.h"

template <typename VType>
class CT_AbstractItemAttributeT : public CT_AbstractItemAttribute
{
public:
    CT_AbstractItemAttributeT(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result);
    CT_AbstractItemAttributeT(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result);

    /**
     * @brief ok = false if the specialization does not exist
     */
    bool toBool(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = false if the specialization does not exist
     */
    double toDouble(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = false if the specialization does not exist
     */
    float toFloat(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = false if the specialization does not exist
     */
    long double toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = false if the specialization does not exist
     */
    int toInt(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = false if the specialization does not exist
     */
    quint64 toUInt64(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = false if the specialization does not exist
     */
    size_t toSizeT(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
     * @brief ok = true in all case and the value is converted to a QString.
     */
    QString toString(const CT_AbstractItemDrawable *item, bool *ok) const;

    /**
      * @brief Real type of value
      */
    CT_AbstractCategory::ValueType realType() const;

    /**
      * @brief Real type of value to String
      */
    QString realTypeToString() const;

protected:

    /**
     * @brief Returns an error if the category don't respect the type of data otherwise return a empty string
     */
    QString internalVerifyCategory() const;

    /**
     * @brief Returns the data
     */
    virtual VType data(const CT_AbstractItemDrawable *item) const = 0;
};

//specialization to convert a type to another on compilation

// BOOLEAN -> BOOLEAN
template<>
inline bool CT_AbstractItemAttributeT<bool>::toBool(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// LONG DOUBLE -> LONG DOUBLE
template<>
inline long double CT_AbstractItemAttributeT<long double>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// DOUBLE -> DOUBLE
template<>
inline double CT_AbstractItemAttributeT<double>::toDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// DOUBLE -> LONG DOUBLE
template<>
inline long double CT_AbstractItemAttributeT<double>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// FLOAT -> FLOAT
template<>
inline float CT_AbstractItemAttributeT<float>::toFloat(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// FLOAT -> DOUBLE
template<>
inline double CT_AbstractItemAttributeT<float>::toDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// FLOAT -> LONG DOUBLE
template<>
inline long double CT_AbstractItemAttributeT<float>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// SIGNED INTEGER -> SIGNED INTEGER
template<>
inline int CT_AbstractItemAttributeT<int>::toInt(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// UNSIGNED INTEGER 64bits -> UNSIGNED INTEGER 64bits
template<>
inline quint64 CT_AbstractItemAttributeT<quint64>::toUInt64(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// SIZE_T -> SIZE_T
template<>
inline size_t CT_AbstractItemAttributeT<size_t>::toSizeT(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// QSTRING -> QSTRING
template<>
inline QString CT_AbstractItemAttributeT<QString>::toString(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

#include "ct_attributes/abstract/ct_abstractitemattributet.hpp"

#endif // CT_ABSTRACTITEMATTRIBUTET_H
