#ifndef CT_STANDARDNOTMODIFIABLECLOUDINDEXREGISTEREDT_HPP
#define CT_STANDARDNOTMODIFIABLECLOUDINDEXREGISTEREDT_HPP

#include "ct_cloudindex/registered/ct_standardnotmodifiablecloudindexregisteredt.h"

template<typename T>
CT_StandardNotModifiableCloudIndexRegisteredT<T>::CT_StandardNotModifiableCloudIndexRegisteredT(CT_AbstractCloudIndexT<T> *ci) : CT_AbstractNotModifiableCloudIndexRegisteredT<T>(ci)
{
}

#endif // CT_STANDARDNOTMODIFIABLECLOUDINDEXREGISTEREDT_HPP
