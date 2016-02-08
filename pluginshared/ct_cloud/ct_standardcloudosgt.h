#ifndef CT_STANDARDCLOUDOSGT_H
#define CT_STANDARDCLOUDOSGT_H

#include "ct_cloud/abstract/ct_abstractcloudt.h"

#include <osg/Array>

/**
 * A cloud of T (point, face, etc..) that use a osg::TemplateArray
 */
template<typename T, osg::Array::Type ARRAYTYPE, int DataSize, int DataType>
class CT_StandardCloudOsgT : virtual public CT_AbstractCloudT<T>
{
public:
    typedef osg::TemplateArray<T,ARRAYTYPE,DataSize,DataType>   AType;
    typedef typename AType::iterator                            Iterator;
    typedef typename AType::const_iterator                      Const_Iterator;

    CT_StandardCloudOsgT(const size_t &initialSize = 0);

    Iterator begin();
    Const_Iterator constBegin() const;

    Iterator end();
    Const_Iterator constEnd() const;

    size_t size() const;

    size_t memoryUsed() const;

    T& operator[](const size_t &index);
    const T& operator[](const size_t &index) const;

    void addT(const T &val);
    T& addT();

    /**
     * @brief Copy data of other source
     * @warning No verification is defined for the validity of size and destIndex
     */
    void copyDataFromTo(T* src, const size_t &destIndex, const size_t &size);

    virtual CT_AbstractCloud* copy() const;

    /**
     * @brief Returns the internal osg array used
     */
    AType* osgArray();

protected:
    osg::ref_ptr< AType >    m_array;

    template<typename A, typename CLOUD> friend class CT_GlobalCloudManagerT;

    /**
     * @brief delete all elements of the cloud between beginIndex and (beginIndex+size-1) included.
     */
    void erase(const size_t &beginIndex, const size_t &sizes);

    /**
     * @brief resize the collection (Called by CT_GlobalCloudManagerT and CT_AbstractCloudSyncToGlobalCloudManager)
     */
    void resize(const size_t &newSize);

    /**
     * @brief copy data
     */
    void internalCopyData(const size_t &destIndex, const size_t &srcIndex, const size_t &size);

    inline const T& pTAt(const size_t &index) const
    {
        return m_array->at(index);
    }

    inline T& pTAt(const size_t &index)
    {
        return m_array->at(index);
    }
};

#include "ct_cloud/ct_standardcloudosgt.hpp"

#endif // CT_STANDARDCLOUDOSGT_H
