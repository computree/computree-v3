#include "ct_standardscannerdrawmanager.h"

#include "ct_scanner.h"

const QString CT_StandardScannerDrawManager::INDEX_CONFIG_FIELD_OF_VIEW = CT_StandardScannerDrawManager::staticInitConfigFieldOfView();

CT_StandardScannerDrawManager::CT_StandardScannerDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? "CT_Scanner" : drawConfigurationName)
{
    
}

CT_StandardScannerDrawManager::~CT_StandardScannerDrawManager()
{
}

void CT_StandardScannerDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_Scanner &item = dynamic_cast<const CT_Scanner&>(itemDrawable);

    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    // Drawing the field of view
    drawFieldOfView(painter, item);
}

CT_ItemDrawableConfiguration CT_StandardScannerDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    // Creating the configuration box for the view of the item drawable
    CT_ItemDrawableConfiguration item(drawConfigurationName);

    // Adding the configuration inherited from the CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager class
    item.addAllConfigurationOf( CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));

    // Adding lines to this config dialog box
    item.addNewConfiguration(CT_StandardScannerDrawManager::staticInitConfigFieldOfView(), "Field of view scale", CT_ItemDrawableConfiguration::Double, 1.0 );       // How big will be the field of view

    return item;
}

// PROTECTED //

QString CT_StandardScannerDrawManager::staticInitConfigFieldOfView()
{
    return "SC_FOV";
}

void CT_StandardScannerDrawManager::drawFieldOfView(PainterInterface &painter, const CT_Scanner &scanner) const
{
    double scaling = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_FIELD_OF_VIEW).toDouble();

    if ( !scanner.getClockWise() )
    {
        painter.drawPartOfSphere( scanner.getPosition().x(), scanner.getPosition().y(), scanner.getPosition().z(), scaling, scanner.getInitTheta(), scanner.getInitTheta()+scanner.getHFov(), scanner.getInitPhi(), scanner.getInitPhi() + scanner.getVFov());

        // Drawing the four major limits of the field of view
        drawLineToPosition( painter, scanner, scanner.getInitTheta(), scanner.getInitPhi() );
        drawLineToPosition( painter, scanner, scanner.getInitTheta() + scanner.getHFov(), scanner.getInitPhi() );
        drawLineToPosition( painter, scanner, scanner.getInitTheta() + scanner.getHFov(), scanner.getInitPhi() + scanner.getVFov() );
        drawLineToPosition( painter, scanner, scanner.getInitTheta(), scanner.getInitPhi() + scanner.getVFov() );
    }

    else
    {
        painter.drawPartOfSphere( scanner.getPosition().x(), scanner.getPosition().y(), scanner.getPosition().z(), scaling, scanner.getInitTheta()-scanner.getHFov(), scanner.getInitTheta(), scanner.getInitPhi(), scanner.getInitPhi() + scanner.getVFov());

        // Drawing the four major limits of the field of view
        drawLineToPosition( painter, scanner, scanner.getInitTheta()-scanner.getHFov(), scanner.getInitPhi() );
        drawLineToPosition( painter, scanner, scanner.getInitTheta(), scanner.getInitPhi() );
        drawLineToPosition( painter, scanner, scanner.getInitTheta(), scanner.getInitPhi() + scanner.getVFov() );
        drawLineToPosition( painter, scanner, scanner.getInitTheta() - scanner.getHFov(), scanner.getInitPhi() + scanner.getVFov() );
    }
}

void CT_StandardScannerDrawManager::drawLineToPosition(PainterInterface &painter, const CT_Scanner &scanner, double theta, double phi) const
{
    double scaling = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_FIELD_OF_VIEW).toDouble();

    double cosPhi, sinPhi, cosTheta, sinTheta;
    sinTheta = sin (theta);
    cosTheta = cos (theta);
    sinPhi = sin (phi);
    cosPhi = cos (phi);
    painter.drawLine(scanner.getPosition().x(), scanner.getPosition().y(), scanner.getPosition().z(),
                     scaling*sinPhi*cosTheta + scanner.getPosition().x(), scaling*sinPhi*sinTheta + scanner.getPosition().y(), scaling*cosPhi + scanner.getPosition().z() );
}
