#ifndef CT_IACREATOR_H
#define CT_IACREATOR_H

#include "ct_attributes/ct_stditemattributewrappert.h"
#include "ct_attributes/ct_stditemattributet.h"
#include "ct_attributes/ct_stditemattributecoordinatewrappert.h"
#include "ct_attributes/ct_stditemattributecoordinatet.h"

class PLUGINSHAREDSHARED_EXPORT CT_IACreator
{
public:

    /************* WRAPPER TO METHOD ********************/

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
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::create(DEF_ATT, CT_AbstractCategory::DATA_RADIUS, outResult, &CT_Circle::getRadius);
     *          circle->addAttribute(attribute);
     */
    template <typename ItemDrawableClass, typename VType>
    static CT_AbstractItemAttribute* create(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result, typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethod)
    {
        return new CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>(modelName, categoryName, result, gMethod);
    }

    /**
     * @brief Create a item attribute (that represent a coordinate) that can use a method of an ItemDrawable to get a data.
     * @param model : the model of the attribute (can be NULL but must be set before finish the step computing by the method setModel(...) )
     * @param category : the category for the attribute (must not be NULL)
     * @param result : the result where the attribute will be stocked (can be NULL but must be set before finish the step computing by the method setResult(...) )
     * @param gMethodX : the method of the itemdrawable to call to get the x data
     * @param gMethodY : the method of the itemdrawable to call to get the y data
     * @param gMethodZ : the method of the itemdrawable to call to get the z data (can be null if you don't use 3D)
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(model, category, outResult, &CT_AbstractSingularItem::getCenterX, &CT_AbstractSingularItem::getCenterX, &CT_AbstractSingularItem::getCenterY, &CT_AbstractSingularItem::getCenterZ);
     *          myItem->addAttribute(attribute);
     */
    template <typename ItemDrawableClass, typename VType>
    static CT_AbstractItemAttribute* createCoordinate(CT_OutAbstractItemAttributeModel *model,
                                                      const CT_AbstractCategory *category,
                                                      const CT_AbstractResult *result,
                                                      typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethodX,
                                                      typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethodY,
                                                      typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethodZ)
    {
        return new CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass, VType>(model, category, result, gMethodX, gMethodY, gMethodZ);
    }

    /**
     * @brief Create a item attribute (that represent a coordinate) that can use a method of an ItemDrawable to get a data.
     * @param modelName : name of model defined in your step (typically a DEF_...) (can not be empty)
     * @param categoryName : the unique name of the category for the attribute (the category will be founded automatically) (typically a CT_AbstractCategory::...)
     * @param result : the result where the attribute will be stocked (can not be NULL)
     * @param gMethodX : the method of the itemdrawable to call to get the x data
     * @param gMethodY : the method of the itemdrawable to call to get the y data
     * @param gMethodZ : the method of the itemdrawable to call to get the z data (can be null if you don't use 3D)
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(DEF_ATT, CT_AbstractCategory::DATA_CX, outResult, &CT_AbstractSingularItem::getCenterX, &CT_AbstractSingularItem::getCenterX, &CT_AbstractSingularItem::getCenterY, &CT_AbstractSingularItem::getCenterZ);
     *          myItem->addAttribute(attribute);
     */
    template <typename ItemDrawableClass, typename VType>
    static CT_AbstractItemAttribute* createCoordinate(const QString &modelName,
                                                      const QString &categoryName,
                                                      const CT_AbstractResult *result,
                                                      typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethodX,
                                                      typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethodY,
                                                      typename CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>::getter gMethodZ)
    {
        return new CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass, VType>(modelName, categoryName, result, gMethodX, gMethodY, gMethodZ);
    }

    /*********************** REAL DATA *********************/

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

    /**
     * @brief Create a item attribute (that represent a coordinate) with a default data.
     * @param model : the model of the attribute (can be NULL but must be set before finish the step computing by the method setModel(...) )
     * @param category : the category for the attribute (must not be NULL)
     * @param result : the result where the attribute will be stocked (can be NULL but must be set before finish the step computing by the method setResult(...) )
     * @param dataX : the x data that will never change
     * @param dataY : the y data that will never change
     * @param dataZ : the z data that will never change
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(model, category, outResult, 5, 0, 10);
     *          myItem->addAttribute(attribute);
     */
    template <typename VType>
    static CT_AbstractItemAttribute* createCoordinate(CT_OutAbstractItemAttributeModel *model,
                                                      const CT_AbstractCategory *category,
                                                      const CT_AbstractResult *result,
                                                      const VType &dataX,
                                                      const VType &dataY,
                                                      const VType &dataZ)
    {
        return new CT_StdItemAttributeCoordinateT<VType>(model, category, result, dataX, dataY, dataZ);
    }

    /**
     * @brief Create a item attribute (that represent a coordinate) with a default data.
     * @param modelName : name of model defined in your step (typically a DEF_...) (can not be empty)
     * @param categoryName : the unique name of the category for the attribute (the category will be founded automatically) (typically a CT_AbstractCategory::...)
     * @param result : the result where the attribute will be stocked (can not be NULL)
     * @param dataX : the x data that will never change
     * @param dataY : the y data that will never change
     * @param dataZ : the z data that will never change
     *
     * @example CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(DEF_ATT, CT_AbstractCategory::DATA_CY, outResult, 4, 2, 8);
     *          myItem->addAttribute(attribute);
     */
    template <typename VType>
    static CT_AbstractItemAttribute* createCoordinate(const QString &modelName,
                                                      const QString &categoryName,
                                                      const CT_AbstractResult *result,
                                                      const VType &dataX,
                                                      const VType &dataY,
                                                      const VType &dataZ)
    {
        return new CT_StdItemAttributeCoordinateT<VType>(modelName, categoryName, result, dataX, dataY, dataZ);
    }
};

#endif // CT_IACREATOR_H
