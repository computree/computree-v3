#ifndef CT_STANDARDPROFILEDRAWMANAGER_HPP
#define CT_STANDARDPROFILEDRAWMANAGER_HPP

#include "ct_itemdrawable/tools/drawmanager/ct_standardprofiledrawmanager.h"
#include "ct_itemdrawable/ct_profile.h"
#include <typeinfo>

#define _USE_MATH_DEFINES
#include <math.h>

#include "qdebug.h"

template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_COLUMN_MODE_ENABLED = CT_StandardProfileDrawManager<DataT>::staticInitConfigColumnModeEnabled();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_COLUMN_RADIUS = CT_StandardProfileDrawManager<DataT>::staticInitConfigColumnRadius();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_GRAPH_MODE_ENABLED = CT_StandardProfileDrawManager<DataT>::staticInitConfigGraphModeEnabled();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_DRAW_AXIS = CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawAxis();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_DRAW_BARS = CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawBars();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_DRAW_POINTS = CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawPoints();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_POINTS_SIZE = CT_StandardProfileDrawManager<DataT>::staticInitConfigPointsSize();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_DRAW_CURVE = CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawCurve();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_GRAPH_ORIENTATION = CT_StandardProfileDrawManager<DataT>::staticInitConfigGraphOrientation();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_SCALE = CT_StandardProfileDrawManager<DataT>::staticInitConfigScale();
template< typename DataT > const QString CT_StandardProfileDrawManager<DataT>::INDEX_CONFIG_SCALE8COEFFICIENT = CT_StandardProfileDrawManager<DataT>::staticInitConfigScaleCoeff();

template< typename DataT >
CT_StandardProfileDrawManager<DataT>::CT_StandardProfileDrawManager(QString drawConfigurationName)
    : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? QString("CT_Profil<%1>").arg(typeid(DataT).name()) : drawConfigurationName)
{
    
}

template< typename DataT >
CT_StandardProfileDrawManager<DataT>::~CT_StandardProfileDrawManager()
{
}

template< typename DataT >
void CT_StandardProfileDrawManager<DataT>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Profile<DataT> &item = dynamic_cast<const CT_Profile<DataT>&>(itemDrawable);

    // Getting the configuration values
    bool    columnMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_COLUMN_MODE_ENABLED).toBool();
    double  radius = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_COLUMN_RADIUS).toDouble();
    bool    graphMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_GRAPH_MODE_ENABLED).toBool();
    bool    drawAxis = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_AXIS).toBool();
    bool    drawBars = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_BARS).toBool();
    bool    drawPoints = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_POINTS).toBool();
    int     pointsSize = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_POINTS_SIZE).toInt();
    bool    drawCurve = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_DRAW_CURVE).toBool();
    int     orientation = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_GRAPH_ORIENTATION).toInt();
    bool    scale = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SCALE).toBool();
    double  scaleCoeff = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SCALE8COEFFICIENT).toDouble();

    if (radius <= 0) {radius = 0.50;}
    if (orientation < 0) {orientation = 0;}
    if (scaleCoeff <= 0) {scaleCoeff = 0.1;}
    if (orientation > 360) {orientation = 360;}

    double amplitude = item.dataMax() - item.dataMin();
    double scaling = 2*scaleCoeff / amplitude;
    float orientationRad = M_PI * (orientation % 360) / 180.0;
    const QVector3D &direction = item.getDirection();

    float lastX = 0;
    float lastY = 0;
    float lastZ = 0;
    bool lastNa = false;

    for (size_t index = 0 ; index < item.nCells() ; index++)
    {
        bool isNa = (item.valueAtIndex(index) == item.NA());
        double value = item.valueAtIndexAsDouble(index);

        QVector3D center;

        if (item.getCellCenterXYZ(index, center))
        {
            QColor color = painter.getColor();
            if (columnMode)
            {
                if (isNa)
                {
                    painter.setColor(QColor(125, 0, 0));
                }  else {
                    int colorLevel = ((value - (double)item.dataMin()) / amplitude)*255;
                    painter.setColor(QColor(colorLevel, colorLevel, colorLevel));
                }

                painter.drawPoint(center.x(), center.y(), center.z());
                painter.drawCylinder3D(center, direction, radius, item.resolution());
            }
            painter.setColor(color);

            if (graphMode)
            {
                if (scale) {value = (value - (double)item.dataMin()) * scaling;}

                QVector3D normalVector;
                if ((direction.x() >= direction.y()) && (direction.x() >= direction.z()))
                {
                    normalVector.setX(0); // = 0
                    normalVector.setY(cos(orientationRad));
                    normalVector.setZ(-sin(orientationRad));
                } else if ((direction.y() >= direction.x()) && (direction.y() >= direction.z())){
                    normalVector.setX(sin(orientationRad));
                    normalVector.setY(0); // = 0
                    normalVector.setZ(cos(orientationRad));
                } else {
                    normalVector.setX(cos(orientationRad));
                    normalVector.setY(-sin(orientationRad));
                    normalVector.setZ(0); // = 0
                }
                normalVector.normalize();

                if (isNa)
                {
                    value = 0;
                    painter.setColor(QColor(125, 0, 0));
                }

                if (drawBars)
                {
                    QVector3D pt1, pt2, pt3, pt4;

                    if (item.getCellCoordinates(index, pt1, pt2))
                    {
                        pt3.setX(pt2.x() + normalVector.x()*value);
                        pt3.setY(pt2.y() + normalVector.y()*value);
                        pt3.setZ(pt2.z() + normalVector.z()*value);

                        pt4.setX(pt1.x() + normalVector.x()*value);
                        pt4.setY(pt1.y() + normalVector.y()*value);
                        pt4.setZ(pt1.z() + normalVector.z()*value);

                        painter.drawLine(pt1.x(), pt1.y(), pt1.z(), pt2.x(), pt2.y(), pt2.z());
                        painter.drawLine(pt2.x(), pt2.y(), pt2.z(), pt3.x(), pt3.y(), pt3.z());
                        painter.drawLine(pt3.x(), pt3.y(), pt3.z(), pt4.x(), pt4.y(), pt4.z());
                        painter.drawLine(pt4.x(), pt4.y(), pt4.z(), pt1.x(), pt1.y(), pt1.z());
                    }
                }

                if (drawPoints || drawCurve)
                {
                    float x = center.x() + normalVector.x()*value;
                    float y = center.y() + normalVector.y()*value;
                    float z = center.z() + normalVector.z()*value;

                    if (drawCurve && index > 0)
                    {
                        if (lastNa) {painter.setColor(QColor(125, 0, 0));}
                        painter.drawLine(lastX, lastY, lastZ, x, y, z);
                    }

                    if (drawPoints && index > 0)
                    {
                        painter.setPointSize(pointsSize);
                        painter.enableSetPointSize(false);
                        painter.drawPoint(x, y, z);
                        painter.enableSetPointSize(true);
                        painter.restoreDefaultPointSize();
                    }

                    lastX = x;
                    lastY = y;
                    lastZ = z;
                    lastNa = isNa;
                }
                painter.setColor(color);
            }
        }
    }

    if (graphMode && drawAxis)
    {
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.maxX(), item.maxY(), item.maxZ());
    }
}

template< typename DataT >
CT_ItemDrawableConfiguration CT_StandardProfileDrawManager<DataT>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addNewConfiguration(staticInitConfigColumnModeEnabled(), "Mode colonne", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigColumnRadius(), "Rayon de colonne", CT_ItemDrawableConfiguration::Double, 0.50);
    item.addNewConfiguration(staticInitConfigGraphModeEnabled(), "Mode graph", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigDrawAxis(), "Dessiner axe", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigDrawBars(), "Dessiner barres", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigDrawPoints(), "Dessiner points", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigPointsSize(), "Taille des points", CT_ItemDrawableConfiguration::Int, 5);
    item.addNewConfiguration(staticInitConfigDrawCurve(), "Dessiner courbe", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigGraphOrientation(), "Orientation graph", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigScale(), "Mise à l'échelle", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigScaleCoeff(), "Coefficient d'échelle (m)", CT_ItemDrawableConfiguration::Double, 5);

    return item;
}

// PROTECTED //

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigColumnModeEnabled()
{
    return "PROF_COL";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigColumnRadius()
{
    return "PROF_RAD";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigGraphModeEnabled()
{
    return "PROF_GRA";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawAxis()
{
    return "PROF_AXI";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawBars()
{
    return "PROF_BAR";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawPoints()
{
    return "PROF_PTS";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigPointsSize()
{
    return "PROF_SZP";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigDrawCurve()
{
    return "PROF_CUR";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigGraphOrientation()
{
    return "PROF_ORI";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigScale()
{
    return "PROF_SCA";
}

template< typename DataT >
QString CT_StandardProfileDrawManager<DataT>::staticInitConfigScaleCoeff()
{
    return "PROF_COE";
}

#endif // CT_STANDARDPROFILEDRAWMANAGER_HPP
