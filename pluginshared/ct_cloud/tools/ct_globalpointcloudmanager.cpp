#include "ct_globalpointcloudmanager.h"

#include "ct_pointcloudindex/ct_pointcloudindexlessmemory.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

CT_GlobalPointCloudManager::CT_GlobalPointCloudManager() : QObject(), CT_GlobalCloudManagerT<CT_Point, CT_PointCloudStdVector>()
{
    m_uspc.first = NULL;
    m_pCloud = dynamic_cast<CT_PointCloudStdVector*>(globalCloud());
}

CT_GlobalPointCloudManager::~CT_GlobalPointCloudManager()
{
    delete m_uspc.first;
}

CT_AbstractUndefinedSizePointCloud* CT_GlobalPointCloudManager::createNewUndefinedSizePointCloud()
{
    if(m_uspc.first == NULL)
    {
        m_uspc.first = new CT_StandardUndefinedSizePointCloud(m_cloud.size(), &m_cloud);
        m_uspc.second = m_uspc.first->beginIndex();

        connect(m_uspc.first, SIGNAL(destroyed(QObject*)), this, SLOT(undefinedSizePointCloudDestroyed(QObject*)), Qt::DirectConnection);

        return m_uspc.first;
    }

    return NULL;
}

CT_GlobalCloudManagerT<CT_Point, CT_PointCloudStdVector>::CT_AbstractNotModifiableCIR CT_GlobalPointCloudManager::registerUndefinedSizePointCloud(CT_AbstractUndefinedSizePointCloud *uspc, IndexOptimization optim)
{
    CT_GlobalCloudManagerT<CT_Point, CT_PointCloudStdVector>::CT_AbstractNotModifiableCIR cir;

    if(uspc == m_uspc.first)
    {
        disconnect(m_uspc.first, NULL, this, NULL);

        size_t size = m_uspc.first->size();

        if(size > 0)
            cir = createCIR<CT_PointCloudIndexLessMemory, CT_PointCloudIndexVector>(m_uspc.first->beginIndex(), size, optim);

        delete m_uspc.first;
        m_uspc.first = NULL;
    }

    return cir;
}

CT_PointCloudStdVector* CT_GlobalPointCloudManager::globalPointsCloud() const
{
    return m_pCloud;
}

void CT_GlobalPointCloudManager::undefinedSizePointCloudDestroyed(QObject *o)
{
    Q_UNUSED(o)

    if(m_uspc.second < m_cloud.size())
    {
        resizeCloud(m_uspc.second);
    }
}
