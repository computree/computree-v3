#include "dm_opengltools.h"

#include "dm_guimanager.h"

#include <QGLFormat>

DM_OpenGLTools::DM_OpenGLTools()
{
    m_maxVertexUniformVec4Initialized = false;
}

void DM_OpenGLTools::init()
{
    QGLFormat def = QGLFormat::defaultFormat();

    if(QGLFormat::openGLVersionFlags().testFlag(QGLFormat::OpenGL_Version_3_0)) {
        def.setVersion(3, 0);
        QGLFormat::setDefaultFormat(def);
    }
}

float DM_OpenGLTools::openGlVersionUsed() const
{
    return QGLFormat::defaultFormat().majorVersion() + (QGLFormat::defaultFormat().minorVersion() / 10.0);
}

GLint DM_OpenGLTools::maxVertexUniformVector(QT_GL_FUNCTIONS &functions) const
{
    if(!m_maxVertexUniformVec4Initialized) {
        m_maxVertexUniformVec4Initialized = true;
        functions.glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_maxVertexUniformVec4);

        if((functions.glGetError() != GL_NO_ERROR) || (m_maxVertexUniformVec4 <= 0)) {
            GUI_LOG->addWarningMessage(LogInterface::gui, QObject::tr("GL_MAX_VERTEX_UNIFORM_VECTORS non supporté !"));
            m_maxVertexUniformVec4 = 0;
        }
    }

    return m_maxVertexUniformVec4;
}
