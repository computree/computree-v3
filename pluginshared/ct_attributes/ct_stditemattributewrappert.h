#ifndef CT_STDITEMATTRIBUTEWRAPPERT_H
#define CT_STDITEMATTRIBUTEWRAPPERT_H

#include "ct_attributes/abstract/ct_abstractitemattributet.h"

/**
 * @brief An item attribute that contains a pointer to a method of an item that returns the data. Use
 *        this class with a CT_IACreator :
 *
 *        CT_AbstractItemAttribute *attribute = CT_IACreator::create(DEF_ATTRIBUTE_MODEL_NAME, CATEGORY_NAME, result, &CT_Line::getP1_X);
 */
template <class ItemDrawableClass, typename VType>
class CT_StdItemAttributeWrapperT : public CT_AbstractItemAttributeT<VType>
{
public:
    typedef VType (ItemDrawableClass::*getter)() const;

    CT_StdItemAttributeWrapperT(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result, getter gMethod);
    CT_StdItemAttributeWrapperT(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result, getter gMethod);

    CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result);

protected:
    VType data(const CT_AbstractItemDrawable *item) const;

private:
    ItemDrawableClass   *m_item;
    getter              m_gMethod;
};

#include "ct_attributes/ct_stditemattributewrappert.hpp"

#endif // CT_STDITEMATTRIBUTEWRAPPERT_H
