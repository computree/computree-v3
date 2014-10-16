#ifndef CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_H
#define CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_H

#include "ct_attributes/ct_stditemattributewrappert.h"
#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

/**
 * @brief An item attribute that contains a pointer to a method of an item that returns the data of type COORDINATE ! Use
 *        this class with a CT_IACreator :
 *
 *        CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(DEF_ATTRIBUTE_MODEL_NAME, CATEGORY_NAME, result, &CT_Line::getP1_X, &CT_Line::getP1_X, &CT_Line::getP1_Y, &CT_Line::getP1_Z);
 */
template <class ItemDrawableClass, typename VType>
class CT_StdItemAttributeCoordinateWrapperT : public CT_StdItemAttributeWrapperT<ItemDrawableClass, VType>
{
public:
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

    virtual CT_AbstractItemAttribute* copy(const CT_OutAbstractItemAttributeModel *model, const CT_AbstractResult *result);

protected:
    virtual VType data(const CT_AbstractItemDrawable *item) const;

    getter                                      m_gMethodY;
    getter                                      m_gMethodZ;
    CT_AbstractCoordinateSystem::DataToConvert  m_convert;
};


#include "ct_attributes/ct_stditemattributecoordinatewrappert.hpp"

#endif // CT_STDITEMATTRIBUTECOORDINATEWRAPPERT_H
