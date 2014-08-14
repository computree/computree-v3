#include "ct_standardbeamdrawmanager.h"

#include "ct_beam.h"

const QString CT_StandardBeamDrawManager::INDEX_CONFIG_LENGTH_OF_BEAM = CT_StandardBeamDrawManager::staticInitConfigLengthOfRay();

CT_StandardBeamDrawManager::CT_StandardBeamDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_Ray" : drawConfigurationName)
{
    
}

CT_StandardBeamDrawManager::~CT_StandardBeamDrawManager()
{
}

void CT_StandardBeamDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Beam &item = dynamic_cast<const CT_Beam&>(itemDrawable);

    double tMax = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LENGTH_OF_BEAM).toDouble();
    painter.drawLine( item.getOrigin().x(),
                      item.getOrigin().y(),
                      item.getOrigin().z(),
                      item.getOrigin().x()+tMax*item.getDirection().x(),
                      item.getOrigin().y()+tMax*item.getDirection().y(),
                      item.getOrigin().z()+tMax*item.getDirection().z() );
}

CT_ItemDrawableConfiguration CT_StandardBeamDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardBeamDrawManager::staticInitConfigLengthOfRay(), "Length of the ray", CT_ItemDrawableConfiguration::Double, 1.0);

    return item;
}

// PROTECTED //

QString CT_StandardBeamDrawManager::staticInitConfigLengthOfRay()
{
    return "BEAM_LOR";
}

