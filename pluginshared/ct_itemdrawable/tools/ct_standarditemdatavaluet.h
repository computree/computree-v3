#ifndef CT_STANDARDITEMDATAVALUET_H
#define CT_STANDARDITEMDATAVALUET_H

#include "ct_itemdrawable/tools/ct_standarditemdatavalue.h"

#include "ct_tools/ct_numerictostringconversiont.h"

template<typename DataType>
class CT_StandardItemDataValueT : public CT_AbstractItemDataValue
{
public:
    CT_StandardItemDataValueT();
    CT_StandardItemDataValueT(const DataType &data);
    CT_StandardItemDataValueT(const QList<DataType> &list, bool memoryCopy = false);
    CT_StandardItemDataValueT(const DataType *array, const size_t &size, bool memoryCopy = false);
    CT_StandardItemDataValueT(const DataType *array, const size_t &xSize, const size_t &ySize, bool memoryCopy = false);
    CT_StandardItemDataValueT(const CT_ITemplatedData1DArray<DataType> *array, bool deleteInDestructor = false);
    CT_StandardItemDataValueT(const CT_ITemplatedData2DArray<DataType> *array, bool deleteInDestructor = false);
    CT_StandardItemDataValueT(const CT_ITemplatedData3DArray<DataType> *array, bool deleteInDestructor = false);
    CT_StandardItemDataValueT(const CT_ITemplatedData4DArray<DataType> *array, bool deleteInDestructor = false);

    ~CT_StandardItemDataValueT();

    // IItemDataValue
    bool isValid() const;

    IItemDataValue::ItemDataValueType type() const;
    IItemDataValue::ItemScalarDataValueType scalarType() const;

    bool toBool(bool *ok) const;
    int toInt(bool *ok) const;
    size_t toSizeT(bool *ok) const;
    quint64 toUInt64(bool *ok) const;
    double toDouble(bool *ok) const;
    long double toLongDouble(bool *ok) const;
    float toFloat(bool *ok) const;
    QString toString(bool *ok) const;
    void toPointCloudData(CT_AbstractPointCloud **pc,
                          CT_AbstractCloudIndex **pcIndex,
                          int &dataIndex,
                          bool *ok) const { Q_UNUSED(pc) Q_UNUSED(pcIndex) Q_UNUSED(dataIndex) Q_UNUSED(ok) return; }

    size_t size() const;
    size_t otherSize(const int &dim) const;
    QString valueAtIndexToString(const size_t &index, bool *ok);
    void* valueAtIndexToScalar(const size_t &index, bool *ok);

    void setValue(const DataType &data);
    void setValue(const QList<DataType> &list, bool memoryCopy = false);
    void setValue(const DataType *array, const size_t &size, bool memoryCopy = false);
    void setValue(const DataType *array, const size_t &xSize, const size_t &ySize, bool memoryCopy = false);
    void setValue(const CT_ITemplatedData1DArray<DataType> *array, bool deleteInDestructor = false);
    void setValue(const CT_ITemplatedData2DArray<DataType> *array, bool deleteInDestructor = false);
    void setValue(const CT_ITemplatedData3DArray<DataType> *array, bool deleteInDestructor = false);
    void setValue(const CT_ITemplatedData4DArray<DataType> *array, bool deleteInDestructor = false);

    void setInvalid();

    void clear();

    bool memoryCopy() const;

    bool isValueTypeScalar() const;
    bool isValueTypeList() const;
    bool isValueTypeArray() const;
    bool isValueType1DArray() const;
    bool isValueType2DArray() const;
    bool isValueType3DArray() const;
    bool isValueType4DArray() const;
    bool isValueTypeCustom() const;

private:

    int                                 _dataType;

    DataType                            m_data;
    DataType                            m_tmpData;
    void                                *m_ptr;

    bool                                _memoryCopy;
    QList<size_t>                       _sizes;

    inline int dataTypeToScalarType() const
    {
        return CT_StandardItemDataValue::staticDataTypeToScalarType<DataType>();
    }

    inline QString scalarToString(const DataType &data) const
    {
        return CT_NumericToStringConversionT<DataType>::toString(data);
    }

    inline QString listToString() const
    {
        return QString("List of %1 elements").arg(((QList<DataType>*)m_ptr)->size());
    }

    inline QString array1DToString() const
    {
        return QString("1D Array of %1 elements").arg(_sizes.first());
    }

    inline QString array2DToString() const
    {
        return QString("2D Array of %1x%2 elements").arg(_sizes.first()).arg(_sizes.last());
    }

    inline QString array3DToString() const
    {
        return QString("3D Array of %1x%2x%3 elements").arg(_sizes.first()).arg(_sizes.at(1)).arg(_sizes.last());
    }

    inline QString array4DToString() const
    {
        return QString("4D Array of %1x%2x%3x%4 elements").arg(_sizes.at(0)).arg(_sizes.at(1)).arg(_sizes.at(2)).arg(_sizes.at(3));
    }
};

// specialization

template<>
bool CT_StandardItemDataValueT<QString>::toBool(bool *ok) const { if(ok != NULL) { *ok = false; } return false; }

template<>
int CT_StandardItemDataValueT<QString>::toInt(bool *ok) const { if(ok != NULL) { *ok = false; } return 0; }

template<>
size_t CT_StandardItemDataValueT<QString>::toSizeT(bool *ok) const { if(ok != NULL) { *ok = false; } return 0; }

template<>
quint64 CT_StandardItemDataValueT<QString>::toUInt64(bool *ok) const { if(ok != NULL) { *ok = false; } return 0; }

template<>
double CT_StandardItemDataValueT<QString>::toDouble(bool *ok) const { if(ok != NULL) { *ok = false; } return 0; }

template<>
long double CT_StandardItemDataValueT<QString>::toLongDouble(bool *ok) const { if(ok != NULL) { *ok = false; } return 0; }

template<>
float CT_StandardItemDataValueT<QString>::toFloat(bool *ok) const { if(ok != NULL) { *ok = false; } return 0; }

//

template<>
inline QString CT_StandardItemDataValueT<bool>::scalarToString(const bool &data) const { return (data ? "true" : "false"); }

template<>
inline QString CT_StandardItemDataValueT<QString>::scalarToString(const QString &data) const { return data; }

#include "ct_itemdrawable/tools/ct_standarditemdatavaluet.hpp"

#endif // CT_STANDARDITEMDATAVALUET_H
