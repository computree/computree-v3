#ifndef CT_STANDARDCLOUDOSGT_HPP
#define CT_STANDARDCLOUDOSGT_HPP

#include "ct_cloud/ct_standardcloudosgt.h"
#include <QDebug>

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::CT_StandardCloudOsgT(const size_t &initialSize) : CT_AbstractCloudT<T>()
{
    m_array = new AType(initialSize);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
typename CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::Iterator CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::begin()
{
    return m_array->begin();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
typename CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::Const_Iterator CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::constBegin() const
{
    return m_array->begin();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
typename CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::Iterator CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::end()
{
    return m_array->end();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
typename CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::Const_Iterator CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::constEnd() const
{
    return m_array->end();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
size_t CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::size() const
{
    return m_array->size();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
size_t CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::memoryUsed() const
{
    return size() * sizeof(T);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
T& CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::operator[](const size_t &index)
{
    return pTAt(index);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
const T& CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::operator[](const size_t &index) const
{
    return pTAt(index);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
typename CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::AType* CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::osgArray()
{
    return m_array.get();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
void CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::erase(const size_t &beginIndex, const size_t &sizes)
{
    size_t endIndex = beginIndex+sizes;
    size_t cpySize = size()-(beginIndex+sizes);

    if(cpySize > 0)
    {
        T *data = m_array->asVector().data();

        T *dst = data+beginIndex;
        T *src = data+endIndex;

        memcpy(dst, src, sizeof(T)*cpySize);
    }

    resize(size()-sizes);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
void CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::resize(const size_t &newSize)
{
    m_array->resize(newSize);
    m_array->asVector().shrink_to_fit();
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
void CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::internalCopyData(const size_t &destIndex, const size_t &srcIndex, const size_t &size)
{
    T *data = m_array->asVector().data();

    T *dst = data+destIndex;
    T *src = data+srcIndex;

    memcpy(dst, src, sizeof(T)*size);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
void CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::copyDataFromTo(T* src, const size_t &destIndex, const size_t &size)
{
    T *data = m_array->asVector().data();
    T *dst = data+destIndex;

    memcpy(dst, src, sizeof(T)*size);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
CT_AbstractCloud* CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::copy() const
{
    size_t s = size();

    CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType> *cloud = new CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>(s);

    for(size_t i=0; i<s; ++i)
        (*cloud)[i] = m_array->at(i);

    return cloud;
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
void CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::addT(const T &val)
{
    m_array->push_back(val);
}

template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
T& CT_StandardCloudOsgT<T, ARRAYTYPE, DataSize, DataType>::addT()
{
    size_t l = size();
    resize(l+1);
    return pTAt(l);
}

#endif // CT_STANDARDCLOUDOSGT_HPP
