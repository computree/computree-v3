#ifndef CT_ABSTRACTCLOUDSYNCTOGLOBALCLOUDMANAGERT_H
#define CT_ABSTRACTCLOUDSYNCTOGLOBALCLOUDMANAGERT_H

#include "ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanager.h"

template<typename T> class CT_AbstractGlobalCloudManagerT;

/**
 * Utility class who sync cloud of T (color, normal, etc....) with
 * the global cloud in the CT_AbstractGlobalCloudManagerT (per example a cloud of point or a cloud of face, etc...).
 *
 * When the cloud created is destroyed from memory it will automatically be unregistered.
 */
template<typename T>
class CT_AbstractCloudSyncToGlobalCloudManagerT : public CT_AbstractCloudSyncToGlobalCloudManager
{
public:
    CT_AbstractCloudSyncToGlobalCloudManagerT(const CT_AbstractGlobalCloudManagerT<T> &gcManager);

    CT_AbstractGlobalCloudManagerT<T>* globalCloudManager() const;

private:

    /**
     * @brief le gestionnaire du nuage global
     */
    CT_AbstractGlobalCloudManagerT<T>   *m_gcManager;
};

#include "ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanagert.hpp"

#endif // CT_ABSTRACTCLOUDSYNCTOGLOBALCLOUDMANAGERT_H
