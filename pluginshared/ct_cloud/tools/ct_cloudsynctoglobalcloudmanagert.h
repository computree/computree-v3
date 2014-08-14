#ifndef CT_CLOUDSYNCTOGLOBALCLOUDMANAGERT_H
#define CT_CLOUDSYNCTOGLOBALCLOUDMANAGERT_H

#include "ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanagert.h"

/**
 * Utility class who can create a cloud of T (color, normal, etc....) that will be sync with
 * the global cloud in the CT_AbstractGlobalCloudManagerT (per example a cloud of point or a cloud of face, etc...).
 *
 * When the cloud created is destroyed from memory it will automatically be unregistered.
 */
template<typename T, class CloudRegisteredInterface>
class CT_CloudSyncToGlobalCloudManagerT : public CT_AbstractCloudSyncToGlobalCloudManagerT<T>
{
public:
    CT_CloudSyncToGlobalCloudManagerT(const CT_AbstractGlobalCloudManagerT<T> &gcManager);

    /**
     * @brief Create a new cloud whose is size while be synchronized with the global T cloud's size
     *
     * @param optionnalParameterForConstructor : an optionnal parameter passed to constructor
     *
     * This method is implemented like this :
     *
     * return QSharedPointer<CloudRegisteredInterface>( new InheritedCloudRegistered( new Cloud( globalCloud->size() , (CastOptionnalParameter*)optionnalParameterForConstructor ) ) );
     *
     * @return return the cloud registered
     */
    template< class InheritedCloudRegistered, class Cloud, typename CastOptionnalParameter >
    QSharedPointer<CloudRegisteredInterface> createNewCloud(void *optionnalParameterForConstructor);

    /**
     * @brief Create a new cloud whose is size while be synchronized with the global T cloud's size
     *
     * This method is implemented like this :
     *
     * return QSharedPointer<CloudRegisteredInterface>( new InheritedCloudRegistered( new Cloud( globalCloud->size() ) ) );
     *
     * @return return the cloud registered
     */
    template< class InheritedCloudRegistered, class Cloud >
    QSharedPointer<CloudRegisteredInterface> createNewCloud();
};

#include "ct_cloud/tools/ct_cloudsynctoglobalcloudmanagert.hpp"

#endif // CT_CLOUDSYNCTOGLOBALCLOUDMANAGERT_H
