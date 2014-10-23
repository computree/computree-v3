#ifndef DM_COLORVBOMANAGER_H
#define DM_COLORVBOMANAGER_H

#include "dm_vertexvbomanager.h"

#include "interfaces.h"

/**
 * @brief A VBO manager for colors : to use in a document with graphics view. One
 *        DM_ColorVBOManager for one document because it exist one colors array by document.
 */
class DM_ColorVBOManager : protected QT_GL_FUNCTIONS
{
public:
    DM_ColorVBOManager();
    ~DM_ColorVBOManager();

    /**
     * @brief Set the current cloud color to use
     * @warning Don't call this method when you draw !!! Only before preDraw or after postDraw !
     */
    void setCurrentColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> cc);

    /**
     * @brief Define if you want use the color cloud or not
     */
    void setUseColorCloud(bool use);

    /**
     * @brief Returns true if the color cloud is used
     */
    bool useColorCloud() const;

    /**
     * @brief Returns true if a vbo is used to draw the color cloud. False if
     *        a glColorPointer(...) is called with the address of the first color.
     */
    bool useVBOForColorCloud() const;

    /**
     * @brief Call this method to refresh the vbo
     */
    void refresh();

    /**
     * @brief Call this function in your initializeGL()
     */
    void initializeGL();

    /**
     * @brief Call in pre draw (enable color array etc...)
     */
    void preDraw();

    /**
     * @brief Call in post draw (disable color array etc...)
     */
    void postDraw();

private:
    bool                                            m_initialized;

    bool                                            m_refresh;
    size_t                                          m_lastSize;

    bool                                            m_useColorArray;
    bool                                            m_colorCloudEnable;
    QSharedPointer<CT_StandardColorCloudRegistered> m_cc;

    bool                                            m_useVbo;
    GLuint                                          m_vbo;
};

#endif // DM_COLORVBOMANAGER_H
