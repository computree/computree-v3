#include "ct_abstractitemdrawabledrawmanager.h"

#include "ct_itemdrawable/ct_itemdrawableconfiguration.h"

CT_AbstractItemDrawableDrawManager::CT_AbstractItemDrawableDrawManager(const QString &drawConfigurationName)
{
    _configuration = NULL;
    _drawConfigurationNameBackup = drawConfigurationName.isEmpty() ? "CT_AbstractItemDrawable" : drawConfigurationName;
}

CT_AbstractItemDrawableDrawManager::~CT_AbstractItemDrawableDrawManager()
{
    delete _configuration;
}

void CT_AbstractItemDrawableDrawManager::initDrawConfiguration()
{
    delete _configuration;
    _configuration = new CT_ItemDrawableConfiguration( createDrawConfiguration(_drawConfigurationNameBackup) );
}

CT_ItemDrawableConfiguration* CT_AbstractItemDrawableDrawManager::getDrawConfiguration() const
{
    return _configuration;
}

CT_ItemDrawableConfiguration CT_AbstractItemDrawableDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    return CT_ItemDrawableConfiguration(drawConfigurationName);
}
