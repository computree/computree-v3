#ifndef PB_GDALEXPORTER_H
#define PB_GDALEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

#include "ct_itemdrawable/ct_box2d.h"
#include "ct_itemdrawable/ct_circle2d.h"
#include "ct_itemdrawable/ct_line2d.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include "ct_itemdrawable/ct_polyline2d.h"
#include "ct_itemdrawable/abstract/ct_abstractimage2d.h"

#ifdef USE_GDAL
#include "gdal_priv.h"

class OGRFeature;
#endif

/**
 * @brief Export ItemDrawable to files that gdal can create
 */
class PB_GDALExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_GDALExporter();

    #ifdef USE_GDAL
    PB_GDALExporter(const GDALDriver *driver);
    #endif

    void init();

    QString getExporterCustomName() const;

    virtual QString getExporterName() const;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();

#ifdef USE_GDAL
private:
    GDALDriver                                      *m_driver;
    bool                                            m_exportRaster;

    bool exportRaster(const QString &filepath);

    bool exportVector(const QString &filepath);

    bool exportItemDrawable(CT_AbstractItemDrawable *item, GDALDataset *dataset, OGRLayer *layer);

    bool exportBox2D(const CT_Box2D *box, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportCircle2D(const CT_Circle2D *circle, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportLine2D(const CT_Line2D *line, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPoint2D(const CT_Point2D *point, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPolygon2D(const CT_Polygon2D *polygon, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPolyline2D(const CT_Polyline2D *polyline, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);

    bool exportOGRGeometry(OGRGeometry *geo, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
#endif
};

#endif // PB_GDALEXPORTER_H
