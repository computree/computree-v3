#ifndef CT_STANDARDCLOUDSTDVECTORT_HPP
#define CT_STANDARDCLOUDSTDVECTORT_HPP

#include "ct_cloud/ct_standardcloudstdvectort.h"
/*#include <typeinfo>
#include "ct_global/ct_context.h"*/

template<typename T>
CT_StandardCloudStdVectorT<T>::CT_StandardCloudStdVectorT(size_t size) : CT_AbstractCloudT<T>()
{
#ifdef USE_PCL
    m_collection = boost::shared_ptr< pcl::PointCloud<T> >(new pcl::PointCloud<T>());
    m_collection->width = size;
    m_collection->height = 1;

    if(size > 0)
        m_collection->points.resize(size);
#else
    if(size > 0)
        m_collection.resize(size);
#endif
}

template<typename T>
typename CT_StandardCloudStdVectorT<T>::Iterator CT_StandardCloudStdVectorT<T>::begin()
{
    #ifdef USE_PCL
    return m_collection->points.begin();
    #else
    return m_collection.begin();
    #endif
}

template<typename T>
typename CT_StandardCloudStdVectorT<T>::Const_Iterator CT_StandardCloudStdVectorT<T>::constBegin() const
{
    #ifdef USE_PCL
    return m_collection->points.begin();
    #else
    return m_collection.begin();
    #endif
}

template<typename T>
typename CT_StandardCloudStdVectorT<T>::Iterator CT_StandardCloudStdVectorT<T>::end()
{
    #ifdef USE_PCL
    return m_collection->points.end();
    #else
    return m_collection.end();
    #endif
}

template<typename T>
typename CT_StandardCloudStdVectorT<T>::Const_Iterator CT_StandardCloudStdVectorT<T>::constEnd() const
{
    #ifdef USE_PCL
    return m_collection->points.end();
    #else
    return m_collection.end();
    #endif
}

template<typename T>
size_t CT_StandardCloudStdVectorT<T>::size() const
{
#ifdef USE_PCL
    return m_collection->points.size();
#else
    return m_collection.size();
#endif
}

template<typename T>
T& CT_StandardCloudStdVectorT<T>::operator[](const size_t &index)
{
    return pTAt(index);
}

template<typename T>
const T& CT_StandardCloudStdVectorT<T>::operator[](const size_t &index) const
{
    return pTAt(index);
}

template<typename T>
void CT_StandardCloudStdVectorT<T>::erase(const size_t &beginIndex, const size_t &sizes)
{
    size_t endIndex = beginIndex+sizes;
    size_t cpySize = size()-(beginIndex+sizes);

    if(cpySize > 0)
    {
        #ifdef USE_PCL
        T *data = m_collection->points.data();
        #else
        T *data = m_collection.data();
        #endif

        T *dst = data+beginIndex;
        T *src = data+endIndex;

        memcpy(dst, src, sizeof(T)*cpySize);
    }

    resize(size()-sizes);
}

template<typename T>
void CT_StandardCloudStdVectorT<T>::resize(const size_t &newSize)
{
#ifdef USE_PCL
    m_collection->width = newSize;
    m_collection->points.resize(newSize);
#else
    m_collection.resize(newSize);
#endif

    //PS_LOG->addDebugMessage(LogInterface::plugin, QString("cloud of type %1 resized to %2").arg(typeid(T).name()).arg(size()));
}

template<typename T>
void CT_StandardCloudStdVectorT<T>::internalCopyData(const size_t &destIndex, const size_t &srcIndex, const size_t &size)
{
    T *data = &pTAt(0);

    T *dst = data+destIndex;
    T *src = data+srcIndex;

    memcpy(dst, src, sizeof(T)*size);
}

template<typename T>
void CT_StandardCloudStdVectorT<T>::copyDataFromTo(T* src, const size_t &destIndex, const size_t &size)
{
    T *data = &pTAt(0);
    T *dst = data+destIndex;

    memcpy(dst, src, sizeof(T)*size);
}

#ifdef USE_PCL
template<typename T>
boost::shared_ptr< pcl::PointCloud<T> > CT_StandardCloudStdVectorT<T>::getPCLCloud() const
{
    return m_collection;
}
#endif

template<typename T>
CT_AbstractCloud* CT_StandardCloudStdVectorT<T>::copy() const
{
    size_t s = size();

    CT_StandardCloudStdVectorT<T> *cloud = new CT_StandardCloudStdVectorT<T>(s);

    for(size_t i=0; i<s; ++i)
    {
    #ifdef USE_PCL
        (*cloud)[i] = m_collection->points[i];
    #else
        (*cloud)[i] = m_collection[i];
    #endif
    }

    return cloud;
}

template<typename T>
void CT_StandardCloudStdVectorT<T>::addT(const T &val)
{
#ifdef USE_PCL
    m_collection->width += 1;
    m_collection->points.push_back(val);
#else
    m_collection.push_back(val);
#endif
}

template<typename T>
T& CT_StandardCloudStdVectorT<T>::addT()
{
#ifdef USE_PCL
    size_t l = m_collection->points.size();
    m_collection->width += 1;
    m_collection->points.resize(l+1);
    return m_collection->points[l];
#else
    size_t l = m_collection.size();
    m_collection.resize(l+1);
    return m_collection[l];
#endif
}

#endif // CT_STANDARDCLOUDSTDVECTORT_HPP
