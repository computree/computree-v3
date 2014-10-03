#include "ct_context.h"
#include "ct_global/ct_temporarylog.h"
#include "ct_attributes/tools/ct_defaultitemattributemanager.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_categories/ct_stdcategory.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_Context* CT_Context::UNIQUE_INSTANCE = NULL;

CT_Context::CT_Context()
{
    m_categoryManager = new CT_CategoryManager();

    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataId(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataDisplayableName(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataCoordinate(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataX(), CT_AbstractCategory::staticInitDataCoordinate()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataY(), CT_AbstractCategory::staticInitDataCoordinate()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataZ(), CT_AbstractCategory::staticInitDataCoordinate()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataCx(), CT_AbstractCategory::staticInitDataX()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataCy(), CT_AbstractCategory::staticInitDataY()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataCz(), CT_AbstractCategory::staticInitDataZ()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataDirection(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataXDirection(), QList<QString>() << CT_AbstractCategory::staticInitDataX() << CT_AbstractCategory::staticInitDataDirection()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataYDirection(), QList<QString>() << CT_AbstractCategory::staticInitDataY() << CT_AbstractCategory::staticInitDataDirection()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataZDirection(), QList<QString>() << CT_AbstractCategory::staticInitDataZ() << CT_AbstractCategory::staticInitDataDirection()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataDimension(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataXDimension(), QList<QString>() << CT_AbstractCategory::staticInitDataX() << CT_AbstractCategory::staticInitDataDimension()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataYDimension(), QList<QString>() << CT_AbstractCategory::staticInitDataY() << CT_AbstractCategory::staticInitDataDimension()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataZDimension(), QList<QString>() << CT_AbstractCategory::staticInitDataZ() << CT_AbstractCategory::staticInitDataDimension()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataRadius(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataR2(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataSize(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataNumber(), CT_AbstractCategory::staticInitDataSize()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataLength(), CT_AbstractCategory::staticInitDataSize()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataWidth(), CT_AbstractCategory::staticInitDataLength()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataHeight(), CT_AbstractCategory::staticInitDataLength()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataArea(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataAngle(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataRatio(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataResolution(), CT_AbstractCategory::staticInitDataValue()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataXResolution(), QList<QString>() << CT_AbstractCategory::staticInitDataX() << CT_AbstractCategory::staticInitDataResolution()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataYResolution(), QList<QString>() << CT_AbstractCategory::staticInitDataY() << CT_AbstractCategory::staticInitDataResolution()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataZResolution(), QList<QString>() << CT_AbstractCategory::staticInitDataZ() << CT_AbstractCategory::staticInitDataResolution()));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::staticInitDataNa(), CT_AbstractCategory::staticInitDataValue()));

    m_tmpLog = new CT_TemporaryLog();
    m_diam = new CT_DefaultItemAttributeManager();
    m_modelHelper = new CT_ModelSearchHelper();

    m_guiManager = NULL;
    m_coreInterface = NULL;

    // initialisation du gestionnaire de dépot
    m_repositoryManager = new CT_RepositoryManager();
}

CT_Context::~CT_Context()
{
    delete m_repositoryManager;
    delete m_diam;
    delete m_categoryManager;
    delete m_tmpLog;
}

CT_RepositoryManager* CT_Context::repositoryManager() const
{
    return m_repositoryManager;
}

GuiManagerInterface* CT_Context::guiManager() const
{
    return m_guiManager;
}

DocumentManagerInterface* CT_Context::documentManager() const
{
    return (m_guiManager == NULL) ? NULL : m_guiManager->documentManager();
}

CT_Repository* CT_Context::repository() const
{
    return m_repositoryManager->repository();
}

LogInterface* CT_Context::log() const
{
    if(m_coreInterface == NULL)
        return m_tmpLog;

    return m_coreInterface->log();
}

PluginManagerInterface* CT_Context::pluginManager() const
{
    if(m_coreInterface == NULL)
        return NULL;

    return m_coreInterface->pluginManager();
}

CT_CategoryManager* CT_Context::categoryManager() const
{
    return m_categoryManager;
}

CT_DefaultItemAttributeManager* CT_Context::defaultItemAttributesManager() const
{
    return m_diam;
}

CT_ModelSearchHelper* CT_Context::modelSearchHelper() const
{
    return m_modelHelper;
}

void CT_Context::setCoreInterface(const CoreInterface *core)
{
    if(m_coreInterface == NULL)
    {
        m_coreInterface = (CoreInterface*)core;

        QListIterator<CT_TemporaryLogMessage> it(m_tmpLog->messages());

        while(it.hasNext())
        {
            const CT_TemporaryLogMessage &m = it.next();

            m_coreInterface->log()->addMessage(m.m_severity, m.m_type, m.m_message, m.m_filter);
        }
    }
}

void CT_Context::setGuiManager(const GuiManagerInterface *gManager)
{
    m_guiManager = (GuiManagerInterface*)gManager;
}
