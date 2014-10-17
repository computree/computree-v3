#ifndef CT_ABSTRACTITEMATTRIBUTE_H
#define CT_ABSTRACTITEMATTRIBUTE_H

#include "ct_item/abstract/ct_abstractitem.h"
#include "ct_categories/abstract/ct_abstractcategory.h"

class CT_OutAbstractItemAttributeModel;
class CT_AbstractResult;
class CT_AbstractItemDrawable;

/**
 * @brief An item attribute is a scalar value of type CT_AbstractCategory::ValueType
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemAttribute : public CT_AbstractItem
{
    Q_OBJECT

public:
    CT_AbstractItemAttribute();

    /**
     * @brief Create a attribute with a model defined in your step
     * @param model : the model of the attribute
     * @param category : the category for the attribute
     * @param result : the result of the attribute
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "setResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractItemAttribute(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractCategory *category, const CT_AbstractResult *result);

    /**
     * @brief Create a attribute with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your attribute
     * @param modelName : the unique model name of the model of the attribute (the model will be founded automatically) (typically a DEF_...)
     * @param categoryName : the unique name of the category for the attribute (the category will be founded automatically) (typically a CT_AbstractCategory::...)
     * @param result : the result of the attribute
     *
     * @warning The modelName must not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractItemAttribute(const QString &modelName, const QString &categoryName, const CT_AbstractResult *result);


    /**
     * @brief Use only this constructor for model !
     */
    CT_AbstractItemAttribute(const QString &categoryName);

    virtual ~CT_AbstractItemAttribute();

    /**
      * @brief Value is valid ? if yes the type is different from CT_AbstractCategory::UNKNOWN.
      */
    bool isValid() const;

    /**
     * @brief Set if this item attribute must use the coordinate system to convert the data if it was a coordinate.
     * @warning The parameter is ignored if the method canUseCoordinateSystem() returns false.
     */
    void setUseCoordinateSystem(bool enable);

    /**
     * @brief Returns true if this item attribute use the coordinate system to convert the data if it was a coordinate. False by default
     */
    bool useCoordinateSystem() const;

    /**
     * @brief Returns true if this item attribute can use the coordinate system. By default return false.
     */
    virtual bool canUseCoordinateSystem() const;

    /**
      * @brief Type of value
      */
    virtual CT_AbstractCategory::ValueType type() const = 0;

    /**
      * @brief Type of value to String
      */
    virtual QString typeToString() const = 0;

    /**
      * @brief Methods to get the value in bool type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual bool toBool(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in double type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual double toDouble(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in float type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual float toFloat(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in long double type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual long double toLongDouble(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in int type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual int toInt(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in uint64 type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual quint64 toUInt64(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in size_t type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual size_t toSizeT(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
      * @brief Methods to get the value in QString type
      *
      * @param item : the item to which this attribute belongs
      * @param ok : false if the value can not be converted to the type you want. ok can be NULL if you don't want
      *        to know the validity of the conversion.
      */
    virtual QString toString(const CT_AbstractItemDrawable *item, bool *ok) const = 0;

    /**
     * @brief Returns a displayable name (by default if model is not null return model()->displayableName(), else
     *        returns category()->displayableName())
     */
    QString displayableName() const;

    /**
     * @brief Returns the model of the attribute
     */
    CT_OutAbstractItemAttributeModel* model() const;

    /**
     * @brief Returns the category of the attribute
     */
    CT_AbstractCategory* category() const;

    /**
     * @brief Change the model of the attribute
     * @warning Not intended for direct use by plugin developper
     */
    void setModel(const CT_OutAbstractItemAttributeModel *model);

    /**
     * @brief Change the result that contains this attribute.
     * @warning Not intended for direct use by plugin developper
     */
    virtual void changeResult(const CT_AbstractResult *newRes);

    /**
     * @brief Returns a copy of the item attribute
     * @param model : the new model that must be set to the copy
     * @param result : the new result that will contains the copy
     * @return NULL if it was an error otherwise a copy of the item
     */
    virtual CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result) = 0;

protected:

    /**
     * @brief Called from Result or item to inform that this item will be removed from the result passed in parameter
     */
    virtual void internalSetWillBeRemovedFromResult(const CT_AbstractResult *res) { Q_UNUSED(res) }

private:
    CT_AbstractCategory     *m_category;
    bool                    m_useCoordinateSystem;
};

#endif // CT_ABSTRACTITEMATTRIBUTE_H
