#ifndef CT_STANDARDGRID3D_SPARSEDRAWMANAGER_HPP
#define CT_STANDARDGRID3D_SPARSEDRAWMANAGER_HPP

#include "ct_itemdrawable/tools/drawmanager/ct_standardgrid3d_sparsedrawmanager.h"
#include "ct_itemdrawable/ct_grid3d.h"
#include <typeinfo>
#include <QObject>

#include "qdebug.h"

template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_WIRE_MODE_ENABLED = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigWireModeEnabled();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_LOW_THRESHOLDS_ENABLED = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigLowThresholdsEnabled();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIGH_THRESHOLDS_ENABLED = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigHighThresholdsEnabled();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_LOW_THRESHOLDS_VALUE = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigLowThresholdValue();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIGH_THRESHOLDS_VALUE = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigHighThresholdValue();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_REDUCTION_COEF = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigReductionCoef();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_USE_PREDEFINED_COLORS = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigUsePredefinedColors();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_TRANSPARENCY_VALUE = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigTransparencyValue();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_XINF = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigXinf();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_XSUP = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigXsup();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_YINF = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigYinf();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_YSUP = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigYsup();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_ZINF = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigZinf();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_HIDE_PLANE_NB_ZSUP = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigZsup();
template< typename DataT > const QString CT_StandardGrid3D_SparseDrawManager<DataT>::INDEX_CONFIG_SHOW_TRUES_ONLY = CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigShowTrueOnly();

template< typename DataT >
CT_StandardGrid3D_SparseDrawManager<DataT>::CT_StandardGrid3D_SparseDrawManager(QString drawConfigurationName)
    : CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager(drawConfigurationName.isEmpty() ? CT_Grid3D<DataT>::staticName() : drawConfigurationName)
{
    
}

template< typename DataT >
CT_StandardGrid3D_SparseDrawManager<DataT>::~CT_StandardGrid3D_SparseDrawManager()
{
}

template< typename DataT >
void CT_StandardGrid3D_SparseDrawManager<DataT>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Grid3D_Sparse<DataT> &item = dynamic_cast<const CT_Grid3D_Sparse<DataT>&>(itemDrawable);

    // Getting the configuration values
    GLenum  drawingMode;
    bool    wireMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_WIRE_MODE_ENABLED).toBool();
    bool    uselowThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LOW_THRESHOLDS_ENABLED).toBool();
    bool    usehighThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIGH_THRESHOLDS_ENABLED).toBool();
    double  lowThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LOW_THRESHOLDS_VALUE).toDouble();
    double  highThresh = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIGH_THRESHOLDS_VALUE).toDouble();
    double  reductionCoef = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_REDUCTION_COEF).toDouble();
    int     transparencyValue = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_VALUE).toInt();
    bool    usePredefinedColors = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_USE_PREDEFINED_COLORS).toBool();

    size_t     nXinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XINF).toInt();
    size_t     nXsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XSUP).toInt();
    size_t     nYinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YINF).toInt();
    size_t     nYsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YSUP).toInt();
    size_t     nZinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZINF).toInt();
    size_t     nZsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZSUP).toInt();

    if (nXsup > item.xdim()) {nXsup = item.xdim();}
    if (nYsup > item.ydim()) {nXsup = item.ydim();}
    if (nZsup > item.zdim()) {nXsup = item.zdim();}

    if (transparencyValue > 255) {transparencyValue = 255;}
    if (transparencyValue < 0) {transparencyValue = 0;}

    if ( wireMode ) {drawingMode = GL_LINE;}
    else            {drawingMode = GL_FILL;}

    if (!uselowThresh) {lowThresh =  (double)item.dataMin();}
    if (!usehighThresh) {highThresh =  (double)item.dataMax();}

    bool drawAsMap = true;
    if (lowThresh == highThresh) {drawAsMap = false;} // Si les limites sont égales : une seule couleur

    QColor color = painter.getColor();

    // For each voxel of the grid
    double scaling = 240.0 / (highThresh - lowThresh);
    double offset = - (240*lowThresh)/(highThresh - lowThresh);

    size_t xdim = item.xdim();
    size_t ydim = item.ydim();
    size_t zdim = item.zdim();
    double demiRes = reductionCoef*item.resolution() / 2.0;

    double xmin, ymin, zmin, xmax, ymax, zmax;

    // For each voxel of the grid
    for (size_t xx = nXinf ; xx < (xdim - nXsup) ; xx++)
    {
        for (size_t yy = nYinf ; yy < (ydim - nYsup)  ; yy++)
        {
            for (size_t zz = nZinf ; zz < (zdim - nZsup); zz++)
            {
                size_t index;
                if (item.index(xx, yy, zz, index))
                {
                    DataT data = item.valueAtIndex(index);

                    // Draw a cube if the value it contains is between the two thresholds
                    if ( data >= lowThresh && data <= highThresh )
                    {
                        bool predef = false;
                        if (usePredefinedColors && item.colorsDefined())
                        {
                            painter.setColor(item.getColorForValue(data));
                            predef = true;
                        }

                        if (!predef && drawAsMap && !itemDrawable.isSelected())
                        {
                            double h = (int) qRound((data*scaling) + offset);
                            painter.setColor( QColor::fromHsv(h,255,255,transparencyValue) );
                        } else if (!predef){
                            painter.setColor(QColor(255,255,255, transparencyValue));
                        }

                        xmin = item.getCellCenterX(xx) - demiRes;
                        ymin = item.getCellCenterY(yy) - demiRes;
                        zmin = item.getCellCenterZ(zz) - demiRes;
                        xmax = item.getCellCenterX(xx) + demiRes;
                        ymax = item.getCellCenterY(yy) + demiRes;
                        zmax = item.getCellCenterZ(zz) + demiRes;

                        painter.drawCube( xmin, ymin, zmin, xmax, ymax, zmax, GL_FRONT_AND_BACK, drawingMode );
                    }
                } else {
                    qDebug() << "Problème d'index (drawmanager)";
                }
            }
        }
    }

    painter.setColor(color);
}

template< typename DataT >
CT_ItemDrawableConfiguration CT_StandardGrid3D_SparseDrawManager<DataT>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addNewConfiguration(staticInitConfigWireModeEnabled(), QObject::tr("Mode filaire"), CT_ItemDrawableConfiguration::Bool, true);             // Draw the grid in wire mode
    item.addNewConfiguration(staticInitConfigLowThresholdsEnabled(), QObject::tr("Forcer limite basse"), CT_ItemDrawableConfiguration::Bool, true);     // Using thresholds or not
    item.addNewConfiguration(staticInitConfigLowThresholdValue(), QObject::tr("Limite basse (forcée)"), CT_ItemDrawableConfiguration::Double, 1 );         // Voxels with value lesser than this threshold will not be drawn
    item.addNewConfiguration(staticInitConfigHighThresholdsEnabled(), QObject::tr("Forcer limite haute"), CT_ItemDrawableConfiguration::Bool, false);     // Using thresholds or not
    item.addNewConfiguration(staticInitConfigHighThresholdValue(), QObject::tr("Limite haute (forcée)"), CT_ItemDrawableConfiguration::Double, 100 );        // Voxels with value greater than this threshold will not be drawn
    item.addNewConfiguration(staticInitConfigReductionCoef(), QObject::tr("Coef. de reduction"), CT_ItemDrawableConfiguration::Double, 1);
    item.addNewConfiguration(staticInitConfigUsePredefinedColors(), QObject::tr("Utiliser couleurs pré-définies"), CT_ItemDrawableConfiguration::Bool, true);             // Draw the grid with pre-defined colors
    item.addNewConfiguration(staticInitConfigTransparencyValue(), QObject::tr("Valeur de transparence"), CT_ItemDrawableConfiguration::Int, 100);
    item.addNewConfiguration(staticInitConfigXinf(), QObject::tr("Nb. Plans masqués X-"), CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigXsup(), QObject::tr("Nb. Plans masqués X+"), CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYinf(), QObject::tr("Nb. Plans masqués Y-"), CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYsup(), QObject::tr("Nb. Plans masqués Y+"), CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZinf(), QObject::tr("Nb. Plans masqués Z-"), CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZsup(), QObject::tr("Nb. Plans masqués Z+"), CT_ItemDrawableConfiguration::Int, 0);

    return item;
}

// PROTECTED //

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigWireModeEnabled()
{
    return "A3DSPGD_WME";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigLowThresholdsEnabled()
{
    return "A3DSPGD_LTE";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigHighThresholdsEnabled()
{
    return "A3DSPGD_HTE";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigLowThresholdValue()
{
    return "A3DSPGD_LTV";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigHighThresholdValue()
{
    return "A3DSPGD_HTV";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigReductionCoef()
{
    return "A3DSPGD_RDC";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigUsePredefinedColors()
{
    return "A3DSPGD_PDC";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigTransparencyValue()
{
    return "A3DSPGD_TRV";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigXinf()
{
    return "A3DSPGD_XIN";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigXsup()
{
    return "A3DSPGD_XSU";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigYinf()
{
    return "A3DSPGD_YIN";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigYsup()
{
    return "A3DSPGD_YSU";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigZinf()
{
    return "A3DSPGD_ZIN";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigZsup()
{
    return "A3DSPGD_ZSU";
}

template< typename DataT >
QString CT_StandardGrid3D_SparseDrawManager<DataT>::staticInitConfigShowTrueOnly()
{
    return "A3DSPGD_STO";
}
#endif // CT_STANDARDGRID3D_SPARSEDRAWMANAGER_HPP
