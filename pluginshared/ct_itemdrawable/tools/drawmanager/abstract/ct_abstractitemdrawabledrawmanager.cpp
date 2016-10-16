#include "ct_abstractitemdrawabledrawmanager.h"

#include "ct_itemdrawable/ct_itemdrawableconfiguration.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

CT_AbstractItemDrawableDrawManager::CT_AbstractItemDrawableDrawManager(const QString &drawConfigurationName)
{
    m_autoDeleteConfig = true;
    _configuration = NULL;
    _drawConfigurationNameBackup = drawConfigurationName.isEmpty() ? CT_AbstractItemDrawable::staticName() : drawConfigurationName;
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

void CT_AbstractItemDrawableDrawManager::preDraw(GraphicsViewInterface &view,
                                                 PainterInterface &painter,
                                                 const CT_AbstractItemDrawable &itemDrawable)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)
    Q_UNUSED(itemDrawable)
}

void CT_AbstractItemDrawableDrawManager::postDraw(GraphicsViewInterface &view,
                                                  PainterInterface &painter,
                                                  const CT_AbstractItemDrawable &itemDrawable)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)
    Q_UNUSED(itemDrawable)
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
