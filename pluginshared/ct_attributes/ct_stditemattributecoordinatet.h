#ifndef CT_STDITEMATTRIBUTECOORDINATET_H
#define CT_STDITEMATTRIBUTECOORDINATET_H

#include "ct_attributes/abstract/ct_abstractitemattributet.h"
#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

/**
 * @brief An item attribute that contains a data that will not changed. Use
 *        this class with a CT_IACreator :
 *
 *        CT_AbstractItemAttribute *attribute = CT_IACreator::createCoordinate(DEF_ATTRIBUTE_MODEL_NAME, CATEGORY_NAME, result, dataY, dataX, dataY, dataZ);
 */
template <typename VType>
class CT_StdItemAttributeCoordinateT : public CT_AbstractItemAttributeT<VType>
{
public:
    CT_StdItemAttributeCoordinateT(const CT_OutAbstractItemAttributeModel *model,
                                   const CT_AbstractCategory *category,
                                   const CT_AbstractResult *result,
                                   const VType &dataX,
                                   const VType &dataY,
                                   const VType &dataZ);

    CT_StdItemAttributeCoordinateT(const QString &modelName,
                                   const QString &categoryName,
                                   const CT_AbstractResult *result,
                                   const VType &dataX,
                                   const VType &dataY,
                                   const VType &dataZ);

    /**
     * @brief Use only this constructor for model !
     */
    CT_StdItemAttributeCoordinateT(const QString &categoryName);

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

    VType                                       m_datas[3];
    CT_AbstractCoordinateSystem::DataToConvert  m_convert;
    int                                         m_index;

    void initConvertionVariable();
};

#include "ct_attributes/ct_stditemattributecoordinatet.hpp"

#endif // CT_STDITEMATTRIBUTECOORDINATET_H
