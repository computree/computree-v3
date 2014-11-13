#include "dm_colorvbomanager.h"

#include <QtOpenGL>

#include "dm_guimanager.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

DM_ColorVBOManager::DM_ColorVBOManager()
{
    m_initialized = false;

    m_colorCloudEnable = false;
    m_useColorArray = false;

    m_refresh = false;
    m_lastSize = 0;

    m_vbo = 0;
    m_useVbo = false;
}

DM_ColorVBOManager::~DM_ColorVBOManager()
{
    if(m_initialized && (glIsBuffer(m_vbo) == GL_TRUE))
        glDeleteBuffers(1, &m_vbo);
}

void DM_ColorVBOManager::setCurrentColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    m_cc = cc;
}

void DM_ColorVBOManager::setUseColorCloud(bool use)
{
    if(use && !m_colorCloudEnable && m_useColorArray)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        m_colorCloudEnable = true;
    }
    else if(!use && m_colorCloudEnable)
    {
        glDisableClientState(GL_COLOR_ARRAY);
        m_colorCloudEnable = false;
    }
}

bool DM_ColorVBOManager::useColorCloud() const
{
    return m_colorCloudEnable;
}

bool DM_ColorVBOManager::useVBOForColorCloud() const
{
    return m_useVbo;
}

void DM_ColorVBOManager::refresh()
{
    m_refresh = true;
}

void DM_ColorVBOManager::initializeGL()
{
    GUI_MANAGER->vertexVBOManager()->initializeGL();

    if(!m_initialized) {
        QT_GL_INIT_FUNCTIONS();

        // create a vbo
        glGenBuffers(1, &m_vbo);

        m_initialized = true;
    }
}

void DM_ColorVBOManager::preDraw()
{
    GUI_MANAGER->vertexVBOManager()->preDraw();

    size_t size = 0;
    CT_AbstractColorCloud *cc = NULL;

    if(!m_cc.isNull()
            && (m_cc->abstractColorCloud() != NULL)) {

        cc = m_cc->abstractColorCloud();
        size = cc->size();
    }

    if((size != m_lastSize) || m_refresh) {
        m_refresh = false;
        m_useVbo = false;

        /*size_t nBytes = size*sizeof(CT_Color);

        // select the vbo
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // allocate nBytes in this vbo
        if(size != m_lastSize)
            glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);

        // if allocation is ok and size > 0
        if((glGetError() == GL_NO_ERROR) && (size > 0)) {

            // copy the colors cloud in this vbo
            glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, &cc->constColorAt(0)(0));

            // set use vbo to true
            if(glGetError() == GL_NO_ERROR)
                m_useVbo = true;
        }

        // deselect the vbo
        glBindBuffer(GL_ARRAY_BUFFER, 0);*/

        m_lastSize = size;

        //GUI_LOG->addDebugMessage(LogInterface::gui, QString("Use color vbo : %1").arg(m_useVbo ? "true" : "false"));
    }

    m_useColorArray = false;
    setUseColorCloud(false);

    // if size > 0
    if(m_lastSize > 0) {

        m_useColorArray = true;

        // if vbo is ok
        if(m_useVbo)
        {
            // select the vbo
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            // define the address of the colors cloud in VRAM (RAM of graphics card)
            glColorPointer(GL_BGRA, GL_UNSIGNED_BYTE, 0, 0);

            // unselect the vbo
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else // define the address of the colors cloud in RAM
            glColorPointer(GL_BGRA, GL_UNSIGNED_BYTE, 0, &m_cc->abstractColorCloud()->colorAt(0)(0));

        // enable color array
        setUseColorCloud(true);
    }
}

void DM_ColorVBOManager::postDraw()
{
    GUI_MANAGER->vertexVBOManager()->postDraw();

    setUseColorCloud(false);
}
