#ifndef DM_VBOMANAGER_H
#define DM_VBOMANAGER_H

#include "view/DocumentView/gdocumentviewforgraphics.h"

//#define USE_VBO_TEST

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QGLFunctions>
#define QT_GL_FUNCTIONS QGLFunctions
#else
#include <QOpenGLFunctions>
#define QT_GL_FUNCTIONS QOpenGLFunctions
#endif

class DM_VBOManager : protected QT_GL_FUNCTIONS
{
public:
    DM_VBOManager();
    ~DM_VBOManager();

    /**
     * @brief Call this function in your initializeGL()
     */
    void initializeGL();

    /**
     * @brief Set the current cloud color to use
     * @warning Don't call this method when you draw !!! Only before preDraw or after postDraw !
     */
    void setCurrentCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc);

    /**
     * @brief Define if you want use the color cloud or not
     */
    void setUseCloudColor(bool use);

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

    bool                                            m_useColorArray;
    bool                                            m_useVertexArray;
    bool                                            m_colorCloudEnable;
    QSharedPointer<CT_StandardColorCloudRegistered> m_cc;

    bool                                            m_useVbo;
    GLuint                                          m_vbo;
};

#endif // DM_VBOMANAGER_H
