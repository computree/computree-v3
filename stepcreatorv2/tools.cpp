#include "tools.h"

// static
QStringList Tools::ITEMTYPE;
void Tools::initItemTypes()
{

    Tools::ITEMTYPE.append("CT_Scene");
    Tools::ITEMTYPE.append("CT_PointCluster");
    Tools::ITEMTYPE.append("CT_PolyLine");

    Tools::ITEMTYPE.append("CT_ReferencePoint");
    Tools::ITEMTYPE.append("CT_AbstractShape");
    Tools::ITEMTYPE.append("CT_Circle");
    Tools::ITEMTYPE.append("CT_Ellipse");
    Tools::ITEMTYPE.append("CT_Line");
    Tools::ITEMTYPE.append("CT_Cylinder");
    Tools::ITEMTYPE.append("CT_Raster2DFloat");
    Tools::ITEMTYPE.append("CT_Raster2DInt");
    Tools::ITEMTYPE.append("CT_RegularGridFloat");
    Tools::ITEMTYPE.append("CT_RegularGridInt");
    Tools::ITEMTYPE.append("CT_RegularGridDouble");
    Tools::ITEMTYPE.append("CT_RegularGridBool");
    Tools::ITEMTYPE.append("CT_Ray");
    Tools::ITEMTYPE.append("CT_Scanner");
    Tools::ITEMTYPE.append("CT_Triangulation2D");

    Tools::ITEMTYPE.append("CT_AbstractItemDrawableWithPointCloud");
    Tools::ITEMTYPE.append("CT_AbstractItemDrawableWithoutPointCloud");
}


// static
QString Tools::getQStringListConcat(QSet<QString> &list)
{
    QStringList list2(list.toList());
    list2.sort();

    QString result = "";

    foreach (const QString &value, list2)
    {
        result += value;
        result += "\n";
    }
    return result;
}
