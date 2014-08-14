#ifndef CT_STANDARDITEMDATAVALUET_HPP
#define CT_STANDARDITEMDATAVALUET_HPP

#include "ct_itemdrawable/tools/ct_standarditemdatavaluet.h"

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT() : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const DataType &data) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(data);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const QList<DataType> &list, bool memoryCopy) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(list, memoryCopy);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const DataType *array, const size_t &size, bool memoryCopy) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(array, size, memoryCopy);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const DataType *array, const size_t &xSize, const size_t &ySize, bool memoryCopy) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(array, xSize, ySize, memoryCopy);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const CT_ITemplatedData1DArray<DataType> *array, bool deleteInDestructor) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(array, deleteInDestructor);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const CT_ITemplatedData2DArray<DataType> *array, bool deleteInDestructor) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(array, deleteInDestructor);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const CT_ITemplatedData3DArray<DataType> *array, bool deleteInDestructor) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(array, deleteInDestructor);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::CT_StandardItemDataValueT(const CT_ITemplatedData4DArray<DataType> *array, bool deleteInDestructor) : CT_AbstractItemDataValue()
{
    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    setValue(array, deleteInDestructor);
}

template<typename DataType>
CT_StandardItemDataValueT<DataType>::~CT_StandardItemDataValueT()
{
    clear();
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValid() const
{
    return scalarType() != IItemDataValue::ISDVT_UNKNOWN;
}

template<typename DataType>
IItemDataValue::ItemDataValueType CT_StandardItemDataValueT<DataType>::type() const
{
    if(isValueTypeCustom())
        return (IItemDataValue::ItemDataValueType)(_dataType - (IItemDataValue::IDVT_UNKNOWN+1));

    return (IItemDataValue::ItemDataValueType)_dataType;
}

template<typename DataType>
IItemDataValue::ItemScalarDataValueType CT_StandardItemDataValueT<DataType>::scalarType() const
{
    int t = type();

    if(t < IItemDataValue::IDVT_UNKNOWN_CLOUD)
    {
        while(t > IDVT_UNKNOWN_SCALAR)
        {
            t -= (IDVT_UNKNOWN_SCALAR+1);
        }

        return (IItemDataValue::ItemScalarDataValueType)t;
    }

    return IItemDataValue::ISDVT_UNKNOWN;
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::toBool(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    if(type() != IItemDataValue::IDVT_BOOL)
    {
        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
int CT_StandardItemDataValueT<DataType>::toInt(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    if(type() != IItemDataValue::IDVT_INT)
    {
        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
size_t CT_StandardItemDataValueT<DataType>::toSizeT(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    IItemDataValue::ItemDataValueType t = type();

    if(t != IItemDataValue::IDVT_SIZE_T)
    {
        if(t == IItemDataValue::IDVT_INT)
        {
            int val = toInt(ok);

            if(val >= 0)
                return val;
        }
        #ifdef ENVIRONMENT64
        else if(t == IItemDataValue::IDVT_UINT64)
            return toUInt64(ok);
        #endif

        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
quint64 CT_StandardItemDataValueT<DataType>::toUInt64(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    IItemDataValue::ItemDataValueType t = type();

    if(t != IItemDataValue::IDVT_UINT64)
    {
        if(t == IItemDataValue::IDVT_INT)
        {
            int val = toInt(ok);

            if(val >= 0)
                return val;
        }
        #ifdef ENVIRONMENT64
        else if(t == IItemDataValue::IDVT_SIZE_T)
            return toSizeT(ok);
        #endif

        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
double CT_StandardItemDataValueT<DataType>::toDouble(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    IItemDataValue::ItemDataValueType t = type();

    if(t != IItemDataValue::IDVT_DOUBLE)
    {
        if(t == IItemDataValue::IDVT_INT)
            return toInt(ok);
        else if(t == IItemDataValue::IDVT_SIZE_T)
            return toSizeT(ok);
        else if(t == IItemDataValue::IDVT_FLOAT)
            return toFloat(ok);

        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
long double CT_StandardItemDataValueT<DataType>::toLongDouble(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    IItemDataValue::ItemDataValueType t = type();

    if(t != IItemDataValue::IDVT_LONG_DOUBLE)
    {
        if(t == IItemDataValue::IDVT_INT)
            return toInt(ok);
        else if(t == IItemDataValue::IDVT_UINT64)
            return toUInt64(ok);
        else if(t == IItemDataValue::IDVT_SIZE_T)
            return toSizeT(ok);
        else if(t == IItemDataValue::IDVT_FLOAT)
            return toFloat(ok);
        else if(t == IItemDataValue::IDVT_DOUBLE)
            return toDouble(ok);

        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
float CT_StandardItemDataValueT<DataType>::toFloat(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    IItemDataValue::ItemDataValueType t = type();

    if(t != IItemDataValue::IDVT_FLOAT)
    {
        if(t == IItemDataValue::IDVT_INT)
            return toInt(ok);
        else if(t == IItemDataValue::IDVT_UINT64)
            return toUInt64(ok);
        else if(t == IItemDataValue::IDVT_SIZE_T)
            return toSizeT(ok);
        else if(t == IItemDataValue::IDVT_DOUBLE)
            return toDouble(ok);

        if(ok != NULL)
            *ok = false;
    }

    return m_data;
}

template<typename DataType>
QString CT_StandardItemDataValueT<DataType>::toString(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    if(isValueTypeScalar())
    {
        return scalarToString(m_data);
    }
    else if(isValueTypeList())
    {
        return listToString();
    }
    else if(isValueType1DArray())
    {
        return array1DToString();
    }
    else if(isValueType2DArray())
    {
        return array2DToString();
    }
    else if(isValueType3DArray())
    {
        return array3DToString();
    }

    return QString();
}

template<typename DataType>
size_t CT_StandardItemDataValueT<DataType>::size() const
{
    if(!isValid())
        return 0;

    if(isValueTypeScalar())
        return 1;

    if(isValueType2DArray())
        return _sizes.at(0) * _sizes.at(1);

    if(isValueType3DArray())
        return _sizes.at(0) * _sizes.at(1) * _sizes.at(2);

    if(!_sizes.isEmpty())
        return _sizes.first();

    return 0;
}

template<typename DataType>
size_t CT_StandardItemDataValueT<DataType>::otherSize(const int &dim) const
{
    if((dim-1) < _sizes.size())
        return _sizes.at(dim-1);

    if((dim == 1) && !isValueTypeArray())
        return size();

    return 0;
}

template<typename DataType>
QString CT_StandardItemDataValueT<DataType>::valueAtIndexToString(const size_t &index, bool *ok)
{
    if(ok != NULL)
        *ok = false;

    if(!isValid())
        return QString("INVALID");

    if(isValueTypeScalar())
        return toString(ok);

    if(isValueTypeCustom())
    {
        if(isValueType1DArray())
        {
            if(ok != NULL)
                *ok = true;

            return scalarToString(((CT_ITemplatedData1DArray<DataType>*)m_ptr)->dataFromArray(index));
        }
        else if(isValueType2DArray())
        {
            if(ok != NULL)
                *ok = true;

            return scalarToString(((CT_ITemplatedData2DArray<DataType>*)m_ptr)->dataFromArray(index));
        }
        else if(isValueType3DArray())
        {
            if(ok != NULL)
                *ok = true;

            return scalarToString(((CT_ITemplatedData3DArray<DataType>*)m_ptr)->dataFromArray(index));
        }
        else if(isValueType4DArray())
        {
            if(ok != NULL)
                *ok = true;

            return scalarToString(((CT_ITemplatedData4DArray<DataType>*)m_ptr)->dataFromArray(index));
        }
        return QString("INVALID");
    }

    if(isValueTypeList())
    {
        if(ok != NULL)
            *ok = true;

        return scalarToString(((QList<DataType>*)m_ptr)->at(index));
    }

    if(isValueType1DArray() || isValueType2DArray() || isValueType3DArray())
    {
        if(ok != NULL)
            *ok = true;

        return scalarToString(((DataType*)m_ptr)[index]);
    }

    return QString("INVALID");
}

template<typename DataType>
void* CT_StandardItemDataValueT<DataType>::valueAtIndexToScalar(const size_t &index, bool *ok)
{
    if(ok != NULL)
        *ok = false;

    if(!isValid())
        return NULL;

    if(ok != NULL)
        *ok = true;

    if(isValueTypeScalar())
    {
        return &(m_data);
    }
    else if(isValueTypeCustom())
    {
        if(isValueType1DArray())
        {
            m_tmpData = ((CT_ITemplatedData1DArray<DataType>*)m_ptr)->dataFromArray(index);
            return &m_tmpData;
        }
        else if(isValueType2DArray())
        {
            m_tmpData = ((CT_ITemplatedData2DArray<DataType>*)m_ptr)->dataFromArray(index);
            return &m_tmpData;
        }
        else if(isValueType3DArray())
        {
            m_tmpData = ((CT_ITemplatedData3DArray<DataType>*)m_ptr)->dataFromArray(index);
            return &m_tmpData;
        }
        else if(isValueType4DArray())
        {
            m_tmpData = ((CT_ITemplatedData4DArray<DataType>*)m_ptr)->dataFromArray(index);
            return &m_tmpData;
        }
        if(ok != NULL)
            *ok = false;

        return NULL;
    }
    else if(isValueTypeList())
    {
        m_tmpData = ((QList<DataType>*)m_ptr)->at(index);
        return &m_tmpData;
    }
    else if(isValueTypeArray())
    {
        return &(((DataType*)m_ptr)[index]);
    }

    if(ok != NULL)
        *ok = false;

    return NULL;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const DataType &data)
{
    clear();
    _dataType = dataTypeToScalarType();
    m_data = data;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const QList<DataType> &list, bool memoryCopy)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_LIST;

    if(memoryCopy)
        m_ptr = new QList<DataType>(list);
    else
        m_ptr = (QList<DataType>*)&list;

    _sizes << list.size();
    _memoryCopy = memoryCopy;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const DataType *array, const size_t &size, bool memoryCopy)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_1D_ARRAY;

    if(memoryCopy)
    {
        m_ptr = new DataType[size];
        memcpy(m_ptr, array, size*sizeof(DataType));
    }
    else
    {
        m_ptr = (void*)array;
    }

    _sizes << size;
    _memoryCopy = memoryCopy;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const DataType *array, const size_t &xSize, const size_t &ySize, bool memoryCopy)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_2D_ARRAY;

    if(memoryCopy)
    {
        m_ptr = new DataType[xSize*ySize];
        memcpy(m_ptr, array, xSize*ySize*sizeof(DataType));
    }
    else
    {
        m_ptr = (void*)array;
    }

    _sizes << xSize;
    _sizes << ySize;
    _memoryCopy = memoryCopy;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const CT_ITemplatedData1DArray<DataType> *array, bool deleteInDestructor)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_1D_ARRAY + (IItemDataValue::IDVT_UNKNOWN+1);
    m_ptr = (CT_ITemplatedData1DArray<DataType>*)array;

    _sizes << array->arraySize();
    _memoryCopy = deleteInDestructor;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const CT_ITemplatedData2DArray<DataType> *array, bool deleteInDestructor)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_2D_ARRAY + (IItemDataValue::IDVT_UNKNOWN+1);
    m_ptr = (CT_ITemplatedData2DArray<DataType>*)array;

    _sizes << array->xArraySize();
    _sizes << array->yArraySize();
    _memoryCopy = deleteInDestructor;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const CT_ITemplatedData3DArray<DataType> *array, bool deleteInDestructor)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_3D_ARRAY + (IItemDataValue::IDVT_UNKNOWN+1);
    m_ptr = (CT_ITemplatedData3DArray<DataType>*)array;

    _sizes << array->xArraySize();
    _sizes << array->yArraySize();
    _sizes << array->zArraySize();
    _memoryCopy = deleteInDestructor;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setValue(const CT_ITemplatedData4DArray<DataType> *array, bool deleteInDestructor)
{
    clear();
    _dataType = dataTypeToScalarType() + IItemDataValue::IDVT_BOOL_4D_ARRAY + (IItemDataValue::IDVT_UNKNOWN+1);
    m_ptr = (CT_ITemplatedData4DArray<DataType>*)array;

    _sizes << array->wArraySize();
    _sizes << array->xArraySize();
    _sizes << array->yArraySize();
    _sizes << array->zArraySize();
    _memoryCopy = deleteInDestructor;
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::setInvalid()
{
    clear();
}

template<typename DataType>
void CT_StandardItemDataValueT<DataType>::clear()
{
    if(memoryCopy())
    {
        if(isValueTypeCustom())
        {
            if(isValueType1DArray())
            {
                delete ((CT_ITemplatedData1DArray<DataType>*)m_ptr);
            }
            else if(isValueType2DArray())
            {
                delete ((CT_ITemplatedData2DArray<DataType>*)m_ptr);
            }
            else if(isValueType3DArray())
            {
                delete ((CT_ITemplatedData3DArray<DataType>*)m_ptr);
            }
            else if(isValueType4DArray())
            {
                delete ((CT_ITemplatedData4DArray<DataType>*)m_ptr);
            }
        }
        else if(isValueTypeList())
        {
            delete ((QList<DataType>*)m_ptr);
        }
        else if(isValueTypeArray())
        {
            delete [] ((DataType*)m_ptr);
        }
        else
        {
            delete (DataType*)m_ptr;
        }
    }

    _dataType = IItemDataValue::IDVT_UNKNOWN;
    _memoryCopy = false;
    _sizes.clear();
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::memoryCopy() const
{
    return _memoryCopy;
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueTypeScalar() const
{
    IItemDataValue::ItemDataValueType t = type();

    return (t >= IItemDataValue::IDVT_BOOL)
            && (t < IDVT_UNKNOWN_LIST);
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueTypeList() const
{
    IItemDataValue::ItemDataValueType t = type();

    return (t >= IItemDataValue::IDVT_BOOL_LIST)
            && (t < IItemDataValue::IDVT_UNKNOWN_1D_ARRAY);
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueTypeArray() const
{
    return isValueType1DArray() || isValueType2DArray() || isValueType3DArray();
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueType1DArray() const
{
    IItemDataValue::ItemDataValueType t = type();

    return (t >= IItemDataValue::IDVT_BOOL_1D_ARRAY)
            && (t < IItemDataValue::IDVT_UNKNOWN_2D_ARRAY);
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueType2DArray() const
{
    IItemDataValue::ItemDataValueType t = type();

    return (t >= IItemDataValue::IDVT_BOOL_2D_ARRAY)
            && (t < IItemDataValue::IDVT_UNKNOWN_3D_ARRAY);
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueType3DArray() const
{
    IItemDataValue::ItemDataValueType t = type();

    return (t >= IItemDataValue::IDVT_BOOL_3D_ARRAY)
            && (t < IItemDataValue::IDVT_BOOL_4D_ARRAY);
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueType4DArray() const
{
    IItemDataValue::ItemDataValueType t = type();

    return (t >= IItemDataValue::IDVT_BOOL_4D_ARRAY)
            && (t < IItemDataValue::IDVT_UNKNOWN_CLOUD);
}

template<typename DataType>
bool CT_StandardItemDataValueT<DataType>::isValueTypeCustom() const
{
    return (_dataType > IDVT_UNKNOWN);
}

#endif // CT_STANDARDITEMDATAVALUET_HPP
