#include "ct_standarditemdatavalue.h"

#include "ct_itemdrawable/tools/ct_standarditemdatavaluet.h"

#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"

#define CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(DATA_TYPE) \
void CT_StandardItemDataValue::setValue(const DATA_TYPE &data) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(data); \
}\
\
void CT_StandardItemDataValue::setValue(const QList<DATA_TYPE> &list, bool memoryCopy) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(list, memoryCopy); \
}\
\
void CT_StandardItemDataValue::setValue(const DATA_TYPE *array, const size_t &size, bool memoryCopy) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(array, size, memoryCopy); \
}\
\
void CT_StandardItemDataValue::setValue(const DATA_TYPE *array, const size_t &xSize, const size_t &ySize, bool memoryCopy) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(array, xSize, ySize, memoryCopy); \
}\
\
void CT_StandardItemDataValue::setValue(const CT_ITemplatedData1DArray<DATA_TYPE> *array, bool deleteInDestructor) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(array, deleteInDestructor); \
}\
\
void CT_StandardItemDataValue::setValue(const CT_ITemplatedData2DArray<DATA_TYPE> *array, bool deleteInDestructor) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(array, deleteInDestructor); \
}\
\
void CT_StandardItemDataValue::setValue(const CT_ITemplatedData3DArray<DATA_TYPE> *array, bool deleteInDestructor) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(array, deleteInDestructor); \
}\
\
void CT_StandardItemDataValue::setValue(const CT_ITemplatedData4DArray<DATA_TYPE> *array, bool deleteInDestructor) \
{ \
    clear(); \
    m_valueT = new CT_StandardItemDataValueT<DATA_TYPE>(array, deleteInDestructor); \
}

#define CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE_CLOUD(CLOUD_TYPE, CLOUD_INDEX_TYPE) \
void CT_StandardItemDataValue::setValue(const CLOUD_TYPE *c, const CLOUD_INDEX_TYPE *cIndex, const int &row) \
{ \
    clear(); \
    m_cloud = (CLOUD_TYPE*)c; \
    m_cloudIndex = (CLOUD_INDEX_TYPE*)cIndex; \
    m_dataIndex = row; \
}

CT_StandardItemDataValue::CT_StandardItemDataValue() : CT_AbstractItemDataValue()
{
    m_valueT = NULL;
    m_cloud = NULL;
    m_cloudIndex = NULL;
    m_dataIndex = -1;
}

CT_StandardItemDataValue::~CT_StandardItemDataValue()
{
    clear();
}

bool CT_StandardItemDataValue::isValid() const
{
    if(m_cloudIndex != NULL)
        return true;

    if(m_valueT == NULL)
        return false;

    return m_valueT->isValid();
}

IItemDataValue::ItemDataValueType CT_StandardItemDataValue::type() const
{
    if(m_cloudIndex != NULL)
        return IItemDataValue::IDVT_POINT_CLOUD_DATA;

    if(m_valueT == NULL)
        return IItemDataValue::IDVT_UNKNOWN;

    return m_valueT->type();
}

IItemDataValue::ItemScalarDataValueType CT_StandardItemDataValue::scalarType() const
{
    if(m_valueT == NULL)
        return IItemDataValue::ISDVT_UNKNOWN;

    return m_valueT->scalarType();
}

bool CT_StandardItemDataValue::toBool(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return false;

    return m_valueT->toBool(ok);
}

int CT_StandardItemDataValue::toInt(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return 0;

    return m_valueT->toInt(ok);
}

size_t CT_StandardItemDataValue::toSizeT(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return 0;

    return m_valueT->toSizeT(ok);
}

quint64 CT_StandardItemDataValue::toUInt64(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return 0;

    return m_valueT->toUInt64(ok);
}

double CT_StandardItemDataValue::toDouble(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return 0;

    return m_valueT->toDouble(ok);
}

long double CT_StandardItemDataValue::toLongDouble(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return 0;

    return m_valueT->toLongDouble(ok);
}

float CT_StandardItemDataValue::toFloat(bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return 0;

    return m_valueT->toFloat(ok);
}

QString CT_StandardItemDataValue::toString(bool *ok) const
{
    if(ok != NULL)
        *ok = true;

    if(m_cloudIndex != NULL)
        return QString("Cloud : %1 elements").arg(size());

    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return QString();

    return m_valueT->toString(ok);
}

void CT_StandardItemDataValue::toPointCloudData(CT_AbstractPointCloud **pc,
                                                CT_AbstractCloudIndex **pcIndex,
                                                int &dataIndex,
                                                bool *ok) const
{
    if(ok != NULL)
        *ok = false;

    if(m_cloudIndex != NULL)
    {
        *pc = m_cloud;
        *pcIndex = m_cloudIndex;
        dataIndex = m_dataIndex;

        if(ok != NULL)
            *ok = true;

        return;
    }
}

size_t CT_StandardItemDataValue::size() const
{
    if(m_cloudIndex != NULL)
        return m_cloudIndex->size();

    if(m_valueT == NULL)
        return 0;

    return m_valueT->size();
}

size_t CT_StandardItemDataValue::otherSize(const int &dim) const
{
    if(m_valueT == NULL)
        return 0;

    return m_valueT->otherSize(dim);
}

QString CT_StandardItemDataValue::valueAtIndexToString(const size_t &index, bool *ok)
{

    if(m_cloudIndex != NULL)
    {
        if(ok != NULL)
            *ok = true;

        if(m_dataIndex == 0)
            return CT_NumericToStringConversionT<double>::toString(m_cloud->tAt((*m_cloudIndex)[index]).getX(), 'f', 6);
        else if(m_dataIndex == 1)
            return CT_NumericToStringConversionT<double>::toString(m_cloud->tAt((*m_cloudIndex)[index]).getY(), 'f', 6);
        else if(m_dataIndex == 2)
            return CT_NumericToStringConversionT<double>::toString(m_cloud->tAt((*m_cloudIndex)[index]).getZ(), 'f', 6);
    }

    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return QString();

    return m_valueT->valueAtIndexToString(index, ok);
}

void* CT_StandardItemDataValue::valueAtIndexToScalar(const size_t &index, bool *ok)
{
    if(m_cloudIndex != NULL)
    {
        if(ok != NULL)
            *ok = true;

        if(m_dataIndex == 0)
            m_tmpDouble = m_cloud->tAt((*m_cloudIndex)[index]).getX();
        else if(m_dataIndex == 1)
            m_tmpDouble = m_cloud->tAt((*m_cloudIndex)[index]).getY();
        else if(m_dataIndex == 2)
            m_tmpDouble = m_cloud->tAt((*m_cloudIndex)[index]).getZ();

        return &m_tmpDouble;
    }

    if(ok != NULL)
        *ok = false;

    if(m_valueT == NULL)
        return NULL;

    return m_valueT->valueAtIndexToScalar(index, ok);
}

void CT_StandardItemDataValue::setInvalid()
{
    clear();
}

void CT_StandardItemDataValue::clear()
{
    delete m_valueT;
    m_valueT = NULL;

    m_cloud = NULL;
    m_cloudIndex = NULL;
    m_dataIndex = -1;
}

bool CT_StandardItemDataValue::memoryCopy() const
{
    if(m_valueT == NULL)
        return false;

    return m_valueT->memoryCopy();
}

// SCALAR
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(bool)
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(int)

#ifdef ENVIRONMENT32
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(quint64)
#endif

CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(size_t)
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(double)
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(long double)
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(float)
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE(QString)

// CLOUD
CT_IMPLEMENT_STANDARD_ITEMDATAVALUE_SET_VALUE_CLOUD(CT_AbstractPointCloud,CT_AbstractCloudIndex)
