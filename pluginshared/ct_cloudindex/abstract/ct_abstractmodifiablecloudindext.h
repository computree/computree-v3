#ifndef CT_ABSTRACTMODIFIABLECLOUDINDEXT_H
#define CT_ABSTRACTMODIFIABLECLOUDINDEXT_H

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"
#include "ct_cloudindex/tools/ct_modifiablecloudindexiteratort.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

/**
 * A MODIFIABLE cloud of index (size_t) for T (point, face, etc...)
 */
template<typename T>
class CT_AbstractModifiableCloudIndexT : virtual public CT_AbstractCloudIndexT<T>, public CT_AbstractModifiableCloudIndex
{
public:

    typedef bool (*FindIfFunction) (void *context, const size_t &value);
    typedef bool (*RemoveIfFunction) (void *context, const size_t &value);
    typedef bool (*ShiftIfFunction) (void *context, const size_t &value);

    /**
     * @brief add a new index in the cloud
     *
     * -> push_back(newIndex) if sortedType() == NotSorted.
     * -> insert new index at the right place if sortedType() != NotSorted
     *
     * @param newIndex : index to add
     */
    virtual void addIndex(const size_t &newIndex) = 0;

    /**
     * @brief Replace the index at the location 'i' in the cloud with the 'newIndex'
     *
     * @warning CAUTION ! If you use this method and sortType() != NotSorted, the sortType will be changed to NotSorted if the index
     * passed in parameter don't respect the sortType. If the class don't accept to be NotSorted the sortType will be corrupted !
     *
     * @param i : location of the index
     * @param newIndex : the newIndex
     * @param verifyRespectSort : set to false if you want to accelerate the process and you are VERY sure to respect the sort
     */
    virtual void replaceIndex(const size_t &i, const ct_index_type &newIndex, const bool &verifyRespectSort = true) = 0;

    /**
     * @brief add a new index a the beginning of the cloud index
     *
     * @warning CAUTION ! If you use this method and sortType() != NotSorted, the sortType will be changed to NotSorted if the index
     * passed in parameter don't respect the sortType. If the class don't accept to be NotSorted the sortType will be corrupted !
     *
     * Prefer addIndex method.
     *
     * @param newIndex : l'index à rajouter
     */
    virtual void push_front(const size_t &newIndex) = 0;

    /**
     * @brief fill the cloud index with '0'
     */
    virtual void fill() = 0;

    /**
     * @brief removes all indexes in the cloud index
     */
    virtual void clear() = 0;

    /**
     * @brief removes all indexes between beginIndex and (beginIndex+sizes-1) included
     */
    virtual void erase(const size_t &beginIndex, const size_t &sizes) = 0;

    /**
     * @brief resize
     */
    virtual void resize(const size_t &newSize) = 0;

    /**
     * @brief Search, first, the first element with the function FindIfFunction. Second, loop at the rest of the cloud and for each value
     *        call the function RemoveIfFunction. If this function return false we call the function ShiftIfFunction otherwise we call nothing
     *        and continue to loop.
     *
     *        the function FindIfFunction must return true when we must begin the loop on the indexes of the cloud index.
     *        the function RemoveIfFunction must return true when we must delete the index of the cloud index.
     *        the function ShiftIfFunction must return true when we must add the "shiftValue" to the current index.
     *
     *        So an element is either deleted or its value is added to the value "shiftValue".
     *
     * @param findIf : FindIfFunction function
     * @param removeIf : RemoveIfFunction function
     * @param shiftIf : ShiftIfFunction function
     * @param shiftValue : value to add if ShiftIfFunction return true
     * @param context : a context for your function or null
     */
    virtual void removeIfOrShiftIf(FindIfFunction findIf,
                                   RemoveIfFunction removeIf,
                                   ShiftIfFunction shiftIf,
                                   const size_t &shiftValue,
                                   const bool &negativeShift,
                                   void *context) = 0;

    /**
     * @brief Add at indexes in the cloud index the value passed in parameter
     * @param offset : value to add
     */
    virtual void shiftAll(const size_t &offset, const bool &negativeOffset) = 0;


    /**
     * @brief Method to erase a part of the cloud and shift the rest of the cloud with an 'offset'
     *
     * If the cloud is a vector or a list or a map how to do this process is optimized
     */
    virtual void eraseBetweenAndShiftRest(const size_t &eraseBeginPos, const size_t &eraseSize,
                                          const size_t &offset, const bool &negativeOffset) = 0;

private:
    template<typename A> friend class CT_CloudIndexStdVectorT;
    template<typename A> friend class CT_CloudIndexStdListT;
    template<typename A, typename B> friend class CT_CloudIndexStdMapT;


    // private and :
    // friend class CT_CloudIndexStdVectorT
    //
    // why ? show CT_CloudIndexLessMemoryT constructor in ct_cloudindexstdvectort.h
    CT_AbstractModifiableCloudIndexT() : CT_AbstractCloudIndexT<T>() {}
};

#endif // CT_ABSTRACTMODIFIABLECLOUDINDEXT_H
