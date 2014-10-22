#ifndef CT_STANDARDCLOUDSTDVECTORT_H
#define CT_STANDARDCLOUDSTDVECTORT_H

#include "ct_cloud/abstract/ct_abstractcloudt.h"
#include <vector>

/**
 * A cloud of T (point, face, etc..) that use a std::vector.
 */
template<typename T>
class CT_StandardCloudStdVectorT : virtual public CT_AbstractCloudT<T>
{
public:
#ifdef USE_PCL
    typedef typename std::vector<T, Eigen::aligned_allocator<T> >::iterator        Iterator;
    typedef typename std::vector<T, Eigen::aligned_allocator<T> >::const_iterator  Const_Iterator;
#else
    typedef typename std::vector<T>::iterator        Iterator;
    typedef typename std::vector<T>::const_iterator  Const_Iterator;
#endif
    CT_StandardCloudStdVectorT(size_t size = 0);

    Iterator begin();
    Const_Iterator constBegin() const;

    Iterator end();
    Const_Iterator constEnd() const;

    size_t size() const;

    T& operator[](const size_t &index);
    const T& operator[](const size_t &index) const;

    void addT(const T &val);
    T& addT();

#ifdef USE_PCL
    boost::shared_ptr< pcl::PointCloud<T> > getPCLCloud() const;
#endif

    /**
     * @brief Copy data of other source
     * @warning No verification is defined for the validity of size and destIndex
     */
    void copyDataFromTo(T* src, const size_t &destIndex, const size_t &size);

    virtual CT_AbstractCloud* copy() const;

protected:

#ifdef USE_PCL
    boost::shared_ptr< pcl::PointCloud<T> >   m_collection;
#else
    std::vector<T>   m_collection;
#endif

    template<typename A, typename CLOUD> friend class CT_GlobalCloudManagerT;

    /**
     * @brief delete all elements of the cloud between beginIndex and (beginIndex+size-1) included.
     */
    void erase(const size_t &beginIndex, const size_t &s);

    /**
     * @brief resize the collection
     */
    void resize(const size_t &newSize);

    /**
     * @brief copy data
     */
    void internalCopyData(const size_t &destIndex, const size_t &srcIndex, const size_t &size);

    inline const T& pTAt(const size_t &index) const
    {
    #ifdef USE_PCL
        return m_collection->points[index];
    #else
        return m_collection[index];
    #endif
    }

    inline T& pTAt(const size_t &index)
    {
    #ifdef USE_PCL
        return m_collection->points[index];
    #else
        return m_collection[index];
    #endif
    }
};

// specialization for vector of type bool because he use not bool but bit !!!!
template<>
class CT_StandardCloudStdVectorT<bool> : virtual public CT_AbstractCloudT<bool>
{

protected:
#ifdef USE_PCL
    boost::shared_ptr< pcl::PointCloud<qint8> >     m_collection;
#else
    std::vector<qint8>                              m_collection;
#endif
};

#include "ct_cloud/ct_standardcloudstdvectort.hpp"

#endif // CT_STANDARDCLOUDSTDVECTORT_H
