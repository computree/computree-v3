#ifndef CT_CLOUDSYNCTOGLOBALCLOUDMANAGERT_HPP
#define CT_CLOUDSYNCTOGLOBALCLOUDMANAGERT_HPP

#include "ct_cloud/tools/ct_cloudsynctoglobalcloudmanagert.h"
#include "ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.h"

template<typename T, class CRI>
CT_CloudSyncToGlobalCloudManagerT<T, CRI>::CT_CloudSyncToGlobalCloudManagerT(const CT_AbstractGlobalCloudManagerT<T> &gcManager) : CT_AbstractCloudSyncToGlobalCloudManagerT<T>(gcManager)
{
}

template<typename T, class CRI>
template<class ICR, class C , typename CASTO>
QSharedPointer<CRI> CT_CloudSyncToGlobalCloudManagerT<T, CRI>::createNewCloud(void *optionnalParameterForConstructor)
{
    QMutexLocker locker(&this->m_mutex);

    ICR *icr = new ICR( new C(this->globalCloudManager()->globalAbstractCloud()->size(), (CASTO*)optionnalParameterForConstructor) );
    this->internalRegisterCloud(icr);

    return QSharedPointer<CRI>(icr);
}

template<typename T, class CRI>
template<class ICR, class C>
QSharedPointer<CRI> CT_CloudSyncToGlobalCloudManagerT<T, CRI>::createNewCloud()
{
    QMutexLocker locker(&this->m_mutex);

    ICR *icr = new ICR( new C(this->globalCloudManager()->globalAbstractCloud()->size()) );
    this->internalRegisterCloud(icr);

    return QSharedPointer<CRI>(icr);
}

#endif // CT_CLOUDSYNCTOGLOBALCLOUDMANAGERT_HPP
