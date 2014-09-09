#include "ct_context.h"
#include "ct_global/ct_temporarylog.h"
#include "ct_attributes/tools/ct_defaultitemattributemanager.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_categories/ct_stdcategory.h"

CT_Context* CT_Context::UNIQUE_INSTANCE = NULL;

CT_Context::CT_Context()
{
    m_categoryManager = new CT_CategoryManager();

    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_VALUE));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_ID));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_DISPLAYABLE_NAME));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_COORDINATE));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_CX));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_CY));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_CZ));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_DIRECTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X_DIRECTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y_DIRECTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z_DIRECTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_DIMENSION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X_DIMENSION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y_DIMENSION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z_DIMENSION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_RADIUS));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_R2));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_SIZE));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_LENGTH));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_WIDTH));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_HEIGHT));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_ANGLE));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_RATIO));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_RESOLUTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X_RESOLUTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y_RESOLUTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z_RESOLUTION));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_NA));

    m_tmpLog = new CT_TemporaryLog();
    m_diam = new CT_DefaultItemAttributeManager();

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
