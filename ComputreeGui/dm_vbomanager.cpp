#include "dm_vbomanager.h"

#include <QtOpenGL>

#include "ct_cloud/abstract/ct_abstractcloudt.h"
#include "ct_global/ct_context.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

DM_VBOManager::DM_VBOManager()
{
    m_colorCloudEnable = false;
    m_vbo = 0;
    m_initialized = false;
    m_lastSize = 0;
    m_useVbo = false;
}

DM_VBOManager::~DM_VBOManager()
{
    #ifdef USE_VBO_TEST
    if(m_initialized)
        glDeleteBuffers(1, &m_vbo);
    #endif
}

void DM_VBOManager::initializeGL()
{
    initializeOpenGLFunctions();
}

void DM_VBOManager::setCurrentCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    m_cc = cc;
}

void DM_VBOManager::setUseCloudColor(bool use)
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

void DM_VBOManager::preDraw()
{
    if(!m_initialized)
    {
        #ifdef USE_VBO_TEST
        glGenBuffers(1, &m_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 1, NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        #endif

        m_initialized = true;
    }

    CT_AbstractCloudT<CT_Point> *points = PS_REPOSITORY->globalCloud<CT_Point>();

    if(points->size() != m_lastSize)
    {
        m_lastSize = points->size();

        #ifdef USE_VBO_TEST
        size_t nBytes = m_lastSize*(sizeof(CT_Point)/sizeof(float));

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STREAM_DRAW);

        if((glGetError() == GL_NO_ERROR) && (m_lastSize > 0)) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, points->constTAt(0).vertex());
            m_useVbo = true;
        }
        else
            m_useVbo = false;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        #endif
    }

    m_useVertexArray = false;
    m_useColorArray = false;

    if(m_lastSize > 0) {
        m_useVertexArray = true;

        glEnableClientState(GL_VERTEX_ARRAY);

        #ifdef USE_VBO_TEST
        if(m_useVbo)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glVertexPointer(sizeof(CT_Point)/sizeof(float), GL_FLOAT, 0, 0);
        }
        else
            glVertexPointer(sizeof(CT_Point)/sizeof(float), GL_FLOAT, 0, points->constTAt(0).vertex());
        #else
        glVertexPointer(sizeof(CT_Point)/sizeof(float), GL_FLOAT, 0, points->constTAt(0).vertex());
        #endif
    }

    #ifndef USE_VBO_TEST
    if(!m_cc.isNull()
            && (m_cc->abstractColorCloud() != NULL)
            && (m_cc->abstractColorCloud()->size() > 0)) {
        m_useColorArray = true;
        m_colorCloudEnable = false;

        setUseCloudColor(true);
        glColorPointer(GL_BGRA, GL_UNSIGNED_BYTE, 0, &m_cc->abstractColorCloud()->colorAt(0).rgba);
    }
    #endif
}

void DM_VBOManager::postDraw()
{
    if(m_useVertexArray)
        glDisableClientState(GL_VERTEX_ARRAY);

    setUseCloudColor(false);
}
