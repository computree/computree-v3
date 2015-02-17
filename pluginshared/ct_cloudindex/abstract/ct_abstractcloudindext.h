#ifndef CT_ABSTRACTCLOUDINDEXT_H
#define CT_ABSTRACTCLOUDINDEXT_H

#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"
#include "ct_cloudindex/tools/ct_cloudindexiteratort.h"

/**
 * A cloud of index (size_t) for T (point, face, etc...)
 */
template<typename T>
class CT_AbstractCloudIndexT : virtual public CT_AbstractCloudIndex
{
protected:

    template<typename U> friend class CT_AbstractGlobalCloudManagerT;

    virtual void internalShiftAll(const size_t &offset, const bool &negativeOffset) = 0;
    virtual void internalClear() = 0;

private:

    template<typename A> friend class CT_CloudIndexStdListT;
    template<typename A> friend class CT_CloudIndexStdVectorT;
    template<typename A> friend class CT_CloudIndexLessMemoryT;
    template<typename A> friend class CT_AbstractModifiableCloudIndexT;
    template<typename A, typename B> friend class CT_CloudIndexStdMapT;
    template<typename A> friend class CT_IndexCloudColorStdMapT;

    // private and :
    // friend class CT_CloudIndexStdVectorT
    // friend class CT_CloudIndexLessMemoryT
    // friend class CT_AbstractModifiableCloudIndexT
    //
    // why ? show CT_CloudIndexLessMemoryT constructor in ct_cloudindexlessmemoryt.h
    CT_AbstractCloudIndexT() : CT_AbstractCloudIndex() {}
};

#endif // CT_ABSTRACTCLOUDINDEXT_H
