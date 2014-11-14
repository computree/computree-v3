#ifndef CT_STDITEMATTRIBUTET_H
#define CT_STDITEMATTRIBUTET_H

#include "ct_attributes/abstract/ct_abstractitemattributet.h"

/**
 * @brief An item attribute that contains a data that will not changed. Use
 *        this class with a CT_IACreator :
 *
 *        CT_AbstractItemAttribute *attribute = CT_IACreator::create(DEF_ATTRIBUTE_MODEL_NAME, CATEGORY_NAME, result, data);
 */
template <typename VType>
class CT_StdItemAttributeT : public CT_AbstractItemAttributeT<VType>
{
public:
    CT_StdItemAttributeT(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result, const VType &data);
    CT_StdItemAttributeT(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result, const VType &data);

    /**
     * @brief Use only this constructor for model !
     */
    CT_StdItemAttributeT(const QString &categoryName);

    CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result);

protected:
    VType data(const CT_AbstractItemDrawable *item) const;

private:
    VType    m_data;
};

// specialisation for QString
template<>
PLUGINSHAREDSHARED_EXPORT CT_StdItemAttributeT<QString>::CT_StdItemAttributeT(const QString &categoryName);

#include "ct_attributes/ct_stditemattributet.hpp"

#endif // CT_STDITEMATTRIBUTET_H
