#include "ct_standardabstractshapedrawmanager.h"

CT_StandardAbstractShapeDrawManager::CT_StandardAbstractShapeDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_AbstractShape" : drawConfigurationName)
{
}

CT_StandardAbstractShapeDrawManager::~CT_StandardAbstractShapeDrawManager()
{
}
