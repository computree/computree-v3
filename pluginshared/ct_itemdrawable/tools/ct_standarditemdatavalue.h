#ifndef CT_STANDARDITEMDATAVALUE_H
#define CT_STANDARDITEMDATAVALUE_H

#include "ct_itemdrawable/tools/ct_abstractitemdatavalue.h"

#include "ct_itemdrawable/tools/ct_itemplateddata1darray.h"
#include "ct_itemdrawable/tools/ct_itemplateddata2darray.h"
#include "ct_itemdrawable/tools/ct_itemplateddata3darray.h"
#include "ct_itemdrawable/tools/ct_itemplateddata4darray.h"

#include "ct_global/ct_context.h"

#include <typeinfo>

#define CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(DATA_TYPE) \
void setValue(const DATA_TYPE &data); \
void setValue(const QList<DATA_TYPE> &list, bool memoryCopy = false); \
void setValue(const DATA_TYPE *array, const size_t &size, bool memoryCopy = false); \
void setValue(const DATA_TYPE *array, const size_t &xSize, const size_t &ySize, bool memoryCopy = false); \
void setValue(const CT_ITemplatedData1DArray<DATA_TYPE> *array, bool deleteInDestructor = false); \
void setValue(const CT_ITemplatedData2DArray<DATA_TYPE> *array, bool deleteInDestructor = false); \
void setValue(const CT_ITemplatedData3DArray<DATA_TYPE> *array, bool deleteInDestructor = false); \
void setValue(const CT_ITemplatedData4DArray<DATA_TYPE> *array, bool deleteInDestructor = false); \

#define CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE_CLOUD(CLOUD_TYPE, CLOUD_INDEX_TYPE) \
void setValue(const CLOUD_TYPE *c, const CLOUD_INDEX_TYPE *cIndex, const int &row);

#define CT_SPECIALIZE_DATA_TYPE_TO(SCALARTYPE, ISDVT_TYPE) \
template<> \
inline int CT_StandardItemDataValue::staticDataTypeToScalarType<SCALARTYPE>() { return ISDVT_TYPE; } \
\
template<> \
inline int CT_StandardItemDataValue::staticDataTypeToArrayTType< CT_ITemplatedData1DArray<SCALARTYPE>, SCALARTYPE >() { return ((int)IDVT_BOOL_1D_ARRAY) + ((int)ISDVT_TYPE); } \
\
template<> \
inline int CT_StandardItemDataValue::staticDataTypeToArrayTType< CT_ITemplatedData2DArray<SCALARTYPE>, SCALARTYPE >() { return ((int)IDVT_BOOL_2D_ARRAY) + ((int)ISDVT_TYPE); } \
\
template<> \
inline int CT_StandardItemDataValue::staticDataTypeToArrayTType< CT_ITemplatedData3DArray<SCALARTYPE>, SCALARTYPE >() { return ((int)IDVT_BOOL_3D_ARRAY) + ((int)ISDVT_TYPE); } \
\
template<> \
inline int CT_StandardItemDataValue::staticDataTypeToArrayTType< CT_ITemplatedData4DArray<SCALARTYPE>, SCALARTYPE >() { return ((int)IDVT_BOOL_4D_ARRAY) + ((int)ISDVT_TYPE); } \


class PLUGINSHAREDSHARED_EXPORT CT_StandardItemDataValue : public CT_AbstractItemDataValue
{
public:
    CT_StandardItemDataValue();
    ~CT_StandardItemDataValue();

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
                          bool *ok) const;

    size_t size() const;
    size_t otherSize(const int &dim) const;
    QString valueAtIndexToString(const size_t &index, bool *ok);
    void* valueAtIndexToScalar(const size_t &index, bool *ok);

    void setInvalid();
    void clear();

    bool memoryCopy() const;

    template<typename T>
    static inline int staticDataTypeToScalarType()
    {
        PS_LOG->addMessage(LogInterface::error, LogInterface::plugin, QString("staticDataTypeToScalarType unknown scalar type (") + typeid(T).name() + ") ===> CT_AbstractItemDataValue.h");
        return ISDVT_UNKNOWN;
    }

    template<class A, typename T>
    static inline int staticDataTypeToArrayTType()
    {
        PS_LOG->addMessage(LogInterface::error, LogInterface::plugin, QString("staticDataTypeToArrayTType unknown array type (") + typeid(T).name() + ") ===> CT_AbstractItemDataValue.h");
        return ISDVT_UNKNOWN;
    }

    template<typename T>
    static inline int staticDataTypeToCloudType()
    {
        PS_LOG->addMessage(LogInterface::error, LogInterface::plugin, QString("staticDataTypeToCloudType unknown cloud type (") + typeid(T).name() + ") ===> CT_AbstractItemDataValue.h");
        return ISDVT_UNKNOWN;
    }

    // DECLARE SET VALUE SCALAR, ARRAY, ETC...
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(bool)
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(int)

    #ifdef ENVIRONMENT32
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(quint64)
    #endif

    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(size_t)
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(double)
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(long double)
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(float)
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE(QString)

    // DECLARE SET VALUE CLOUD
    CT_DECLARE_STANDARD_ITEMDATAVALUE_SET_VALUE_CLOUD(CT_AbstractPointCloud,CT_AbstractCloudIndex)

private:
    CT_AbstractItemDataValue    *m_valueT;
    CT_AbstractPointCloud       *m_cloud;
    CT_AbstractCloudIndex       *m_cloudIndex;
    int                         m_dataIndex;
    double                      m_tmpDouble;
};

// SPECIALIZE DATA TYPE TO SCALAR, ARRAY, ETC...
CT_SPECIALIZE_DATA_TYPE_TO(bool, ISDVT_BOOL)
CT_SPECIALIZE_DATA_TYPE_TO(int, ISDVT_INT)

#ifdef ENVIRONMENT32
CT_SPECIALIZE_DATA_TYPE_TO(quint64, ISDVT_UINT64)
#endif

CT_SPECIALIZE_DATA_TYPE_TO(size_t, ISDVT_SIZE_T)
CT_SPECIALIZE_DATA_TYPE_TO(double, ISDVT_DOUBLE)
CT_SPECIALIZE_DATA_TYPE_TO(long double, ISDVT_LONG_DOUBLE)
CT_SPECIALIZE_DATA_TYPE_TO(float, ISDVT_FLOAT)
CT_SPECIALIZE_DATA_TYPE_TO(QString, ISDVT_STRING)

// SPECIALIZE DATA TYPE TO CLOUD

template<>
inline int CT_StandardItemDataValue::staticDataTypeToCloudType<CT_AbstractPointCloud>() { return IDVT_POINT_CLOUD_DATA; }

#endif // CT_STANDARDITEMDATAVALUE_H
