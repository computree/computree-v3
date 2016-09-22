#include "ct_standardreferencepointdrawmanager.h"

#include "ct_referencepoint.h"

const QString CT_StandardReferencePointDrawManager::INDEX_CONFIG_BUFFER_VISIBLE = CT_StandardReferencePointDrawManager::staticInitConfigBufferVisible();
const QString CT_StandardReferencePointDrawManager::INDEX_CONFIG_FACTOR = CT_StandardReferencePointDrawManager::staticInitConfigFactor();
const QString CT_StandardReferencePointDrawManager::INDEX_CONFIG_POINT_SIZE = CT_StandardReferencePointDrawManager::staticInitConfigPointSize();

CT_StandardReferencePointDrawManager::CT_StandardReferencePointDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_ReferencePoint" : drawConfigurationName)
{
    
}

CT_StandardReferencePointDrawManager::~CT_StandardReferencePointDrawManager()
{
}

void CT_StandardReferencePointDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_ReferencePoint &item = dynamic_cast<const CT_ReferencePoint&>(itemDrawable);

    double sizeCube = (double) (getDrawConfiguration()->getVariableValue(INDEX_CONFIG_POINT_SIZE).toInt()) / 100.0;

    painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_POINT_SIZE).toInt());
    //painter.drawPoint(item.getCenterX(), item.getCenterY(), item.getCenterZ());
    painter.drawCube(item.getCenterX() - sizeCube, item.getCenterY() - sizeCube, item.getCenterZ() - sizeCube,item.getCenterX() + sizeCube, item.getCenterY() + sizeCube, item.getCenterZ() + sizeCube, GL_FRONT_AND_BACK, GL_FILL);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BUFFER_VISIBLE).toBool())
    {
        painter.drawCircle(item.x(), item.y(), item.z(), item.xyBuffer()*getDrawConfiguration()->getVariableValue(INDEX_CONFIG_FACTOR).toDouble());
    }
    painter.restoreDefaultPointSize();
}

CT_ItemDrawableConfiguration CT_StandardReferencePointDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    //item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardReferencePointDrawManager::staticInitConfigBufferVisible(), "Buffer", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardReferencePointDrawManager::staticInitConfigFactor(), "Facteur multiplicatif", CT_ItemDrawableConfiguration::Double, 1.0);
    item.addNewConfiguration(CT_StandardReferencePointDrawManager::staticInitConfigPointSize(), "Taille de point", CT_ItemDrawableConfiguration::Int, 10);

    return item;
}

// PROTECTED //

QString CT_StandardReferencePointDrawManager::staticInitConfigBufferVisible()
{
    return "RP_BV";
}

QString CT_StandardReferencePointDrawManager::staticInitConfigFactor()
{
    return "RP_F";
}

QString CT_StandardReferencePointDrawManager::staticInitConfigPointSize()
{
    return "RP_S";
}


