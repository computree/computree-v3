#include "ct_standardscannerdrawmanager.h"

#include "ct_scanner.h"

#include <Eigen/Dense>
#include <QObject>
#include <QScopedPointer>
#include <QtMath>

const QString CT_StandardScannerDrawManager::INDEX_CONFIG_FIELD_OF_VIEW = CT_StandardScannerDrawManager::staticInitConfigFieldOfView();

CT_StandardScannerDrawManager::CT_StandardScannerDrawManager(QString drawConfigurationName) : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? CT_Scanner::staticName() : drawConfigurationName)
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
    item.addNewConfiguration(CT_StandardScannerDrawManager::staticInitConfigFieldOfView(), QObject::tr("Field of view scale"), CT_ItemDrawableConfiguration::Double, 1.0 );       // How big will be the field of view

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
    double clockWiseScaler = scanner.getClockWise() ? -1.0 : 1.0;
    double endTheta = scanner.getInitTheta() + clockWiseScaler * scanner.getHFov();
    double endPhi = scanner.getInitPhi() + scanner.getVFov();

    /*
    painter.drawPartOfSphere(scanner.getPositionX(),
                             scanner.getPositionY(),
                             scanner.getPositionZ(),
                             scaling,
                             scanner.getInitTheta(),
                             endTheta,
                             scanner.getInitPhi(),
                             endPhi);
    */

    CT_ThetaPhiShootingPattern* pattern = dynamic_cast<CT_ThetaPhiShootingPattern*>(scanner.getShootingPattern());
    QScopedPointer<CT_ThetaPhiShootingPattern> patternSimple(dynamic_cast<CT_ThetaPhiShootingPattern*>(pattern->clone()));
    patternSimple->setHRes(qDegreesToRadians(10.));
    patternSimple->setVRes(qDegreesToRadians(10.));
    size_t n = patternSimple->getNumberOfShots();

    for (size_t i = 0; i < n; i++) {
        const CT_Shot& shot = patternSimple->getShotAt(i);
        double x0 = shot.getOrigin().x();
        double y0 = shot.getOrigin().y();
        double z0 = shot.getOrigin().z();
        Eigen::Vector3d dir = shot.getDirection();
        dir.normalize();
        Eigen::Vector3d v = dir + shot.getOrigin();
        painter.drawLine(x0, y0, z0, v.x(), v.y(), v.z());
    }

    // Drawing the four major limits of the field of view
    /*
    double midPhi = scanner.getVFov()/2 + scanner.getInitPhi();
    drawLineToPosition(painter, scanner, endTheta, scanner.getInitPhi());
    drawLineToPosition(painter, scanner, endTheta, endPhi);
    drawLineToPosition(painter, scanner, endTheta, midPhi);
    */
}

void CT_StandardScannerDrawManager::drawLineToPosition(PainterInterface &painter, const CT_Scanner &scanner, double theta, double phi) const
{
    double scaling = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_FIELD_OF_VIEW).toDouble();

    double cosPhi, sinPhi, cosTheta, sinTheta;
    sinTheta = sin(theta);
    cosTheta = cos(theta);
    sinPhi = sin(phi);
    cosPhi = cos(phi);

    painter.drawLine(scanner.getPositionX(),
                     scanner.getPositionY(),
                     scanner.getPositionZ(),
                     scanner.getPositionX() + scaling*sinPhi*cosTheta,
                     scanner.getPositionY() + scaling*sinPhi*sinTheta,
                     scanner.getPositionZ() + scaling*cosPhi);
}
