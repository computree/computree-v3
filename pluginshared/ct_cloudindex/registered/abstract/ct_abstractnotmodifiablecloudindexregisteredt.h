#ifndef CT_ABSTRACTNOTMODIFIABLECLOUDINDEXREGISTEREDT_H
#define CT_ABSTRACTNOTMODIFIABLECLOUDINDEXREGISTEREDT_H

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

/**
 * A NOT MODIFIABLE cloud of index (size_t) for T (point, face, etc...) registered
 */
template<typename T>
class CT_AbstractNotModifiableCloudIndexRegisteredT : public CT_AbstractCloudIndexRegisteredT<T>
{
public:
    typedef typename CT_AbstractCloudIndexT<T>::ConstIterator  ConstIterator;

    ~CT_AbstractNotModifiableCloudIndexRegisteredT();

    ConstIterator constBegin() const { return this->abstractCloudIndexT()->constBegin(); }
    ConstIterator constEnd() const { return this->abstractCloudIndexT()->constEnd(); }

protected:
    CT_AbstractNotModifiableCloudIndexRegisteredT(CT_AbstractCloudIndexT<T> *ci) :  CT_AbstractCloudIndexRegisteredT<T>(ci) {}
};

#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.hpp"

#endif // CT_ABSTRACTNOTMODIFIABLECLOUDINDEXREGISTEREDT_H
