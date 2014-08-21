#include "ct_repository.h"

#include "ct_cloud/tools/ct_cloudsynctoglobalcloudmanagert.h"
#include "ct_cloudindex/tools/ct_cloudindexregistrationmanagert.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_colorcloud/ct_indexcloudcolorstdmapt.h"

#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

#include "ct_cloudindex/ct_cloudindexstdlistt.h"

#include "ct_cloud/tools/ct_globalpointcloudmanager.h"
#include "ct_cloud/tools/ct_globaledgecloudmanager.h"
#include "ct_cloud/tools/ct_globalfacecloudmanager.h"

#include "ct_pointcloudindex/ct_pointcloudindexlessmemory.h"
#include "ct_mesh/cloud/ct_edgecloudindexlessmemory.h"
#include "ct_mesh/cloud/ct_facecloudindexlessmemory.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_mesh/cloud/ct_edgecloudindexvector.h"
#include "ct_mesh/cloud/ct_facecloudindexvector.h"

#include <QMutexLocker>

CT_Repository::CT_AbstractNotModifiablePCIR CT_Repository::createNewPointCloud(const size_t &size, CloudIndexOptimizationType optim)
{
    return createNewCloud<CT_Point>(size, optim);
}

CT_AbstractUndefinedSizePointCloud* CT_Repository::createNewUndefinedSizePointCloud()
{
    QMutexLocker locker(&m_mutexUSPC);

    return m_gpcManager->createNewUndefinedSizePointCloud();
}

CT_Repository::CT_CCR CT_Repository::createNewColorCloud(SyncCloudWith syncWith, bool withAlphaInformation)
{
    if(syncWith == SyncWithPointCloud)
        return m_pColorCloudManager->createNewCloud<CT_StandardColorCloudRegistered, CT_ColorCloudStdVector, bool>(&withAlphaInformation);
    else if(syncWith == SyncWithFaceCloud)
        return m_fColorCloudManager->createNewCloud<CT_StandardColorCloudRegistered, CT_ColorCloudStdVector, bool>(&withAlphaInformation);
    else if(syncWith == SyncWithEdgeCloud)
        return m_eColorCloudManager->createNewCloud<CT_StandardColorCloudRegistered, CT_ColorCloudStdVector, bool>(&withAlphaInformation);

    return CT_Repository::CT_CCR(NULL);
}

CT_Repository::CT_NCR CT_Repository::createNewNormalCloud(SyncCloudWith syncWith)
{
    if(syncWith == SyncWithPointCloud)
        return m_pNormalCloudManager->createNewCloud<CT_StandardNormalCloudRegistered, CT_NormalCloudStdVector>();
    else if(syncWith == SyncWithFaceCloud)
        return m_fNormalCloudManager->createNewCloud<CT_StandardNormalCloudRegistered, CT_NormalCloudStdVector>();
    else if(syncWith == SyncWithEdgeCloud)
        return m_eNormalCloudManager->createNewCloud<CT_StandardNormalCloudRegistered, CT_NormalCloudStdVector>();

    return CT_Repository::CT_NCR(NULL);
}

QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> CT_Repository::createNewIndexCloud(CT_Repository::SyncCloudWith syncWith)
{
    if(syncWith == SyncWithPointCloud)
        return registerCloudIndex<CT_Point>(new CT_CloudIndexStdListT<CT_Point>());
    else if(syncWith == SyncWithFaceCloud)
        return registerCloudIndex<CT_Face>(new CT_CloudIndexStdListT<CT_Face>());
    else if(syncWith == SyncWithEdgeCloud)
        return registerCloudIndex<CT_Edge>(new CT_CloudIndexStdListT<CT_Edge>());

    return QSharedPointer<CT_AbstractModifiableCloudIndexRegistered>(NULL);
}

QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> CT_Repository::createNewMapIndexCloudColor(CT_Repository::SyncCloudWith syncWith)
{
    if(syncWith == SyncWithPointCloud)
        return registerCloudIndex<CT_Point>(new CT_IndexCloudColorStdMapT<CT_Point>());
    else if(syncWith == SyncWithFaceCloud)
        return registerCloudIndex<CT_Face>(new CT_IndexCloudColorStdMapT<CT_Face>());
    else if(syncWith == SyncWithEdgeCloud)
        return registerCloudIndex<CT_Edge>(new CT_IndexCloudColorStdMapT<CT_Edge>());

    return QSharedPointer<CT_AbstractModifiableCloudIndexRegistered>(NULL);
}

CT_Repository::CT_AbstractNotModifiablePCIR CT_Repository::registerUndefinedSizePointCloud(CT_AbstractUndefinedSizePointCloud *uspc, CT_Repository::CloudIndexOptimizationType optim)
{
    QMutexLocker locker(&m_mutexUSPC);

    return m_gpcManager->registerUndefinedSizePointCloud(uspc, (CT_GlobalPointCloudManager::IndexOptimization)optim);
}

CT_Repository::CT_AbstractNotModifiablePCIR CT_Repository::copyPointCloud(CT_Repository::CT_AbstractPCIR pcir, CloudIndexOptimizationType optim)
{
    return m_gpcManager->copyCloud<CT_PointCloudIndexLessMemory, CT_PointCloudIndexVector>(pcir, (CT_GlobalPointCloudManager::IndexOptimization)optim);
}

CT_Repository::CT_AbstractNotModifiablePCIR CT_Repository::copyPointCloud(const CT_AbstractCloudIndexT<CT_Point> *index, CT_Repository::CloudIndexOptimizationType optim)
{
    return m_gpcManager->copyCloud<CT_PointCloudIndexLessMemory, CT_PointCloudIndexVector>(index, (CT_GlobalPointCloudManager::IndexOptimization)optim);
}

CT_Repository::CT_AbstractNotModifiablePCIR CT_Repository::mergePointCloudContiguous(const QList< CT_Repository::CT_AbstractPCIR > &pcir_collection)
{
    return m_pcirManager->mergeCloudContiguous(pcir_collection);
}

CT_Repository::CT_AbstractModifiablePCIR CT_Repository::registerPointCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Point> *index)
{
    return registerCloudIndex<CT_Point>(index);
}

CT_AbstractPointCloud* CT_Repository::globalPointCloud() const
{
    return m_gpcManager->globalPointsCloud();
}

void CT_Repository::begingDeleteMultiCloud()
{
    m_gpcManager->beginMultiDelete();
    m_gecManager->beginMultiDelete();
    m_gfcManager->beginMultiDelete();
}

void CT_Repository::endDeleteMultiCloud()
{
    // first we delete faces
    m_gfcManager->endMultiDelete();
    // then edges
    m_gecManager->endMultiDelete();
    // and finally points
    m_gpcManager->endMultiDelete();
}

// PROTECTED //

CT_Repository::CT_Repository()
{
    m_gpcManager = new GlobalPointCloudManager();
    m_gecManager = new GlobalEdgeCloudManager();
    m_gecManager->setGlobalCloudManager(m_gpcManager);
    m_gfcManager = new GlobalFaceCloudManager(m_gecManager);
    m_gecManager->setGlobalCloudManager(m_gfcManager);

    m_pcirManager = new PointCloudIndexRegistrationManager(*m_gpcManager);
    m_fcirManager = new FaceCloudIndexRegistrationManager(*m_gfcManager);
    m_ecirManager = new EdgeCloudIndexRegistrationManager(*m_gecManager);

    m_pColorCloudManager = new SyncPColorCloudManager(*m_gpcManager);
    m_pNormalCloudManager = new SyncPNormalCloudManager(*m_gpcManager);

    m_fColorCloudManager = new SyncFColorCloudManager(*m_gfcManager);
    m_fNormalCloudManager = new SyncFNormalCloudManager(*m_gfcManager);

    m_eColorCloudManager = new SyncEColorCloudManager(*m_gecManager);
    m_eNormalCloudManager = new SyncENormalCloudManager(*m_gecManager);
}

CT_Repository::~CT_Repository()
{
    delete m_pcirManager;
    delete m_fcirManager;
    delete m_ecirManager;

    delete m_pColorCloudManager;
    delete m_pNormalCloudManager;

    delete m_fColorCloudManager;
    delete m_fNormalCloudManager;

    delete m_eColorCloudManager;
    delete m_eNormalCloudManager;

    delete m_gfcManager;
    delete m_gecManager;
    delete m_gpcManager;
}

// TEMPLATE //

template<>
CT_AbstractCloudT<CT_Point>* CT_Repository::globalCloud() const
{
    return m_gpcManager->globalCloud();
}

template<>
CT_AbstractCloudT<CT_Face>* CT_Repository::globalCloud() const
{
    return m_gfcManager->globalCloud();
}

template<>
CT_AbstractCloudT<CT_Edge>* CT_Repository::globalCloud() const
{
    return m_gecManager->globalCloud();
}

template<>
CT_AbstractGlobalCloudManagerT<CT_Point>* CT_Repository::globalCloudManager() const
{
    return m_gpcManager;
}

template<>
CT_AbstractGlobalCloudManagerT<CT_Face>* CT_Repository::globalCloudManager() const
{
    return m_gfcManager;
}

template<>
CT_AbstractGlobalCloudManagerT<CT_Edge>* CT_Repository::globalCloudManager() const
{
    return m_gecManager;
}

template<>
CT_StandardCloudStdVectorT<CT_Point>* CT_Repository::globalStandardCloudT() const
{
    return m_gpcManager->globalCloud();
}

template<>
CT_StandardCloudStdVectorT<CT_Face>* CT_Repository::globalStandardCloudT() const
{
    return m_gfcManager->globalCloud();
}

template<>
CT_StandardCloudStdVectorT<CT_Edge>* CT_Repository::globalStandardCloudT() const
{
    return m_gecManager->globalCloud();
}

template<>
QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> > CT_Repository::resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> > cir, const size_t &newSize)
{
    return m_gpcManager->resizeCloudAndCloudIndex(cir, newSize);
}

template<>
QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face> > CT_Repository::resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face> > cir, const size_t &newSize)
{
    return m_gfcManager->resizeCloudAndCloudIndex(cir, newSize);
}

template<>
QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge> > CT_Repository::resizeCloudIndexAndGlobalCloud(QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge> > cir, const size_t &newSize)
{
    return m_gecManager->resizeCloudAndCloudIndex(cir, newSize);
}

template<>
QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Point> > CT_Repository::createNewCloud(const size_t &size, CT_Repository::CloudIndexOptimizationType optim)
{
    return m_gpcManager->createNewCloud<CT_PointCloudIndexLessMemory, CT_PointCloudIndexVector>(size, (CT_GlobalPointCloudManager::IndexOptimization)optim);
}

template<>
QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Face> > CT_Repository::createNewCloud(const size_t &size, CT_Repository::CloudIndexOptimizationType optim)
{
    return m_gfcManager->createNewCloud< CT_FaceCloudIndexLessMemory , CT_FaceCloudIndexVector >(size, (CT_AbstractGlobalCloudManagerT<CT_Face>::IndexOptimization)optim);
}

template<>
QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<CT_Edge> > CT_Repository::createNewCloud(const size_t &size, CT_Repository::CloudIndexOptimizationType optim)
{
    return m_gecManager->createNewCloud< CT_EdgeCloudIndexLessMemory , CT_EdgeCloudIndexVector >(size, (CT_AbstractGlobalCloudManagerT<CT_Edge>::IndexOptimization)optim);
}

template<>
QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Point> > CT_Repository::registerCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Point> *index)
{
    return m_pcirManager->registerIndex(index);
}

template<>
QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Face> > CT_Repository::registerCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Face> *index)
{
    return m_fcirManager->registerIndex(index);
}

template<>
QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<CT_Edge> > CT_Repository::registerCloudIndex(CT_AbstractModifiableCloudIndexT<CT_Edge> *index)
{
    return m_ecirManager->registerIndex(index);
}
