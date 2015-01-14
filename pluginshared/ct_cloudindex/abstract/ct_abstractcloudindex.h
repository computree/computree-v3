#ifndef CT_ABSTRACTCLOUDINDEX_H
#define CT_ABSTRACTCLOUDINDEX_H

#include "ct_cloud/abstract/ct_abstractcloud.h"

#ifdef USE_PCL
#include "pcl/point_cloud.h"
#endif

/**
 * A cloud of index (size_t)
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractCloudIndex : public CT_AbstractCloud
{
public:

#ifdef USE_PCL
    typedef int size_type;
#else
    typedef size_t size_type;
#endif

    enum SortType
    {
        SortedInAscendingOrder,
        NotSorted
    };

    CT_AbstractCloudIndex();
    virtual ~CT_AbstractCloudIndex() {}

    /**
     * @brief return the sort type
     */
    SortType sortType() const;

    /**
      * \brief return the index in the cloud at index 'i'.
      *
      *        You can probably have better performance (speed) if you use
      *        the method indexAt(const size_t &i, size_t &index)
      */
    virtual size_t indexAt(const size_t &i) const = 0;

    /**
      * \brief return the index in the cloud at index 'i'.
      *
      *        You can probably have better performance (speed) if you use
      *        the method indexAt(const size_t &i, size_t &index)
      */
    virtual size_t operator[](const size_t &i) const = 0;

    /**
      * \brief return the index in the cloud at index 'i'.
      *
      * \param i :the location of the index you want
      * \param index : the index (out parameter)
      */
    virtual void indexAt(const size_t &i, size_t &index) const = 0;

    /**
     * @brief Returns a const reference of the index at the location 'i'
     */
    virtual const CT_AbstractCloudIndex::size_type& constIndexAt(const size_t &i) const = 0;

    /**
     * @brief return the first index of the cloud index
     * @warning undefined if the cloud is empty !
     */
    virtual size_t first() const = 0;

    /**
     * @brief return the last index of the cloud index
     * @warning undefined if the cloud is empty !
     */
    virtual size_t last() const = 0;

    /**
     * @brief return true if the cloud index contains the index passed in parameter
     * @param index : index to search
     * @return true if the cloud contains at least one time the index passed in parameter
     */
    virtual bool contains(const size_t &index) const = 0;

    /**
     * @brief Return the index (local) of the index (global) passed in parameter
     */
    virtual size_t indexOf(const size_t &index) const = 0;

    /**
     * @brief return the location of the first element that is greater or egual the value passed as a parameter
     * @param value : value to search
     */
    virtual size_t lowerBound(const size_t &value) const = 0;

    /**
     * @brief return the location of the first member is greater than the value passed as parameter
     * @param value : value to search
     */
    virtual size_t upperBound(const size_t &value) const = 0;

#ifdef USE_PCL
    /**
     * @brief return the PCL cloud index. Changing it does not necessarily affect the cloud index (depends on whether the cloud is modifiable or not)
     */
    virtual boost::shared_ptr< std::vector<int> > getPCLIndices() const = 0;
#endif

    /**
     * @brief Returns true if it must be unregistered when it was empty (true by default)
     */
    bool mustBeUnregisteredWhenIsEmpty() const;

private:
    SortType        m_sortType;
    bool            m_unregisterWhenEmpty;

protected:

    void internalSetSortType(SortType st);

    void internalSetUnregisteredWhenIsEmpty(bool enable);
};

#endif // CT_ABSTRACTCLOUDINDEX_H
