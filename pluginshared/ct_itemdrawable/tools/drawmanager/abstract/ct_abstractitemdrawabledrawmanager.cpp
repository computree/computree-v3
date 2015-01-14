#include "ct_abstractitemdrawabledrawmanager.h"

#include "ct_itemdrawable/ct_itemdrawableconfiguration.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

CT_AbstractItemDrawableDrawManager::CT_AbstractItemDrawableDrawManager(const QString &drawConfigurationName)
{
    m_autoDeleteConfig = true;
    _configuration = NULL;
    _drawConfigurationNameBackup = drawConfigurationName.isEmpty() ? "CT_AbstractItemDrawable" : drawConfigurationName;
}

CT_AbstractItemDrawableDrawManager::~CT_AbstractItemDrawableDrawManager()
{
    if(m_autoDeleteConfig)
        delete _configuration;
}

void CT_AbstractItemDrawableDrawManager::initDrawConfiguration()
{
    if(m_autoDeleteConfig)
        delete _configuration;

    _configuration = new CT_ItemDrawableConfiguration( createDrawConfiguration(_drawConfigurationNameBackup) );
}

bool CT_AbstractItemDrawableDrawManager::useItemTransformMatrix() const
{
    return false;
}

void CT_AbstractItemDrawableDrawManager::preDraw(GraphicsViewInterface &view,
                                                 PainterInterface &painter,
                                                 const CT_AbstractItemDrawable &itemDrawable)
{
    Q_UNUSED(view)

    if(!useItemTransformMatrix() && !itemDrawable.transformMatrix().isIdentity())
    {
        painter.pushMatrix();
        QMatrix4x4 m = itemDrawable.transformMatrix();
        Eigen::Matrix4d em;
        em << m(0, 0), m(0, 1), m(0, 2), m(0, 3),
              m(1, 0), m(1, 1), m(1, 2), m(1, 3),
              m(2, 0), m(2, 1), m(2, 2), m(2, 3),
              m(3, 0), m(3, 1), m(3, 2), m(3, 3);
        painter.multMatrix(em);
    }
}

void CT_AbstractItemDrawableDrawManager::postDraw(GraphicsViewInterface &view,
                                                  PainterInterface &painter,
                                                  const CT_AbstractItemDrawable &itemDrawable)
{
    Q_UNUSED(view)
    Q_UNUSED(itemDrawable)

    if(!useItemTransformMatrix() && !itemDrawable.transformMatrix().isIdentity())
        painter.popMatrix();
}

CT_ItemDrawableConfiguration* CT_AbstractItemDrawableDrawManager::getDrawConfiguration() const
{
    return _configuration;
}

CT_ItemDrawableConfiguration CT_AbstractItemDrawableDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    return CT_ItemDrawableConfiguration(drawConfigurationName);
}

void CT_AbstractItemDrawableDrawManager::internalSetDrawConfiguration(CT_ItemDrawableConfiguration *configuration)
{
    if(m_autoDeleteConfig)
        delete _configuration;

    _configuration = configuration;
}

void CT_AbstractItemDrawableDrawManager::internalSetAutoDeleteDrawConfiguration(bool e)
{
    m_autoDeleteConfig = e;
}
