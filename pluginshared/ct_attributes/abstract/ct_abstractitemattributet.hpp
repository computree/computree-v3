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
}

template <typename VType>
CT_AbstractItemAttributeT<VType>::CT_AbstractItemAttributeT(const QString &modelName,
                                                            const QString &categoryName,
                                                            const CT_AbstractResult *result) : CT_AbstractItemAttribute(modelName,
                                                                                                                     categoryName,
                                                                                                                     result)
{
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
CT_AbstractCategory::ValueType CT_AbstractItemAttributeT<VType>::type() const
{
    return CT_AbstractCategory::staticValueTypeToCategoryType<VType>();
}

template <typename VType>
QString CT_AbstractItemAttributeT<VType>::typeToString() const
{
    return CT_AbstractCategory::staticValueTypeToCategoryTypeString<VType>();
}

#endif // CT_ABSTRACTITEMATTRIBUTET_HPP
