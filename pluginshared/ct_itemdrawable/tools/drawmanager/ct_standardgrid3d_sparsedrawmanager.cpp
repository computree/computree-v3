#include "ct_standardgrid3d_sparsedrawmanager.h"

/////////////////////////////////////////////////////////////////////
/// Specialisations for bool type ///////////////////////////////////
/////////////////////////////////////////////////////////////////////

template<>
void CT_StandardGrid3D_SparseDrawManager<bool>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::draw(view, painter, itemDrawable);

    const CT_Grid3D_Sparse<bool> &item = dynamic_cast<const CT_Grid3D_Sparse<bool>&>(itemDrawable);

    // Getting the configuration values
    GLenum  drawingMode;
    bool    wireMode = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_WIRE_MODE_ENABLED).toBool();
    double  reductionCoef = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_REDUCTION_COEF).toDouble();
    int     transparencyValue = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_TRANSPARENCY_VALUE).toInt();
    bool    showTrueOnly = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_SHOW_TRUES_ONLY).toBool();

    size_t     nXinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XINF).toUInt();
    size_t     nXsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_XSUP).toUInt();
    size_t     nYinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YINF).toUInt();
    size_t     nYsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_YSUP).toUInt();
    size_t     nZinf = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZINF).toUInt();
    size_t     nZsup = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_HIDE_PLANE_NB_ZSUP).toUInt();

    size_t xinf = nXinf;
    size_t yinf = nYinf;
    size_t zinf = nZinf;
    size_t xsup = 0;
    size_t ysup = 0;
    size_t zsup = 0;

    if (nXsup < item.xdim()) {xsup = item.xdim() - nXsup - 1;}
    if (nYsup < item.ydim()) {ysup = item.ydim() - nYsup - 1;}
    if (nZsup < item.zdim()) {zsup = item.zdim() - nZsup - 1;}

    if (transparencyValue > 255) {transparencyValue = 255;}
    if (transparencyValue < 0) {transparencyValue = 0;}

    if ( wireMode ) {drawingMode = GL_LINE;}
    else            {drawingMode = GL_FILL;}

    bool drawAsMap = true;

    QColor color = painter.getColor();

    double demiRes = reductionCoef*item.resolution() / 2.0;

    double xmin, ymin, zmin, xmax, ymax, zmax;
    size_t xx, yy, zz;

    // For each voxel of the grid   
    QList<size_t> indices;
    item.getIndicesWithData(indices);

    for (int i = 0 ; i < indices.size() ; i++)
    {
        size_t index = indices.at(i);

        item.indexToGrid(index, xx, yy, zz);

        if( xx >= xinf && xx <= xsup && yy >= yinf && yy <= ysup && zz >= zinf && zz <= zsup )
        {
            bool data = item.valueAtIndex(index);

            if (data || !showTrueOnly)
            {
                if (drawAsMap && !itemDrawable.isSelected())
                {
                    if (data)
                    {
                        painter.setColor(QColor(0,255,0, transparencyValue));
                    } else {
                        painter.setColor(QColor(255,0,0, transparencyValue));
                    }

                    xmin = item.getCellCenterX(xx) - demiRes;
                    ymin = item.getCellCenterY(yy) - demiRes;
                    zmin = item.getCellCenterZ(zz) - demiRes;
                    xmax = item.getCellCenterX(xx) + demiRes;
                    ymax = item.getCellCenterY(yy) + demiRes;
                    zmax = item.getCellCenterZ(zz) + demiRes;

                    painter.drawCube( xmin, ymin, zmin, xmax, ymax, zmax, GL_FRONT_AND_BACK, drawingMode );
                }
            }
        }
    }

    painter.setColor(color);
}

template<>
CT_ItemDrawableConfiguration CT_StandardGrid3D_SparseDrawManager<bool>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addNewConfiguration(staticInitConfigWireModeEnabled(), "Mode filaire", CT_ItemDrawableConfiguration::Bool, true);             // Draw the grid in wire mode
    item.addNewConfiguration(staticInitConfigReductionCoef(), "Coef. de reduction", CT_ItemDrawableConfiguration::Double, 1);
    item.addNewConfiguration(staticInitConfigShowTrueOnly(), "Valeurs TRUE seulement", CT_ItemDrawableConfiguration::Bool, 1);
    item.addNewConfiguration(staticInitConfigTransparencyValue(), "Valeur de transparence", CT_ItemDrawableConfiguration::Int, 100);
    item.addNewConfiguration(staticInitConfigXinf(), "Nb. Plans masqués X-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigXsup(), "Nb. Plans masqués X+", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYinf(), "Nb. Plans masqués Y-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigYsup(), "Nb. Plans masqués Y+", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZinf(), "Nb. Plans masqués Z-", CT_ItemDrawableConfiguration::Int, 0);
    item.addNewConfiguration(staticInitConfigZsup(), "Nb. Plans masqués Z+", CT_ItemDrawableConfiguration::Int, 0);

    return item;
}

