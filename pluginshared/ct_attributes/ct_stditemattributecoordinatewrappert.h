#ifndef CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_H
#define CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_H

#include "ct_attributes/abstract/ct_abstractitemattributet.h"
#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

/*#if !defined(static_assert)
#   define static_assert(pred, explanation) { #error tmp}
#endif*/

/**
 * @brief An item attribute that contains a pointer to a method of an item that returns the data of type COORDINATE ! Use
 *        this class with a CT_IACreator :
 *
 *        CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(DEF_ATTRIBUTE_MODEL_NAME, CATEGORY_NAME, result, &CT_Line::getP1_X, &CT_Line::getP1_X, &CT_Line::getP1_Y, &CT_Line::getP1_Z);
 */
template <class ItemDrawableClass, typename VType>
class CT_StdItemAttributeCoordinateWrapperT : public CT_AbstractItemAttributeT<VType>
{
public:
    typedef VType (ItemDrawableClass::*getter)() const;

    CT_StdItemAttributeCoordinateWrapperT(const CT_OutAbstractItemAttributeModel *model,
                                          const CT_AbstractCategory *category,
                                          const CT_AbstractResult *result,
                                          getter gMethodX,
                                          getter gMethodY,
                                          getter gMethodZ);

    CT_StdItemAttributeCoordinateWrapperT(const QString &modelName,
                                          const QString &categoryName,
                                          const CT_AbstractResult *result,
                                          getter gMethodX,
                                          getter gMethodY,
                                          getter gMethodZ);

    /**
     * @brief Use only this constructor for model !
     */
    CT_StdItemAttributeCoordinateWrapperT(const QString &categoryName);

    /**
     * @brief Returns true if this item attribute can use the coordinate system.
     */
    virtual bool canUseCoordinateSystem() const;

    virtual CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result);

protected:
    /**
     * @brief Returns the data (not converted with the coordinate system)
     */
    virtual VType data(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Return the data converted by the coordinate system.
     */
    virtual CT_AbstractCoordinateSystem::realEx dataConverted(const CT_AbstractItemDrawable *item) const;

    getter                                      m_gMethods[3];
    CT_AbstractCoordinateSystem::DataToConvert  m_convert;
    int                                         m_index;

    void initConvertionVariable();
};

// class specialization to avoid certains type for coordinate

#define AVOID_COORDINATE_TYPE(X) template <class ItemDrawableClass> \
class CT_StdItemAttributeCoordinateWrapperT<ItemDrawableClass, X> : public CT_AbstractItemAttributeT<X> \
{ \
public:\
    typedef X (ItemDrawableClass::*getter)() const; \
    CT_StdItemAttributeCoordinateWrapperT(const CT_OutAbstractItemAttributeModel *model, \
                                          const CT_AbstractCategory *category, \
                                          const CT_AbstractResult *result, \
                                          getter gMethodX, \
                                          getter gMethodY, \
                                          getter gMethodZ) : CT_AbstractItemAttributeT<X>(model, category, result) {} \
    CT_StdItemAttributeCoordinateWrapperT(const QString &modelName, \
                                          const QString &categoryName, \
                                          const CT_AbstractResult *result, \
                                          getter gMethodX, \
                                          getter gMethodY, \
                                          getter gMethodZ) : CT_AbstractItemAttributeT<X>(modelName, categoryName, result) {} \
    virtual bool canUseCoordinateSystem() const { /*static_assert(0, "Your type can not be a coordinate !");*/ return false; } \
    virtual CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result) { return NULL; } \
protected: \
    virtual X data(const CT_AbstractItemDrawable *item) const { return X(0); } \
}; \

// BOOLEAN
AVOID_COORDINATE_TYPE(bool)

// STRING
AVOID_COORDINATE_TYPE(QString)

#include "ct_attributes/ct_stditemattributecoordinatewrappert.hpp"

#endif // CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_H
