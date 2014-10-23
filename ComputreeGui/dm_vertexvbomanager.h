#ifndef DM_VERTEXVBOMANAGER_H
#define DM_VERTEXVBOMANAGER_H

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QGLFunctions>
#define QT_GL_FUNCTIONS QGLFunctions
#define QT_GL_INIT_FUNCTIONS initializeGLFunctions
#else
#include <QOpenGLFunctions>
#define QT_GL_FUNCTIONS QOpenGLFunctions
#define QT_GL_INIT_FUNCTIONS initializeOpenGLFunctions
#endif

/**
 * @brief A VBO manager for vertex. Only one DM_VertexVBOManager for all the application, because
 *        it exist only one global vertex array.
 */
class DM_VertexVBOManager : protected QT_GL_FUNCTIONS
{
public:
    DM_VertexVBOManager();
    ~DM_VertexVBOManager();

    /**
     * @brief Returns true if a vbo is used to draw the vertex cloud. False if
     *        a glVertexPointer(...) is called with the address of the first point.
     */
    bool useVBOForVertexCloud() const;

    /**
     * @brief Call this method to refresh the vbo
     */
    void refresh();

    /**
     * @brief Call this function in your initializeGL()
     */
    void initializeGL();

    /**
     * @brief Call in pre draw (enable vertex array etc...)
     */
    void preDraw();

    /**
     * @brief Call in post draw (disable vertex array etc...)
     */
    void postDraw();

private:

    bool                                            m_initialized;

    size_t                                          m_lastSize;

    bool                                            m_useVertexArray;

    bool                                            m_useVbo;
    GLuint                                          m_vbo;

    bool                                            m_refresh;
};

#endif // DM_VERTEXVBOMANAGER_H
