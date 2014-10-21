#ifndef CT_STANDARDMESHMODELOPFDRAWMANAGER_H
#define CT_STANDARDMESHMODELOPFDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardmeshmodeldrawmanager.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtOpenGL/QGLShaderProgram>
#define QT_GL_SHADERPROGRAM QGLShaderProgram
#define QT_GL_SHADER QGLShader
#define QT_GL_CONTEXT QGLContext
#else
#include <QOpenGLShaderProgram>
#define QT_GL_SHADERPROGRAM QOpenGLShaderProgram
#define QT_GL_SHADER QOpenGLShader
#define QT_GL_CONTEXT QOpenGLContext
#endif

class PLUGINSHAREDSHARED_EXPORT CT_StandardMeshModelOPFDrawManager : public CT_StandardMeshModelDrawManager
{
public:
    CT_StandardMeshModelOPFDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardMeshModelOPFDrawManager();

    virtual bool useItemTransformMatrix() const;

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

    void setDUp(double dUp);
    void setDDown(double dDown);

    double dUp() const;
    double dDown() const;

    /**
     * @brief Set another draw configuration
     * @warning The configuration must at least have elements of the default draw configuration. Use "createDrawConfiguration" to create a default configuration.
     */
    void setDrawConfiguration(CT_ItemDrawableConfiguration *dc);

    /**
     * @brief Set if the draw configuration must be deleted. true by default.
     */
    void setAutoDeleteDrawConfiguration(bool e);

    /**
     * @brief Create a default draw configuration.
     */
    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

private:
    double                      m_dUp;
    double                      m_dDown;
    static QT_GL_SHADERPROGRAM  *SHADER_PROG;
    static QT_GL_SHADER         *SHADER;
    static bool                 SHADER_ERROR;
    static int                  MATRIX_LOCATION;
    static int                  DELTAX_LOCATION;
    static int                  DELTAD_LOCATION;
    static int                  MINX_LOCATION;
    static int                  DDOWN_LOCATION;
    static int                  N_SHARED;

    float                       m_deltaD;

protected:

    void initShaders() const;
};

#endif // CT_STANDARDMESHMODELOPFDRAWMANAGER_H
