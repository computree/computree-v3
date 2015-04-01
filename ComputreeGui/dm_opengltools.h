#ifndef DM_OPENGLTOOLS_H
#define DM_OPENGLTOOLS_H

#include <qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QGLFunctions>
#define QT_GL_FUNCTIONS QGLFunctions
#else
#include <QOpenGLFunctions>
#define QT_GL_FUNCTIONS QOpenGLFunctions
#endif

class DM_OpenGLTools
{
public:
    DM_OpenGLTools();

    /**
     * @brief Call to init (only call this in main function)
     */
    void init();

    /**
     * @brief Get opengl version used
     */
    float openGlVersionUsed() const;

    /**
     * @brief Get GL_MAX_VERTEX_UNIFORM_VECTORS.
     */
    GLint maxVertexUniformVector(QT_GL_FUNCTIONS &functions) const;

private:
    mutable bool    m_maxVertexUniformVec4Initialized;
    mutable GLint   m_maxVertexUniformVec4;
};

#endif // DM_OPENGLTOOLS_H
