#include "ct_standardgrid4ddrawmanager.h"

#include <QDebug>

/////////////////////////////////////////////////////////////////////
/// Template specialisation here ////////////////////////////////////
/////////////////////////////////////////////////////////////////////

template<>
void CT_StandardGrid4DDrawManager<bool>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Grid4D<bool> &item = dynamic_cast<const CT_Grid4D<bool>&>(itemDrawable);

    // Getting the configuration values
    GLenum  drawingMode;
    bool    wireMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_WIRE_MODE_ENABLED).toBool();
    bool    uselowThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LOW_THRESHOLDS_ENABLED).toBool();
    bool    usehighThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIGH_THRESHOLDS_ENABLED).toBool();
    double  lowThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LOW_THRESHOLDS_VALUE).toDouble();
    double  highThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIGH_THRESHOLDS_VALUE).toDouble();
    double  reductionCoef = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_REDUCTION_COEF).toDouble();
    bool    useTransparency = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_ENABLED).toBool();
    int     transparencyValue = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_VALUE).toInt();

    size_t     ww  = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_W_DISPLAY_VALUE).toInt();
    size_t     nXinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XINF).toInt();
    size_t     nXsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XSUP).toInt();
    size_t     nYinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YINF).toInt();
    size_t     nYsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YSUP).toInt();
    size_t     nZinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZINF).toInt();
    size_t     nZsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZSUP).toInt();

    if (nXsup > item.xdim()) {nXsup = item.xdim();}
    if (nYsup > item.ydim()) {nXsup = item.ydim();}
    if (nZsup > item.zdim()) {nXsup = item.zdim();}

    if (!useTransparency || (transparencyValue > 255)) {transparencyValue = 255;}
    if (transparencyValue < 0) {transparencyValue = 0;}

    if ( wireMode ) {drawingMode = GL_LINE;}
    else            {drawingMode = GL_FILL;}

    if (!uselowThresh) {lowThresh =  (double)item.dataMin();}
    if (!usehighThresh) {highThresh =  (double)item.dataMax();}

    bool drawAsMap = true;
    if (lowThresh == highThresh) {drawAsMap = false;}

    QColor color = painter.getColor();

    // For each voxel of the grid
    double scaling = 240.0 / (highThresh - lowThresh);
    double offset = - (240*lowThresh)/(highThresh - lowThresh);

    size_t xdim = item.xdim();
    size_t ydim = item.ydim();
    size_t zdim = item.zdim();
    float demiResx = reductionCoef*item.xres() / 2.0;
    float demiResy = reductionCoef*item.yres() / 2.0;
    float demiResz = reductionCoef*item.zres() / 2.0;

    float xmin, ymin, zmin, xmax, ymax, zmax;

    // If w val > wwMax, do not display
    if ( ww <= item.wdim() )
    {
        // For each voxel of the grid
        for (size_t xx = nXinf ; xx < (xdim - nXsup) ; xx++)
        {
            for (size_t yy = nYinf ; yy < (ydim - nYsup)  ; yy++)
            {
                for (size_t zz = nZinf ; zz < (zdim - nZsup); zz++)
                {
                    size_t index;
                    if (item.index(ww, xx, yy, zz, index))
                    {
                        bool data = item.valueAtIndex(index);

                        // Draw a cube if the value it contains is between the two thresholds
                        if ( data >= lowThresh && data <= highThresh )
                        {

                            if (drawAsMap && !itemDrawable.isSelected())
                            {
                                double h = qRound((data*scaling) + offset);
                                painter.setColor( QColor::fromHsv(h,255,255,transparencyValue) );
                            } else {
                                painter.setColor(QColor(255,255,255));
                            }

                            xmin = item.getCellCenterX(xx) - demiResx;
                            ymin = item.getCellCenterY(yy) - demiResy;
                            zmin = item.getCellCenterZ(zz) - demiResz;
                            xmax = item.getCellCenterX(xx) + demiResx;
                            ymax = item.getCellCenterY(yy) + demiResy;
                            zmax = item.getCellCenterZ(zz) + demiResz;

                            painter.drawCube( xmin, ymin, zmin, xmax, ymax, zmax, GL_FRONT_AND_BACK, drawingMode );
                        }
                    }

                    else
                    {
                        qDebug() << "Probleme d'index (drawmanager)";
                    }
                }
            }
        }
    }

    painter.setColor(color);
}

template<>
CT_ItemDrawableConfiguration CT_StandardGrid4DDrawManager<bool>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addNewConfiguration(staticInitConfigWDisplayValue(), "W fixe", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigWireModeEnabled(), "Mode filaire", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigLowThresholdsEnabled(), "Forcer limite basse", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfigLowThresholdValue(), "Limite basse (forcée)", CT_ItemDrawableConfiguration::Double, 1 );
    item.addNewConfiguration(staticInitConfigHighThresholdsEnabled(), "Forcer limite haute", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigHighThresholdValue(), "Limite haute (forcée)", CT_ItemDrawableConfiguration::Double, 100 );
    item.addNewConfiguration(staticInitConfigReductionCoef(), "Coef. de reduction", CT_ItemDrawableConfiguration::Double, 1);
    item.addNewConfiguration(staticInitConfigEnableTransparency(), "Activer transparence", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigTransparencyValue(), "Valeur de transparence", CT_ItemDrawableConfiguration::Int, 100);
    item.addNewConfiguration(staticInitConfigXinf(), "Nb. Plans masqués X-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigXsup(), "Nb. Plans masqués X+", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYinf(), "Nb. Plans masqués Y-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYsup(), "Nb. Plans masqués Y+", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZinf(), "Nb. Plans masqués Z-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZsup(), "Nb. Plans masqués Z+", CT_ItemDrawableConfiguration::Int, 0);

    return item;
}
