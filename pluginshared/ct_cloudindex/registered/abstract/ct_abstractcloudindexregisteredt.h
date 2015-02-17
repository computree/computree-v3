#ifndef CT_ABSTRACTCLOUDINDEXREGISTEREDT_H
#define CT_ABSTRACTCLOUDINDEXREGISTEREDT_H

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregistered.h"
#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

template<typename T> class CT_AbstractCloudIndexRegistrationManagerT;

/**
 * A cloud of index (size_t) for T (point, face, etc...) registered
 */
template<typename T>
class CT_AbstractCloudIndexRegisteredT : virtual public CT_AbstractCloudIndexRegistered
{
public:
    ~CT_AbstractCloudIndexRegisteredT();

    CT_AbstractCloudIndexT<T>* operator->() const;
    CT_AbstractCloudIndexT<T>* abstractCloudIndexT() const;
    CT_AbstractCloudIndex* abstractCloudIndex() const;

    /**
     * @brief return the sort type
     */
    inline typename CT_AbstractCloudIndexT<T>::SortType sortType() const { return m_ci->sortType(); }

    /**
     * @brief return the size of the cloud index
     */
    inline size_t size() const { return m_ci->size(); }

    /**
      * \brief return the index in the cloud at index 'i'.
      *
      *        You can probably have better performance (speed) if you use
      *        the method indexAt(const size_t &i, size_t &index)
      */
    inline size_t indexAt(const size_t &i) const { return m_ci->indexAt(i); }

    /**
      * \brief return the index in the cloud at index 'i'.
      *
      * \param i :the location of the index you want
      * \param index : the index (out parameter)
      */
    inline void indexAt(const size_t &i, size_t &index) const { m_ci->indexAt(i, index); }

    /**
     * @brief return the first index of the cloud index
     * @warning undefined if the cloud is empty !
     */
    inline size_t first() const { return m_ci->first(); }

    /**
     * @brief return the last index of the cloud index
     * @warning undefined if the cloud is empty !
     */
    inline size_t last() const { return m_ci->last(); }

    /**
     * @brief return true if the cloud index contains the index passed in parameter
     * @param index : index to search
     * @return true if the cloud contains at least one time the index passed in parameter
     */
    inline bool contains(const size_t &index) const { return m_ci->contains(index); }

    /**
     * @brief return the location of the first element that is greater or egual the value passed as a parameter
     * @param value : value to search
     */
    inline size_t lowerBound(const size_t &value) const { return m_ci->lowerBound(value); }

    /**
     * @brief return the location of the first member is greater than the value passed as parameter
     * @param value : value to search
     */
    inline size_t upperBound(const size_t &value) const { return m_ci->upperBound(value); }

#ifdef USE_PCL
    /**
     * @brief return the PCL cloud index. Changing it does not necessarily affect the cloud index (depends on whether the cloud is modifiable or not)
     */
    inline boost::shared_ptr< std::vector<int> > getPCLIndices() const { return m_ci->getPCLIndices(); }
#endif

    /**
     * @return value at index passed in parameter.
     *
     *         For example, if the cloud index starts at the index 500 in the global cloud, call
     *         this method this way : tAt(0) will return the first value (T) in the cloud index which is
     *         the location 500 in the global cloud.
     *
     *         Use the method in in this manner:
     *
     *         CT_AbstractCloudIndexT<T> *index = ...;
     *         size_t size = index->size();
     *
     *         for(size_t i=0; i<size; ++i)
     *              T &myT = index->tAt(index);
     */
    inline T& tAt(const size_t &index) const { return m_ci->tAt(index); }

    /**
     * @brief same method as previously but you can get in the same time the index in the global cloud
     * @param index : index in the cloud index
     * @param globalIndex : index in the global cloud (out parameter)
     * @return a reference to the value
     */
    inline T& tAt(const size_t &index, size_t &globalIndex) const { return m_ci->tAt(index, globalIndex); }

    /**
     * @brief return a reference to the value but you pass the index of the global cloud
     *
     * The specified index corresponds to the index in the global cloud, not in this cloud index
     */
    inline T& tAtGlobalIndex(const size_t &index) const { return m_ci->tAtGlobalIndex(index); }

    /**
     * @brief same method as previously but return a constant reference
     */
    inline const T& constTAt(const size_t &index) const { return m_ci->constTAt(index); }

    /**
     * @brief same method as previously but you can get in the same time the index in the global cloud
     * @param index : index in the cloud index
     * @param globalIndex : index in the global cloud (out parameter)
     * @return a reference to the value
     */
    inline const T& constTAt(const size_t &index, size_t &globalIndex) const { return m_ci->constTAt(index, globalIndex); }

    /**
     * @brief return a constante reference to the value but you pass the index of the global cloud
     *
     * The specified index corresponds to the index in the global cloud, not in this cloud index
     */
    inline const T& constTAtGlobalIndex(const size_t &index) const { return m_ci->constTAtGlobalIndex(index); }

protected:
    CT_AbstractCloudIndexRegisteredT(CT_AbstractCloudIndexT<T> *ci);

    void setCloudIndexRegistrationManager(const CT_AbstractCloudIndexRegistrationManagerT<T> *cirm);
    CT_AbstractCloudIndexRegistrationManagerT<T>* cloudIndexRegistrationManager() const;

private:
    CT_AbstractCloudIndexT<T>                       *m_ci;
    CT_AbstractCloudIndexRegistrationManagerT<T>    *m_cirm;
};

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.hpp"

#endif // CT_ABSTRACTCLOUDINDEXREGISTEREDT_H
