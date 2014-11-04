#ifndef CT_POINTSATTRIBUTESSCALARMASKT_H
#define CT_POINTSATTRIBUTESSCALARMASKT_H

#include "ct_cloud/ct_standardcloudstdvectort.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_attributes/abstract/ct_abstractattributesscalar.h"
#include "ct_math/ct_math.h"

#include <typeinfo>

/**
 *  This class is like a CT_PointsAttributesScalarTemplated but the difference is that the cloud of information
 *  can mask the information of the element in cloud to get the real information.
 *
 *  @example You must create a structure with the 'MASK' typedef :
 *
 *           struct myStruct { // information of a point
 *              typedef quint8 MASK; // typedef of the mask type // OBLIGATORY in your struct and named 'MASK'
 *
 *              quint8 entire;   // first element is contained in 3 bits and second element in 5 bits // OBLIGATORY in your struct and named 'entire'
 *           };
 *
 *           // This vector use the structure to contains datas for all points
 *           CT_StandardCloudStdVectorT<myStruct> vector(size);
 *
 *           // Create two CT_PointsAttributesScalarMaskT
 *           // The first must contains a pointer to the vector and delete it when it will be destroyed from memory
 *           // The second contains also a pointer to the vector but doesn't delete it when it will be destroyed from memory
 *           CT_PointsAttributesScalarMaskT<myStruct> firstElement(model, result, 224, 5, vector, true);    // the mask is '0b11100000' in binary, and shift right is 5
 *           CT_PointsAttributesScalarMaskT<myStruct> secondElement(model, result, 31, 0, vector, false);     // the mask is '0b00011111' in binary, and shift right is 0
 *
 *           // Set 6 for the first element and 4 for the second element
 *           myStruct tmp;
 *           tmp.entire = 196; (egual 6 for the first and 4 for the second if use the mask)
 *
 *           vector.tAt(0) = tmp;
 *
 *           double first = firstElement.dValueAt(0);       // first == 6
 *           double second = secondElement.dValueAt(0);     // second == 4
 */
template<typename StructType>
class CT_PointsAttributesScalarMaskT : public CT_AbstractPointAttributesScalar, public CT_AbstractAttributesScalar
{
public:

    typedef typename StructType::MASK StructMASK;

    CT_PointsAttributesScalarMaskT();
    CT_PointsAttributesScalarMaskT(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result,
                                   StructMASK mask,
                                   quint16 shiftRight,
                                   CT_StandardCloudStdVectorT<StructType> *collection,
                                   CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                   bool autoDeleteCollection = true);

    CT_PointsAttributesScalarMaskT(const QString &modelName,
                                   const CT_AbstractResult *result,
                                   StructMASK mask,
                                   quint16 shiftRight,
                                   CT_StandardCloudStdVectorT<StructType> *collection,
                                   CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                   bool autoDeleteCollection = true);

    CT_PointsAttributesScalarMaskT(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result,
                                   StructMASK mask,
                                   quint16 shiftRight,
                                   const StructMASK &min,
                                   const StructMASK &max,
                                   CT_StandardCloudStdVectorT<StructType> *collection,
                                   CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                   bool autoDeleteCollection = true);

    CT_PointsAttributesScalarMaskT(const QString &modelName,
                                   const CT_AbstractResult *result,
                                   StructMASK mask,
                                   quint16 shiftRight,
                                   const StructMASK &min,
                                   const StructMASK &max,
                                   CT_StandardCloudStdVectorT<StructType> *collection,
                                   CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir,
                                   bool autoDeleteCollection = true);

    virtual ~CT_PointsAttributesScalarMaskT();

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    double dMin() const;
    double dMax() const;

    double dValueAt(const size_t &index) const;

    size_t attributesSize() const;

    /*StructMASK min() const;
    StructMASK max() const;

    void setMin(const StructMASK &min);
    void setMax(const StructMASK &max);*/

    /**
     * @brief Returns the collection used by this object
     */
    CT_StandardCloudStdVectorT<StructType>* collection() const;

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    CT_StandardCloudStdVectorT<StructType>              *m_collection;
    bool                                                m_autoDeleteCollection;
    StructMASK                                          m_mask;
    quint16                                             m_shiftRight;
    StructMASK                                          m_min;
    StructMASK                                          m_max;

protected:
    static bool staticCompareStruct(const StructType &a, const StructType &b, void *context);
};

#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.hpp"

#endif // CT_POINTSATTRIBUTESSCALARMASKT_H
