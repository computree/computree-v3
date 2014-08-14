#ifndef CT_ABSTRACTCLOUDSYNCTOGLOBALCLOUDMANAGERT_HPP
#define CT_ABSTRACTCLOUDSYNCTOGLOBALCLOUDMANAGERT_HPP

#include "ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanagert.h"
#include "ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.h"

template<typename T>
CT_AbstractCloudSyncToGlobalCloudManagerT<T>::CT_AbstractCloudSyncToGlobalCloudManagerT(const CT_AbstractGlobalCloudManagerT<T> &gcManager) : CT_AbstractCloudSyncToGlobalCloudManager()
{
    m_gcManager = (CT_AbstractGlobalCloudManagerT<T>*)&gcManager;
    m_gcManager->addGlobalCloudListener(this);
}

template<typename T>
CT_AbstractGlobalCloudManagerT<T>* CT_AbstractCloudSyncToGlobalCloudManagerT<T>::globalCloudManager() const
{
    return m_gcManager;
}

#endif // CT_ABSTRACTCLOUDSYNCTOGLOBALCLOUDMANAGERT_HPP
