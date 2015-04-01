#include "pb_gdalexporter.h"

#ifdef USE_GDAL
#include "gdal.h"
#include "ogrsf_frmts.h"
#endif

#include "ct_tools/ct_gdaltools.h"

#include <QFileInfo>

// TODO : choose raster type (what is R, G or B, etc...) and export it in multiple bands
// TODO : Group raster by dimension and resolution

PB_GDALExporter::PB_GDALExporter() : CT_AbstractExporter()
{
}

#ifdef USE_GDAL
PB_GDALExporter::PB_GDALExporter(const GDALDriver *driver) : CT_AbstractExporter()
{
    m_driver = (GDALDriver*)driver;

    setCanExportItems(true);
}
#endif

void PB_GDALExporter::init()
{
    #ifdef USE_GDAL
    if(m_driver != NULL) {
        QStringList ext = CT_GdalTools::staticGdalDriverExtension(m_driver);

        if(ext.isEmpty())
            ext.append("gdal");

        addNewExportFormat(FileFormat(ext, CT_GdalTools::staticGdalDriverName((m_driver))));
    }
    #endif
}

QString PB_GDALExporter::getExporterCustomName() const
{
    #ifdef USE_GDAL
    if(m_driver != NULL) {
        return CT_GdalTools::staticGdalDriverName(m_driver);
    }
    #endif

    return CT_AbstractExporter::getExporterCustomName();
}

bool PB_GDALExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    clearErrorMessage();

    m_exportRaster = false;

    QList<CT_AbstractItemDrawable*> myVectorList;
    QList<CT_AbstractItemDrawable*> myRasterList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_AbstractShape2D*>(item) != NULL)
            myVectorList.append(item);
        else if(dynamic_cast<CT_AbstractGrid2D*>(item) != NULL)
            myRasterList.append(item);
    }

    if(myVectorList.isEmpty() && myRasterList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable exportable"));
        return false;
    }
    else if(!myVectorList.isEmpty() && !myRasterList.isEmpty())
    {
        setErrorMessage(tr("Vous ne pouvez pas exporter des rasters ET des éléments 2D dans un seul et même fichier, veuillez recommencer votre sélection."));
        return false;
    }

    if(!myVectorList.isEmpty())
        return CT_AbstractExporter::setItemDrawableToExport(myVectorList);
    else {
        m_exportRaster = true;
        return CT_AbstractExporter::setItemDrawableToExport(myRasterList);
    }
}

CT_AbstractExporter *PB_GDALExporter::copy() const
{
    #ifdef USE_GDAL
    return new PB_GDALExporter(m_driver);
    #else
    return new PB_GDALExporter();
    #endif
}

bool PB_GDALExporter::protectedExportToFile()
{
#ifdef USE_GDAL
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = exportFormats().first().suffixes().first();

    QString filePath = QString("%1/%2.%3").arg(path).arg(baseName).arg(suffix);

    if(m_exportRaster)
        return exportRaster(filePath);

    return exportVector(filePath);
#endif

    return false;
}

#ifdef USE_GDAL
bool PB_GDALExporter::exportRaster(const QString &filepath)
{
    CT_AbstractGrid2D *grid = dynamic_cast<CT_AbstractGrid2D*>(itemDrawableToExport().first());

    size_t nXSize = grid->colDim();
    size_t nYSize = grid->linDim();

    GDALDataset *dataset = m_driver->Create(filepath.toLatin1(), nXSize, nYSize, 1, GDT_Float64, NULL);

    if( dataset == NULL )
    {
        setErrorMessage(tr("Creation of output file failed."));
        return false;
    }

    if( dataset->GetRasterBand(1) == NULL )
    {
        GDALClose(dataset);
        setErrorMessage(tr("Creation of output file failed."));
        return false;
    }

    GDALRasterBand *poBand = dataset->GetRasterBand(1);

    double padfTransform[6] = { 0, 0, 0, 0, 0, 0 };

    padfTransform[0] = grid->minX();
    padfTransform[1] = grid->resolution();
    padfTransform[3] = grid->minY();

    dataset->SetGeoTransform( padfTransform );

    double *pafScanline = (double *) CPLMalloc(sizeof(double)*nXSize);

    size_t index = 0;

    for(int y=0; y<nYSize; ++y) {

        for(int x=0; x<nXSize; ++x) {
            grid->index(x, nYSize - y - 1, index);
            pafScanline[x] = grid->valueAtIndexAsDouble(index);
        }

        poBand->RasterIO( GF_Write, 0, y, nXSize, 1, pafScanline, nXSize, 1, GDT_Float64, 0, 0 );
    }

    CPLFree(pafScanline);

    GDALClose(dataset);

    return true;
}

bool PB_GDALExporter::exportVector(const QString &filepath)
{
    GDALDataset *dataset = m_driver->Create(filepath.toLatin1(), 0, 0, 0, GDT_Unknown, NULL);

    if( dataset == NULL )
    {
        setErrorMessage(tr("Creation of output file failed."));
        return false;
    }

    OGRLayer *layer = dataset->CreateLayer("layer", NULL, wkbUnknown, NULL);

    if( layer == NULL )
    {
        GDALClose(dataset);
        setErrorMessage(tr("Layer creation failed."));
        return false;
    }

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

    while(it.hasNext())
        exportItemDrawable(it.next(), dataset, layer);

    GDALClose(dataset);

    return true;
}

bool PB_GDALExporter::exportItemDrawable(CT_AbstractItemDrawable *item, GDALDataset *dataset, OGRLayer *layer)
{
    OGRFeature *poFeature;
    poFeature = OGRFeature::CreateFeature( layer->GetLayerDefn() );

    CT_Box2D *box = dynamic_cast<CT_Box2D*>(item);

    if(box != NULL)
    {
        return exportBox2D(box, dataset, layer, poFeature);
    }
    else
    {
        CT_Circle2D *circle = dynamic_cast<CT_Circle2D*>(item);

        if(circle != NULL)
        {
            return exportCircle2D(circle, dataset, layer, poFeature);
        }
        else
        {
            CT_Line2D *line = dynamic_cast<CT_Line2D*>(item);

            if(line != NULL)
            {
                return exportLine2D(line, dataset, layer, poFeature);
            }
            else
            {
                CT_Point2D *point = dynamic_cast<CT_Point2D*>(item);

                if(point != NULL)
                {
                    return exportPoint2D(point, dataset, layer, poFeature);
                }
                else
                {
                    CT_Polygon2D *polyg = dynamic_cast<CT_Polygon2D*>(item);

                    if(polyg != NULL)
                    {
                        return exportPolygon2D(polyg, dataset, layer, poFeature);
                    }
                    else
                    {
                        CT_Polyline2D *polyl = dynamic_cast<CT_Polyline2D*>(item);

                        if(polyl != NULL)
                        {
                            return exportPolyline2D(polyl, dataset, layer, poFeature);
                        }
                        else
                        {
                            OGRFeature::DestroyFeature( poFeature );
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool PB_GDALExporter::exportBox2D(const CT_Box2D *box, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRPolygon po;

    Eigen::Vector3d min, max;
    box->getBoundingBox(min, max);

    OGRLinearRing extRing;
    extRing.addPoint(min[0], min[1], 0);
    extRing.addPoint(max[0], min[1], 0);
    extRing.addPoint(max[0], max[1], 0);
    extRing.addPoint(min[0], max[1], 0);

    po.addRing(&extRing);

    return exportOGRGeometry(&po, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportCircle2D(const CT_Circle2D *circle, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    Eigen::Vector2d c = circle->getCenter();
    c[0] -= circle->getRadius();

    Eigen::Vector2d c2 = circle->getCenter();
    c2[0] += circle->getRadius();

    OGRLineString *ogrl = OGRGeometryFactory::curveToLineString(c[0], c[1], 0, c2[0], c2[1], 0, c[0], c[1], 0, false, 0, NULL);

    bool r = exportOGRGeometry(ogrl, dataset, layer, poFeature);

    OGRGeometryFactory::destroyGeometry(ogrl);

    return r;
}

bool PB_GDALExporter::exportLine2D(const CT_Line2D *line, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRLineString ogrl;

    ogrl.addPoint(line->x1(), line->y1(), 0);
    ogrl.addPoint(line->x2(), line->y2(), 0);

    return exportOGRGeometry(&ogrl, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportPoint2D(const CT_Point2D *point, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRPoint ogrp;

    ogrp.setX(point->x());
    ogrp.setX(point->y());
    ogrp.setZ(0);

    return exportOGRGeometry(&ogrp, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportPolygon2D(const CT_Polygon2D *polygon, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRPolygon po;
    OGRLinearRing extRing;

    QVectorIterator<Eigen::Vector2d*> it(polygon->getVertices());

    while(it.hasNext())
    {
        Eigen::Vector2d *v = it.next();
        extRing.addPoint((*v)[0], (*v)[1], 0);
    }

    po.addRing(&extRing);

    return exportOGRGeometry(&po, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportPolyline2D(const CT_Polyline2D *polyline, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRLineString ogrl;

    QVectorIterator<Eigen::Vector2d*> it(polyline->getVertices());

    while(it.hasNext())
    {
        Eigen::Vector2d *v = it.next();
        ogrl.addPoint((*v)[0], (*v)[1], 0);
    }

    return exportOGRGeometry(&ogrl, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportOGRGeometry(OGRGeometry *geo, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    Q_UNUSED(dataset)

    poFeature->SetGeometry( geo );

    if( layer->CreateFeature( poFeature ) != OGRERR_NONE )
    {
        OGRFeature::DestroyFeature( poFeature );
        setErrorMessage(tr("Failed to create feature in shapefile."));
        return false;
    }

    OGRFeature::DestroyFeature( poFeature );

    return true;
}
#endif
