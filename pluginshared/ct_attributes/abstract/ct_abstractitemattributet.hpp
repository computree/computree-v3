#ifndef CT_ABSTRACTITEMATTRIBUTET_HPP
#define CT_ABSTRACTITEMATTRIBUTET_HPP

#include "ct_attributes/abstract/ct_abstractitemattributet.h"

#include "ct_tools/ct_numerictostringconversiont.h"

template <typename VType>
CT_AbstractItemAttributeT<VType>::CT_AbstractItemAttributeT(const CT_OutAbstractItemAttributeModel *model,
                                                            const CT_AbstractCategory *category,
                                                            const CT_AbstractResult *result) : CT_AbstractItemAttribute(model,
                                                                                                                     category,
                                                                                                                     result)
{
    // TODO
    /*QString error = internalVerifyCategory();
    Q_ASSERT_X(error.isEmpty(), "CT_AbstractItemAttribute constructor", qPrintable("You created a Attribute with a categoryName but the category is not allowed : " + error));
    */
}

template <typename VType>
CT_AbstractItemAttributeT<VType>::CT_AbstractItemAttributeT(const QString &modelName,
                                                            const QString &categoryName,
                                                            const CT_AbstractResult *result) : CT_AbstractItemAttribute(modelName,
                                                                                                                     categoryName,
                                                                                                                     result)
{
    // TODO
    /*QString error = internalVerifyCategory();
    Q_ASSERT_X(error.isEmpty(), "CT_AbstractItemAttribute constructor", qPrintable("You created a Attribute with a categoryName but the category is not allowed : " + error));
    */
}

template <typename VType>
bool CT_AbstractItemAttributeT<VType>::toBool(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return false;
}

template <typename VType>
double CT_AbstractItemAttributeT<VType>::toDouble(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return 0;
}

template <typename VType>
float CT_AbstractItemAttributeT<VType>::toFloat(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return 0;
}

template <typename VType>
long double CT_AbstractItemAttributeT<VType>::toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return 0;
}

template <typename VType>
int CT_AbstractItemAttributeT<VType>::toInt(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return 0;
}

template <typename VType>
quint64 CT_AbstractItemAttributeT<VType>::toUInt64(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return 0;
}

template <typename VType>
size_t CT_AbstractItemAttributeT<VType>::toSizeT(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = false;

    return 0;
}

template <typename VType>
QString CT_AbstractItemAttributeT<VType>::toString(const CT_AbstractItemDrawable *item, bool *ok) const
{
    Q_UNUSED(item)

    if(ok != NULL)
        *ok = true;

    return CT_NumericToStringConversionT<VType>::toString(data(item));
}

template <typename VType>
CT_AbstractCategory::ValueType CT_AbstractItemAttributeT<VType>::realType() const
{
    return CT_AbstractCategory::staticValueTypeToCategoryType<VType>();
}

template <typename VType>
QString CT_AbstractItemAttributeT<VType>::realTypeToString() const
{
    return CT_AbstractCategory::staticValueTypeToCategoryTypeString<VType>();
}

template <typename VType>
QString CT_AbstractItemAttributeT<VType>::internalVerifyCategory() const
{
    CT_AbstractCategory::ValueType realType = CT_AbstractCategory::staticValueTypeToCategoryType<VType>();
    CT_AbstractCategory::ValueType catType = type();

    if(catType == realType)
        return "";

    return QString("The type ") + category()->valueTypeToString(catType) + " defined in the category does not match the real type : " + category()->valueTypeToString(realType);
}

#endif // CT_ABSTRACTITEMATTRIBUTET_HPP
