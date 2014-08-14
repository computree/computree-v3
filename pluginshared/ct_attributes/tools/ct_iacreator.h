#ifndef CT_IACREATOR_H
#define CT_IACREATOR_H

#include "ct_attributes/ct_stditemattributewrappert.h"
#include "ct_attributes/ct_stditemattributet.h"

class PLUGINSHAREDSHARED_EXPORT CT_IACreator
{
public:

    /**
     * @brief Create a item attribute that can use a method of an ItemDrawable to get a data.
     * @param model : the model of the attribute (can be NULL but must be set before finish the step computing by the method setModel(...) )
     * @param category : the category for the attribute (must not be NULL)
     * @param result : the result where the attribute will be stocked (can be NULL but must be set before finish the step computing by the method setResult(...) )
     * @param gMethod : the method of the itemdrawable to call to get the data.
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::create(model, category, outResult, &CT_Circle::getRadius);
     *          circle->addAttribute(attribute);
     */
    template <typename ItemDrawableClass, typename VType>
    static CT_AbstractItemAttribute* create(CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result, typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethod)
    {
        return new CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>(model, category, result, gMethod);
    }

    /**
     * @brief Create a item attribute that can use a method of an ItemDrawable to get a data.
     * @param modelName : name of model defined in your step (typically a DEF_...) (can not be empty)
     * @param categoryName : the unique name of the category for the attribute (the category will be founded automatically) (typically a CT_AbstractCategory::...)
     * @param result : the result where the attribute will be stocked (can not be NULL)
     * @param gMethod : the method of the itemdrawable to call to get the data.
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::create(DEF_ATT, CT_AbstractCategory::RADIUS, outResult, &CT_Circle::getRadius);
     *          circle->addAttribute(attribute);
     */
    template <typename ItemDrawableClass, typename VType>
    static CT_AbstractItemAttribute* create(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result, typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethod)
    {
        return new CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>(modelName, categoryName, result, gMethod);
    }

    /**
     * @brief Create a item attribute with a default data.
     * @param model : the model of the attribute (can be NULL but must be set before finish the step computing by the method setModel(...) )
     * @param category : the category for the attribute (must not be NULL)
     * @param result : the result where the attribute will be stocked (can be NULL but must be set before finish the step computing by the method setResult(...) )
     * @param data : the data that will never change
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::create(model, category, outResult, 5.0f);
     *          item->addAttribute(attribute);
     */
    template <typename VType>
    static CT_AbstractItemAttribute* create(CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result, const VType &data)
    {
        return new CT_StdItemAttributeT<VType>(model, category, result, data);
    }

    /**
     * @brief Create a item attribute with a default data.
     * @param modelName : name of model defined in your step (typically a DEF_...) (can not be empty)
     * @param categoryName : the unique name of the category for the attribute (the category will be founded automatically) (typically a CT_AbstractCategory::...)
     * @param result : the result where the attribute will be stocked (can not be NULL)
     * @param data : the data that will never change
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::create(DEF_ATT, DEF_MY_CATEGORY, outResult, 1);
     *          item->addAttribute(attribute);
     */
    template <typename VType>
    static CT_AbstractItemAttribute* create(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result, const VType &data)
    {
        return new CT_StdItemAttributeT<VType>(modelName, categoryName, result, data);
    }
};

#endif // CT_IACREATOR_H
