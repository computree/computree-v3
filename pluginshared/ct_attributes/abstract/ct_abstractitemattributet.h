#ifndef CT_ABSTRACTITEMATTRIBUTET_H
#define CT_ABSTRACTITEMATTRIBUTET_H

#include "ct_attributes/abstract/ct_abstractitemattribute.h"
#include "ct_coordinates/abstract/ct_abstractcoordinatesystem.h"

template <typename VType>
class CT_AbstractItemAttributeT : public CT_AbstractItemAttribute
{
public:
    CT_AbstractItemAttributeT(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result);
    CT_AbstractItemAttributeT(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result);

    /**
     * @brief Use only this constructor for model !
     */
    CT_AbstractItemAttributeT(const QString &categoryName);

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
    virtual QString toString(const CT_AbstractItemDrawable *item, bool *ok) const;

    template<typename T>
    T toTypeT(const CT_AbstractItemDrawable *item) const {
        return static_cast<T>(data(item));
    }

    /**
      * @brief Type of value
      */
    CT_AbstractCategory::ValueType type() const;

    /**
      * @brief Type of value to String
      */
    QString typeToString() const;

protected:

    /**
     * @brief Returns the data (not converted with the coordinate system)
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

// SIGNED INTEGER -> FLOAT
template<>
inline float CT_AbstractItemAttributeT<int>::toFloat(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// SIGNED INTEGER -> DOUBLE
template<>
inline double CT_AbstractItemAttributeT<int>::toDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// SIGNED INTEGER -> LONG DOUBLE
template<>
inline long double CT_AbstractItemAttributeT<int>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// UNSIGNED INTEGER 64bits -> UNSIGNED INTEGER 64bits
template<>
inline quint64 CT_AbstractItemAttributeT<quint64>::toUInt64(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// UNSIGNED INTEGER 64bits -> FLOAT
template<>
inline float CT_AbstractItemAttributeT<quint64>::toFloat(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// UNSIGNED INTEGER 64bits -> DOUBLE
template<>
inline double CT_AbstractItemAttributeT<quint64>::toDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// UNSIGNED INTEGER 64bits -> LONG DOUBLE
template<>
inline long double CT_AbstractItemAttributeT<quint64>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


// SIZE_T -> SIZE_T
template<>
inline size_t CT_AbstractItemAttributeT<size_t>::toSizeT(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

#ifndef ENVIRONMENT64
// SIZE_T -> FLOAT
template<>
inline float CT_AbstractItemAttributeT<size_t>::toFloat(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// SIZE_T -> DOUBLE
template<>
inline double CT_AbstractItemAttributeT<size_t>::toDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }

// SIZE_T -> LONG DOUBLE
template<>
inline long double CT_AbstractItemAttributeT<size_t>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }
#endif

// QSTRING -> QSTRING
template<>
inline QString CT_AbstractItemAttributeT<QString>::toString(const CT_AbstractItemDrawable *item, bool *ok) const { if(ok != NULL) { *ok = true; } return data(item); }


//specialization to avoid compilation error

// STRING
template<>
inline QString CT_AbstractItemAttributeT<QString>::data(const CT_AbstractItemDrawable *item) const { return data(item); }

#include "ct_attributes/abstract/ct_abstractitemattributet.hpp"

#endif // CT_ABSTRACTITEMATTRIBUTET_H
