#ifndef CT_REPOSITORY_H
#define CT_REPOSITORY_H

#include <QMutex>

#include "interfaces.h"

#include "ct_cloud/tools/ct_cloudsynctoglobalcloudmanager.h"
#include "ct_pointcloud/tools/abstract/ct_abstractundefinedsizepointcloud.h"
#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"

#include "ct_point.h"

template<typename T> class CT_AbstractGlobalCloudManagerT;
template<typename T, typename CLOUD> class CT_GlobalCloudManagerT;
template<typename T> class CT_AbstractCloudT;
template<typename T> class CT_StandardCloudStdVectorT;
template<typename T> class CT_AbstractCloudIndexT;
template<typename T> class CT_AbstractModifiableCloudIndexT;
template<typename T> class CT_CloudIndexLessMemoryT;
class CT_PointCloudIndexLessMemory;
class CT_EdgeCloudIndexLessMemory;
class CT_FaceCloudIndexLessMemory;

template<typename T> class CT_ModifiableCloudIndexIteratorT;
template<typename T> class CT_CloudIndexIteratorT;

template<typename T, class CloudIndexLessMemory> class CT_CloudIndexRegistrationManagerT;
template<typename T> class CT_AbstractCloudIndexRegisteredT;
template<typename T> class CT_AbstractNotModifiableCloudIndexRegisteredT;
template<typename T> class CT_AbstractModifiableCloudIndexRegisteredT;

class CT_GlobalPointCloudManager;
class CT_GlobalFaceCloudManager;
class CT_GlobalEdgeCloudManager;

class CT_Face;
class CT_Edge;

class CT_StandardColorCloudRegistered;
class CT_StandardNormalCloudRegistered;

class PLUGINSHAREDSHARED_EXPORT CT_Repository
{
protected:
    typedef CT_GlobalPointCloudManager                                                      GlobalPointCloudManager;
    typedef CT_GlobalFaceCloudManager                                                       GlobalFaceCloudManager;
    typedef CT_GlobalEdgeCloudManager                                                       GlobalEdgeCloudManager;

    typedef CT_CloudIndexRegistrationManagerT<CT_Point, CT_PointCloudIndexLessMemory >      PointCloudIndexRegistrationManager;
    typedef CT_CloudIndexRegistrationManagerT<CT_Face, CT_FaceCloudIndexLessMemory >        FaceCloudIndexRegistrationManager;
    typedef CT_CloudIndexRegistrationManagerT<CT_Edge, CT_EdgeCloudIndexLessMemory >        EdgeCloudIndexRegistrationManager;

    typedef CT_CloudSyncToGlobalCloudManager                                                SyncPointCloudManager;
    typedef CT_CloudSyncToGlobalCloudManager                                                SyncFaceCloudManager;
    typedef CT_CloudSyncToGlobalCloudManager                                                SyncEdgeCloudManager;

public:
    typedef CT_ModifiableCloudIndexIteratorT<CT_Point >                                     VertexIndexIterator;
    typedef CT_ModifiableCloudIndexIteratorT<CT_Face >                                      FaceIndexIterator;
    typedef CT_ModifiableCloudIndexIteratorT<CT_Edge >                                      EdgeIndexIterator;

    typedef CT_CloudIndexIteratorT<CT_Point>                                                ConstVertexIndexIterator;
    typedef CT_CloudIndexIteratorT<CT_Face>                                                 ConstFaceIndexIterator;
    typedef CT_CloudIndexIteratorT<CT_Edge>                                                 ConstEdgeIndexIterator;

    typedef QSharedPointer< CT_AbstractCloudIndexRegisteredT<CT_Point> >                    CT_AbstractPCIR;
    typedef QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> >       CT_AbstractNotModifiablePCIR;
    typedef QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Point> >          CT_AbstractModifiablePCIR;
    typedef QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Face> >           CT_AbstractModifiableFCIR;
    typedef QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Edge> >           CT_AbstractModifiableECIR;
    typedef QSharedPointer< CT_StandardColorCloudRegistered >                               CT_CCR;
    typedef QSharedPointer< CT_StandardNormalCloudRegistered >                              CT_NCR;

    /**
     * @brief Type of synchronization for new cloud index
     */
    enum SyncCloudWith {
        SyncWithPointCloud, // sync cloud index with point cloud
        SyncWithFaceCloud,  // sync cloud index with face cloud
        SyncWithEdgeCloud   // sync cloud index with edge cloud
    };

    /**
     * @brief Type of optimization in memory for new cloud index
     */
    enum CloudIndexOptimizationType
    {
        MemoryOptimized = 0,    // Add only a "begin index" and a "size" variables in the class that manage index.
        AccessSpeedOptimized    // Add one index for one points.
    };

    ~CT_Repository();

    /**
     * @brief Create a new point cloud in memory (Update the size of the global points cloud).
     *
     *        When you delete from memory the object returned by this method, the points cloud will be automatically
     *        removed from global points cloud and its size will be updated. The index manager will automatically update
     *        all cloud index that points to elements after the cloud deleted.
     *
     * @param size : the size of the point cloud to create
     * @param optim : optimization type
     * @return Returns a cloud index that points to new points created. This cloud index is not modifiable but points are.
     */
    CT_AbstractNotModifiablePCIR createNewPointCloud(const size_t &size, CloudIndexOptimizationType optim = MemoryOptimized);

    /**
     * @brief Create a new point cloud in memory that size is not known.
     * @warning You can call once again this method only after you have registered the object returned
     *          by this method : use the method "registerUndefinedSizePointCloud"
     *
     * @return Returns NULL if a previous CT_AbstractUndefinedSizePointCloud was created but not registered. Otherwise returns an object
     *         that can be used to add/remove new points to global points cloud.
     */
    CT_AbstractUndefinedSizePointCloud* createNewUndefinedSizePointCloud();

    /**
     * @brief Create a new colors cloud whose is size is synchronized with the global XXX cloud. This means that the size
     *        of the cloud index will always be sync with the global cloud and indices will be automatically modified if
     *        a cloud is deleted.
     *
     * @param syncWith : define what cloud sync
     * @param withAlphaInformation : true if you want a rgba color cloud instead of a rgb
     * @return return the colors cloud registered
     */
    CT_CCR createNewColorCloud(SyncCloudWith syncWith, bool withAlphaInformation = false);

    /**
     * @brief Create a new normals cloud whose is size is synchronized with the global XXX cloud. This means that the size
     *        of the cloud index will always be sync with the global cloud and indices will be automatically modified if
     *        a cloud is deleted.
     *
     * @param syncWith : define what cloud sync
     * @return return the normals cloud registered
     */
    CT_NCR createNewNormalCloud(SyncCloudWith syncWith);

    /**
     * @brief Create a new cloud of type CloudT that will be synchronized with the global XXX cloud (defined by syncWith param). This
     *        means that the size of the cloud index will always be sync with the global cloud and indices will be automatically modified if
     *        a cloud is deleted.
     *
     * @param syncWith : define what cloud sync
     * @example : create a cloud of quint8 that is synchorinzed with the global points cloud :
     *
     *            QSharedPointer<> sp = createNewSyncCloudT<CT_StandardCloudStdVectorT<quint8> >(CT_Repository::SyncWithPointCloud);
     *
     * @return return the CloudT cloud registered
     */
    template<class CloudRegistered, class CloudT >
    QSharedPointer<CloudRegistered> createNewSyncCloudT(SyncCloudWith syncWith)
    {
        if(syncWith == SyncWithPointCloud)
            return m_syncPointCloudManager->createNewCloud<CloudRegistered, CloudT >();
        else if(syncWith == SyncWithFaceCloud)
            return m_syncFaceCloudManager->createNewCloud<CloudRegistered, CloudT >();
        else if(syncWith == SyncWithEdgeCloud)
            return m_syncEdgeCloudManager->createNewCloud<CloudRegistered, CloudT >();

        return QSharedPointer<CloudRegistered>(NULL);
    }

    /**
     * @brief Create a new index cloud whose indices is synchronized with index of cloud passed in parameter. This means that
     *        indices will be automatically modified if a cloud is deleted.
     *
     * @param syncWith : define what cloud sync
     * @return return the index cloud registered
     */
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> createNewIndexCloud(SyncCloudWith syncWith);

    /**
     * @brief Create a new index cloud whose indices is synchronized with index of cloud passed in parameter. This means that
     *        indices will be automatically modified if a cloud is deleted. The cloud is a map : QMap<size_t, Color>, key is indices
     *        and value the color.
     *
     * @example An example of how to use the result of this method :
     *
     *        QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> mccr = createNewMapIndexCloudColor(CT_Repository::SyncWithXXXCloud);
     *        CT_AbstractModifiableIndexCloudColorMap *map = dynamic_cast<CT_AbstractModifiableIndexCloudColorMap*>(mccr->abstractModifiableCloudIndex());
     *        map->insertIndexAndColor(0, color);
     *
     * @return return the index cloud registered
     */
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> createNewMapIndexCloudColor(SyncCloudWith syncWith);

    /**
     * @brief Register the object create with the method "createNewUndefinedSizePointCloud". Convert the points added to a
     *        point cloud index.
     *
     * @warning the object passed in parameter will be deleted from memory after you call this method. You can not use it.
     * @param uspc : the object created by the method "createNewUndefinedSizePointCloud
     * @param optim : optimization type
     * @return Returns a cloud index that points to new points created. This cloud index is not modifiable but points are.
     */
    CT_AbstractNotModifiablePCIR registerUndefinedSizePointCloud(CT_AbstractUndefinedSizePointCloud *uspc, CloudIndexOptimizationType optim = MemoryOptimized);

    /**
     * @brief Recopie EN MEMOIRE les points du nuage de points défini par le nuage d'index contenu dans le nuage de points enregistré passé en paramètre.
     *
     *        Crée un nouvel espace dans le nuage de points global de la taille du nuage d'index. Copie
     *        les points défini par le nuage d'index dans ce nouvel espace. Crée un nouveau nuage d'index non modifiable et l'enregistre auprès
     *        du gestionnaire d'index.
     *
     * @param pcr : le nuage de points enregistré à copier
     * @param optim : le type d'optimisation du nuage d'index
     * @return Retourne un nuage d'index correspondant à l'emplacement en mémoire des nouveaux points. Ce nuage d'index n'est pas modifiable !
     */
    CT_AbstractNotModifiablePCIR copyPointCloud(CT_AbstractPCIR pcir, CloudIndexOptimizationType optim = MemoryOptimized);

    /**
     * @brief Recopie EN MEMOIRE les points du nuage de points défini par le nuage d'index.
     *
     *        Crée un nouvel espace dans le nuage de points global de la taille du nuage d'index. Copie
     *        les points défini par le nuage d'index dans ce nouvel espace. Copie le nuage d'index et l'enregistre auprès
     *        du gestionnaire d'index.
     *
     * @param pcr : le nuage de points enregistré à copier
     * @param optim : le type d'optimisation du nuage d'index
     * @return Retourne un nuage d'index correspondant à l'emplacement en mémoire des nouveaux points. Ce nuage d'index n'est pas modifiable !
     */
    CT_AbstractNotModifiablePCIR copyPointCloud(const CT_AbstractCloudIndexT<CT_Point> *index, CloudIndexOptimizationType optim = MemoryOptimized);

    /**
     * @brief Return a memory optimized points cloud index that begin at the first index of the first points cloud and finish at the end of the last points cloud of the list
     * @param pcir_collection : the list of points cloud that is contiguous in memory
     * @return A memory optimized points cloud index. This points cloud index is not modifiable !
     */
    CT_AbstractNotModifiablePCIR mergePointCloudContiguous(const QList< CT_AbstractPCIR > &pcir_collection);

    /**
     * @brief Enregistre le nuage d'index auprès du gestionnaire. Le gestionnaire va modifier (décaler) les index du nuage si un nuage de points
     *        est supprimé de la mémoire et que vos index deviennent invalide.
     *
     *        Si vos index correspondent à un nuage de points qui a été supprimé les index seront supprimés. Si vos index correspondent à un nuage
     *        de points qui se trouve après celui qui a été supprimé, vos index seront décalés. Si certains index correspondent à un nuage de points qui
     *        à été supprimé ils seront par la même occasion supprimé.
     *
     *        Ainsi les index sont toujours synchronisés par rapport au nuage de points global.
     *
     *       PROTEGE PAR UN MUTEX
     *
     * @param index : le nuage d'index que vous avez créé. (il n'a pas besoin d'être remplis)
     * @return Retourne un objet contenant le nuage de points (global) et le nuage d'index du nuage de points.
     */
    CT_AbstractModifiablePCIR registerPointCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Point> *index);
    CT_AbstractModifiableFCIR registerFaceCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Face> *index);
    CT_AbstractModifiableECIR registerEdgeCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Edge> *index);

    /**
     * @brief globalPointCloud
     * @return Retourne le nuage de points global (il contient tout les nuages de points créés)
     */
    CT_AbstractPointCloud* globalPointCloud() const;

    template<typename T>
    CT_AbstractCloudT<T>* globalCloud() const { return NULL; }

protected:

    friend class CT_RepositoryManager;
    friend class CT_Mesh;
    template<typename T, typename CLOUD> friend class CT_GlobalCloudManagerT;
    template<typename T> friend class CT_MeshAllocatorT;

    CT_Repository();

    template<typename T>
    CT_AbstractGlobalCloudManagerT<T>* globalCloudManager() const { return NULL; }

    template<typename T>
    CT_StandardCloudStdVectorT<T>* globalStandardCloudT() const { return NULL; }

    /**
     * @brief IF the cloud index passed in parameter was created by the global cloud manager and IF it was at
     *        the end of the global cloud => Resize the cloud index to the 'newSize' and resize the cloud
     *        to (current size + (newSize - cloudIndex->size()) ).
     *
     *        If all condition it's ok, the cloud in the QSharedPointer is resized to the 'newSize' and the global cloud is resized to
     *        (current size + (newSize - cloudIndex->size()) )
     *
     * @return If all condition it's ok the QSharePointer passed in parameter is returned, otherwise a QSharedPointer with data() == NULL.
     */
    template<typename T>
    QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> > resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> > cir, const size_t &newSize);

    /**
     * @brief Crée un nouveau nuage en mémoire. Met à jour le nuage complet.
     *
     *        Lorsque vous allez supprimé de la mémoire l'objet retourné par cette méthode le nuage
     *        sera supprimé de la mémoire. Le gestionnaire d'index synchronisera les index toujours actifs.
     *
     * @param size : la taille du nouveau nuage à créer
     * @param optim : le type d'optimisation du nuage d'index
     * @return Retourne un nuage d'index correspondant à l'emplacement en mémoire des points. Ce nuage d'index n'est pas modifiable !
     */
    template<typename T>
    QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> > createNewCloud(const size_t &size, CloudIndexOptimizationType optim = MemoryOptimized);

    /**
     * @brief Enregistre le nuage d'index auprès du gestionnaire. Le gestionnaire va modifier (décaler) les index du nuage si un nuage
     *        est supprimé de la mémoire et que vos index deviennent invalide.
     *
     *        Si vos index correspondent à un nuage qui a été supprimé les index seront supprimés. Si vos index correspondent à un nuage
     *        qui se trouve après celui qui a été supprimé, vos index seront décalés. Si certains index correspondent à un nuage qui
     *        à été supprimé ils seront par la même occasion supprimé.
     *
     *        Ainsi les index sont toujours synchronisés par rapport au nuage global.
     *
     *       PROTEGE PAR UN MUTEX
     *
     * @param index : le nuage d'index que vous avez créé. (il n'a pas besoin d'être remplis)
     * @return Retourne un objet contenant le nuage global et le nuage d'index du nuage de points.
     */
    template<typename T>
    QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<T> > registerCloudIndex(CT_AbstractModifiableCloudIndexT<T> *index);

    friend class CT_ResultGroup;
    /**
     * @brief Call this method when you begin to delete multiple cloud (to accelerate the process)
     *
     *        Called from CT_ResultGroup
     */
    void begingDeleteMultiCloud();

    /**
     * @brief Call this method when you have finished to delete multiple cloud
     *
     *        Called from CT_ResultGroup
     */
    void endDeleteMultiCloud();

private:

    GlobalPointCloudManager                 *m_gpcManager;
    GlobalFaceCloudManager                  *m_gfcManager;
    GlobalEdgeCloudManager                  *m_gecManager;

    PointCloudIndexRegistrationManager      *m_pcirManager;
    FaceCloudIndexRegistrationManager       *m_fcirManager;
    EdgeCloudIndexRegistrationManager       *m_ecirManager;

    SyncPointCloudManager                   *m_syncPointCloudManager;
    SyncFaceCloudManager                    *m_syncFaceCloudManager;
    SyncEdgeCloudManager                    *m_syncEdgeCloudManager;

    QMutex                                  m_mutexUSPC;
};

template<>
PLUGINSHAREDSHARED_EXPORT CT_AbstractCloudT<CT_Point>* CT_Repository::globalCloud() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_AbstractCloudT<CT_Face>* CT_Repository::globalCloud() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_AbstractCloudT<CT_Edge>* CT_Repository::globalCloud() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_AbstractGlobalCloudManagerT<CT_Point>* CT_Repository::globalCloudManager() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_AbstractGlobalCloudManagerT<CT_Face>* CT_Repository::globalCloudManager() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_AbstractGlobalCloudManagerT<CT_Edge>* CT_Repository::globalCloudManager() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_StandardCloudStdVectorT<CT_Point>* CT_Repository::globalStandardCloudT() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_StandardCloudStdVectorT<CT_Face>* CT_Repository::globalStandardCloudT() const;

template<>
PLUGINSHAREDSHARED_EXPORT CT_StandardCloudStdVectorT<CT_Edge>* CT_Repository::globalStandardCloudT() const;

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> > CT_Repository::createNewCloud(const size_t &size, CT_Repository::CloudIndexOptimizationType optim);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face> > CT_Repository::createNewCloud(const size_t &size, CT_Repository::CloudIndexOptimizationType optim);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge> > CT_Repository::createNewCloud(const size_t &size, CT_Repository::CloudIndexOptimizationType optim);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> > CT_Repository::resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> > cir, const size_t &newSize);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face> > CT_Repository::resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face> > cir, const size_t &newSize);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge> > CT_Repository::resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge> > cir, const size_t &newSize);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Point> > CT_Repository::registerCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Point> *index);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Face> > CT_Repository::registerCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Face> *index);

template<>
PLUGINSHAREDSHARED_EXPORT QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Edge> > CT_Repository::registerCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Edge> *index);

#endif // CT_REPOSITORY_H
