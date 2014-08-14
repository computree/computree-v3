#include "ct_context.h"
#include "ct_global/ct_temporarylog.h"
#include "ct_attributes/tools/ct_defaultitemattributemanager.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_categories/ct_stdcategory.h"

CT_Context::CT_Context()
{
    m_categoryManager = new CT_CategoryManager();

    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_ID, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_DISPLAYABLE_NAME, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_COORDINATE, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_CX, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_CY, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_CZ, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_DIRECTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X_DIRECTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y_DIRECTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z_DIRECTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_DIMENSION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X_DIMENSION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y_DIMENSION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z_DIMENSION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_RADIUS, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_R2, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_SIZE, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_LENGTH, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_WIDTH, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_HEIGHT, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_ANGLE, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_RATIO, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_RESOLUTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_X_RESOLUTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Y_RESOLUTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_Z_RESOLUTION, CT_AbstractCategory::UNKNOWN));
    m_categoryManager->registerCategory(new CT_StdCategory(CT_AbstractCategory::DATA_NA, CT_AbstractCategory::UNKNOWN));

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
