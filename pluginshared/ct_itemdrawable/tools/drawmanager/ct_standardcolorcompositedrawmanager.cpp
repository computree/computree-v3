#include "ct_standardcolorcompositedrawmanager.h"

#include <stdlib.h>
#include <time.h>

#ifdef USE_OPENCV
#include "ct_itemdrawable/ct_colorcomposite.h"
#include "ct_tools/ct_typeinfo.h"

#include <ctime>

#include <QObject>

const QString CT_StandardColorCompositeDrawManager::INDEX_CONFIG_3D_MODE_ENABLED = CT_StandardColorCompositeDrawManager::staticInitConfig3DModeEnabled();
const QString CT_StandardColorCompositeDrawManager::INDEX_CONFIG_MAP_MODE_ENABLED = CT_StandardColorCompositeDrawManager::staticInitConfigMapModeEnabled();
const QString CT_StandardColorCompositeDrawManager::INDEX_CONFIG_MAP_MODE_ZLEVEL_ENABLED = CT_StandardColorCompositeDrawManager::staticInitConfigMapModeZLevelEnabled();
const QString CT_StandardColorCompositeDrawManager::INDEX_CONFIG_MAP_MODE_ZLEVEL_VALUE = CT_StandardColorCompositeDrawManager::staticInitConfigMapModeZLevelValue();

CT_StandardColorCompositeDrawManager::CT_StandardColorCompositeDrawManager(QString drawConfigurationName, bool mapMode, bool scale)
    : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? CT_ColorComposite::staticName() : drawConfigurationName)
{
    _defaultMapMode = mapMode;
    _defaultScaleState = scale;
}

CT_StandardColorCompositeDrawManager::~CT_StandardColorCompositeDrawManager()
{
}

void CT_StandardColorCompositeDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_ColorComposite &item = dynamic_cast< const CT_ColorComposite& >(itemDrawable);

    const CT_Image2D<quint8>* red = item.getRedBand();
    const CT_Image2D<quint8>* green = item.getGreenBand();
    const CT_Image2D<quint8>* blue = item.getBlueBand();
    const CT_Image2D<float>*  zvalue = item.getZValueRaster();

    if (red != NULL && green != NULL && blue != NULL)
    {

        bool mode3D = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_3D_MODE_ENABLED).toBool();
        bool modeMap = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_MAP_MODE_ENABLED).toBool();
        bool fixerZ = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_MAP_MODE_ZLEVEL_ENABLED).toBool();
        double z = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_MAP_MODE_ZLEVEL_VALUE).toDouble();

        double demiRes = red->resolution()/2.0;

        if (mode3D && zvalue != NULL)
        {
            // sinon dessiner seulement les points
            double x, y, value;

            for (size_t ly = 0 ; ly < red->linDim()-1 ; ly++)
            {
                for (size_t cx = 0 ; cx < red->colDim()-1 ; cx++)
                {
                    x = red->getCellCenterColCoord(cx);
                    y = red->getCellCenterLinCoord(ly);

                    value = zvalue->valueAtCoords(x, y);

                    if (value != zvalue->NA())
                    {
                        int r = red->value(cx, ly);
                        int g = green->value(cx, ly);
                        int b = blue->value(cx, ly);

                        painter.setColor(r, g, b);

                        painter.drawPoint(x, y, value);
                    }
                }
            }
        }

        if (modeMap) {
            double z_val = red->level();
            if (fixerZ) {z_val = z;}

            for (size_t cx = 0 ; cx < red->colDim() ; cx++)
            {
                for (size_t ly = 0 ; ly < red->linDim() ; ly++)
                {
                    double x = red->getCellCenterColCoord(cx);
                    double y = red->getCellCenterLinCoord(ly);

                    int r = red->value(cx, ly);
                    int g = green->value(cx, ly);
                    int b = blue->value(cx, ly);

                    painter.setColor(QColor(r, g, b));

                    Eigen::Vector2d tLeft(x - demiRes, y + demiRes);
                    Eigen::Vector2d bRight(tLeft(0) + red->resolution(), tLeft(1) - red->resolution());

                    painter.fillRectXY(tLeft, bRight, z_val);

                }
            }
        }
    }
}

CT_ItemDrawableConfiguration CT_StandardColorCompositeDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));

    // Adding lines to this config dialog box
    item.addNewConfiguration(staticInitConfigMapModeEnabled(), QObject::tr("Mode Raster"), CT_ItemDrawableConfiguration::Bool, _defaultMapMode);
    item.addNewConfiguration(staticInitConfigMapModeZLevelEnabled(), QObject::tr("Mode Raster : Fixer le niveau Z"), CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigMapModeZLevelValue(), QObject::tr("Mode Raster : Niveau Z (m)"), CT_ItemDrawableConfiguration::Double, 0);

    item.addNewConfiguration(staticInitConfig3DModeEnabled(), QObject::tr("Mode 3D"), CT_ItemDrawableConfiguration::Bool, !_defaultMapMode);

    return item;
}

// PROTECTED //

QString CT_StandardColorCompositeDrawManager::staticInitConfig3DModeEnabled()
{
    return "CCOMP_3DME";
}


QString CT_StandardColorCompositeDrawManager::staticInitConfigMapModeEnabled()
{
    return "CCOMP_MME";
}

QString CT_StandardColorCompositeDrawManager::staticInitConfigMapModeZLevelEnabled()
{
    return "CCOMP_MMZLE";
}

QString CT_StandardColorCompositeDrawManager::staticInitConfigMapModeZLevelValue()
{
    return "CCOMP_MMZLV";
}


#endif
