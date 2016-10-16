#include "ct_standardabstractshapedrawmanager.h"
#include "ct_itemdrawable/abstract/ct_abstractshape.h"

CT_StandardAbstractShapeDrawManager::CT_StandardAbstractShapeDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? CT_AbstractShape::staticName() : drawConfigurationName)
{
}

CT_StandardAbstractShapeDrawManager::~CT_StandardAbstractShapeDrawManager()
{
}
