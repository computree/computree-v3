#include "ct_standardmeshmodelopfdrawmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"

#include <QtOpenGL>

QOpenGLShaderProgram* CT_StandardMeshModelOPFDrawManager::SHADER_PROG = NULL;
QOpenGLShader* CT_StandardMeshModelOPFDrawManager::SHADER = NULL;
bool CT_StandardMeshModelOPFDrawManager::SHADER_ERROR = false;
int CT_StandardMeshModelOPFDrawManager::MATRIX_LOCATION = -1;
int CT_StandardMeshModelOPFDrawManager::DELTAX_LOCATION = -1;
int CT_StandardMeshModelOPFDrawManager::DELTAD_LOCATION = -1;
int CT_StandardMeshModelOPFDrawManager::MINX_LOCATION = -1;
int CT_StandardMeshModelOPFDrawManager::DDOWN_LOCATION = -1;
int CT_StandardMeshModelOPFDrawManager::N_SHARED = 0;

CT_StandardMeshModelOPFDrawManager::CT_StandardMeshModelOPFDrawManager(QString drawConfigurationName) : CT_StandardMeshModelDrawManager(drawConfigurationName)
{
    m_dUp = 1.0;
    m_dDown = 1.0;
    ++N_SHARED;

    if(SHADER_PROG == NULL)
        SHADER_PROG = new QOpenGLShaderProgram();
}

CT_StandardMeshModelOPFDrawManager::~CT_StandardMeshModelOPFDrawManager()
{
    --N_SHARED;

    if(N_SHARED == 0)
    {
        delete SHADER;
        SHADER = NULL;

        delete SHADER_PROG;
        SHADER_PROG = NULL;
    }
}

// add overloaded functions which call the underlying OpenGL function
inline void glMultMatrix(const GLfloat  *m) { glMultMatrixf(m); }
inline void glMultMatrix(const GLdouble *m) { glMultMatrixd(m); }

// add an overload for QMatrix4x4 for convenience
inline void glMultMatrix(const QMatrix4x4 &m) { glMultMatrix(m.constData()); }

void CT_StandardMeshModelOPFDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    Q_UNUSED(view)

    const CT_AbstractMeshModel &item = dynamic_cast<const CT_AbstractMeshModel&>(itemDrawable);

    bool ok = false;

    if((dUp() != 1.0) || (dDown() != 1.0))
    {
        initShaders();

        if(!SHADER_ERROR && (QOpenGLContext::currentContext() != NULL))
        {
            if(SHADER_PROG->isLinked())
            {
                if(!SHADER_PROG->bind())
                {
                    QString log = SHADER_PROG->log();

                    if(!log.isEmpty())
                        PS_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("CT_StandardMeshModelOPFDrawManager => Bind error : %1").arg(SHADER_PROG->log()));
                }
                else
                    ok = true;
            }
        }
    }

    if(ok)
    {
        if(MATRIX_LOCATION == -1)
        {
            MATRIX_LOCATION = SHADER_PROG->uniformLocation("tMatrix");
            DELTAX_LOCATION = SHADER_PROG->uniformLocation("deltaX");
            DELTAD_LOCATION = SHADER_PROG->uniformLocation("deltaD");
            MINX_LOCATION = SHADER_PROG->uniformLocation("minX");
            DDOWN_LOCATION = SHADER_PROG->uniformLocation("dDown");
        }

        QVector3D min, max;
        item.getBoundingBox(min, max);

        float deltaX = max.x() - min.x();

        SHADER_PROG->setUniformValue(MATRIX_LOCATION, item.transformMatrix());
        SHADER_PROG->setUniformValue(DELTAX_LOCATION, (float)deltaX);
        SHADER_PROG->setUniformValue(DELTAD_LOCATION, (float)m_deltaD);
        SHADER_PROG->setUniformValue(MINX_LOCATION, (float)min.x());
        SHADER_PROG->setUniformValue(DDOWN_LOCATION, (float)m_dDown);
    }
    else
    {
        glPushMatrix();
        glMultMatrix(item.transformMatrix());
    }

    bool showFaces = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_FACES).toBool();
    bool showEdges = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_EDGES).toBool();
    bool showPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_POINTS).toBool();

    if(showFaces)
        painter.drawFaces(&item);

    if(showEdges)
        painter.drawEdges(&item);

    if(showPoints)
        painter.drawPoints(&item, 4);

    if(!ok)
        glPopMatrix();
    else
        SHADER_PROG->release();
}

void CT_StandardMeshModelOPFDrawManager::setDUp(double dUp)
{
    m_dUp = dUp;
    m_deltaD = m_dUp - m_dDown;
}

void CT_StandardMeshModelOPFDrawManager::setDDown(double dDown)
{
    m_dDown = dDown;
    m_deltaD = m_dUp - m_dDown;
}

double CT_StandardMeshModelOPFDrawManager::dUp() const
{
    return m_dUp;
}

double CT_StandardMeshModelOPFDrawManager::dDown() const
{
    return m_dDown;
}

void CT_StandardMeshModelOPFDrawManager::setDrawConfiguration(CT_ItemDrawableConfiguration *dc)
{
    internalSetDrawConfiguration(dc);
}

void CT_StandardMeshModelOPFDrawManager::setAutoDeleteDrawConfiguration(bool e)
{
    internalSetAutoDeleteDrawConfiguration(e);
}

CT_ItemDrawableConfiguration CT_StandardMeshModelOPFDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    return CT_StandardMeshModelDrawManager::createDrawConfiguration(drawConfigurationName);
}

void CT_StandardMeshModelOPFDrawManager::initShaders() const
{
    if(!SHADER_ERROR && (QOpenGLContext::currentContext() != NULL))
    {
        if(SHADER == NULL)
        {
            SHADER = new QOpenGLShader(QOpenGLShader::Vertex);

            if(!SHADER->compileSourceFile("./shaders/opfMesh.vert"))
            {
                PS_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("CT_StandardMeshModelOPFDrawManager => Vertex shader compilation error : %1").arg(SHADER->log()));

                delete SHADER;
                SHADER = NULL;

                SHADER_ERROR = true;
            }
            else
            {
                SHADER_PROG->addShader(SHADER);
            }

            if(!SHADER_PROG->shaders().isEmpty() && !SHADER_PROG->isLinked())
            {
                if(!SHADER_PROG->link())
                {
                    PS_LOG->addErrorMessage(LogInterface::unknow, QObject::tr("CT_StandardMeshModelOPFDrawManager => Link error : %1").arg(SHADER_PROG->log()));
                    SHADER_ERROR = true;
                }
            }
        }
    }
}
