#include "dm_vertexvbomanager.h"

#include <QtOpenGL>

#include "dm_guimanager.h"

#include "ct_cloud/abstract/ct_abstractcloudt.h"
#include "ct_global/ct_context.h"

DM_VertexVBOManager::DM_VertexVBOManager()
{
    m_vbo = 0;
    m_lastSize = 0;
    m_useVbo = false;
    m_initialized = false;
    m_refresh = false;
}

DM_VertexVBOManager::~DM_VertexVBOManager()
{
    if(m_initialized && (m_vbo != 0))
        glDeleteBuffers(1, &m_vbo);
}

bool DM_VertexVBOManager::useVBOForVertexCloud() const
{
    return m_useVbo;
}

void DM_VertexVBOManager::refresh()
{
    m_refresh = true;
}

void DM_VertexVBOManager::initializeGL()
{
    if(!m_initialized) {
        QT_GL_INIT_FUNCTIONS();

        // create a vbo
        glGenBuffers(1, &m_vbo);

        m_initialized = true;
    }
}

void DM_VertexVBOManager::preDraw()
{
    CT_AbstractCloudT<CT_Point> *points = PS_REPOSITORY->globalCloud<CT_Point>();

    size_t size = points->size();

    // if size changed we refresh the vbo
    if((size != m_lastSize) || m_refresh)
    {
        m_useVbo = false;
        m_refresh = false;

        /*size_t nBytes = size*sizeof(CT_Point);

        // select the vbo
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // allocate nBytes in this vbo
        if(size != m_lastSize)
            glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);

        // if allocation is ok and size > 0
        if((glGetError() == GL_NO_ERROR) && (size > 0)) {

            // copy the points cloud in this vbo
            glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, points->constTAt(0).vertex());

            // set use vbo to true
            if(glGetError() == GL_NO_ERROR)
                m_useVbo = true;
        }

        // deselect the vbo
        glBindBuffer(GL_ARRAY_BUFFER, 0);*/

        m_lastSize = size;

        //GUI_LOG->addDebugMessage(LogInterface::gui, QString("Use vertex vbo : %1").arg(m_useVbo ? "true" : "false"));
    }

    m_useVertexArray = false;

    // if size > 0
    if(m_lastSize > 0) {
        m_useVertexArray = true;

        // enable vertex array
        glEnableClientState(GL_VERTEX_ARRAY);

        // if vbo is ok
        if(m_useVbo)
        {
            // select the vbo
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            // define the address of the points cloud in VRAM (RAM of graphics card)
            glVertexPointer(sizeof(CT_Point)/sizeof(float), GL_FLOAT, 0, 0);

            // unselect the vbo
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else // define the address of the points cloud in RAM
            glVertexPointer(sizeof(CT_Point)/sizeof(float), GL_FLOAT, 0, points->constTAt(0).vertex());
    }
}

void DM_VertexVBOManager::postDraw()
{
    if(m_useVertexArray)
        glDisableClientState(GL_VERTEX_ARRAY);
}
