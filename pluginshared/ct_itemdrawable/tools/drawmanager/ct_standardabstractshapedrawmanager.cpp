#include "ct_standardabstractshapedrawmanager.h"

#include <QObject>


CT_StandardAbstractShapeDrawManager::CT_StandardAbstractShapeDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? QObject::tr("3D shape") : drawConfigurationName)
{
}

CT_StandardAbstractShapeDrawManager::~CT_StandardAbstractShapeDrawManager()
{
}
