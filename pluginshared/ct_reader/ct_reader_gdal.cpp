#include "ct_reader_gdal.h"

#ifdef USE_GDAL
#include "gdal.h"
#include "ogrsf_frmts.h"
#endif

#include "ct_itemdrawable/ct_image2d.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include "ct_itemdrawable/ct_polyline2d.h"
#include "ct_tools/ct_gdaltools.h"

CT_Reader_GDAL::CT_Reader_GDAL()
{
}

#ifdef USE_GDAL
CT_Reader_GDAL::CT_Reader_GDAL(const GDALDriver *driver)
{
    m_driver = (GDALDriver*)driver;
}
#endif

CT_Reader_GDAL::~CT_Reader_GDAL()
{
}

bool CT_Reader_GDAL::setFilePath(const QString &filepath)
{        
    m_filePath = filepath;

    #ifdef USE_GDAL
    if(m_driver != NULL) {
        if(canBeOpened(filepath))
        {
            m_header = new CT_FileHeader(NULL, NULL);
            m_header->setFile(m_filePath);
            return true;
        }
    }
    #endif

    return false;
}

CT_AbstractReader* CT_Reader_GDAL::copy() const
{
    #ifdef USE_GDAL
    return new CT_Reader_GDAL(m_driver);
    #else
    return new CT_Reader_GDAL();
    #endif
}

void CT_Reader_GDAL::protectedInit()
{
    #ifdef USE_GDAL
    if(m_driver != NULL) {
        QStringList ext = CT_GdalTools::staticGdalDriverExtension(m_driver);

        if(ext.isEmpty())
            ext.append("*");

        addNewReadableFormat(FileFormat(ext, CT_GdalTools::staticGdalDriverName(m_driver)));
    }
    #endif
}

#ifdef USE_GDAL
bool createGeometryModel(OGRGeometry *poGeometry,
                         int layerIndex,
                         QStringList &canNotBeLoaded,
                         CT_OutStdSingularItemModel **pointModel,
                         CT_OutStdSingularItemModel **polygoneModel,
                         CT_OutStdSingularItemModel **polylineModel)
{
    OGRwkbGeometryType geoT = wkbFlatten(poGeometry->getGeometryType());

    if((geoT == wkbPoint) || (geoT == wkbMultiPoint)) {

        if(*pointModel == NULL)
            *pointModel = new CT_OutStdSingularItemModel(QString(DEF_CT_Reader_GDAL_pointOut).arg(layerIndex), new CT_Point2D(), QObject::tr("Points"));

        return true;
    }
    else if((geoT == wkbPolygon) || (geoT == wkbMultiPolygon)) {

        if(*polygoneModel == NULL)
            *polygoneModel = new CT_OutStdSingularItemModel(QString(DEF_CT_Reader_GDAL_polygoneOut).arg(layerIndex), new CT_Polygon2D(), QObject::tr("Polygones"));

        return true;
    }
    else if((geoT == wkbLineString) || (geoT == wkbMultiLineString)) {

        if(*polylineModel == NULL)
            *polylineModel = new CT_OutStdSingularItemModel(QString(DEF_CT_Reader_GDAL_polylineOut).arg(layerIndex), new CT_Polyline2D(), QObject::tr("Polylignes"));

        return true;
    } else if(geoT == wkbGeometryCollection) {

        OGRGeometryCollection *multi = (OGRGeometryCollection*)poGeometry;

        int n = multi->getNumGeometries();

        for(int j=0; j<n; ++j) {

            OGRGeometry *poGeometry2 = multi->getGeometryRef(j);

            if(!createGeometryModel(poGeometry2,
                                layerIndex,
                                canNotBeLoaded,
                                pointModel,
                                polygoneModel,
                                polylineModel))
                return false;
        }

        return true;
    }

    QString geoName = poGeometry->getGeometryName();

    if(!canNotBeLoaded.contains(geoName))
        canNotBeLoaded.append(geoName);

    return false;
}
#endif

void CT_Reader_GDAL::protectedCreateOutItemDrawableModelList()
{
    #ifdef USE_GDAL
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();


    GDALDataset *data = getDataSet(filepath());

    if(data != NULL) {
        int n = data->GetRasterCount();

        if(n != 0) {
            for(int i=0; i<n; ++i) {
                QString name = GDALGetColorInterpretationName(data->GetRasterBand(i+1)->GetColorInterpretation());
                addOutItemDrawableModel(new CT_OutStdSingularItemModel(QString(DEF_CT_Reader_GDAL_rasterOut).arg(i), new CT_Image2D<float>(), (name.isEmpty() || (name == "Undefined")) ? QString("Raster %1").arg(i) : name));
            }
        } else {
            n = data->GetLayerCount();

            for(int i=0; i<n; ++i) {

                CT_OutStdGroupModel *layerModel = new CT_OutStdGroupModel(QString(DEF_CT_Reader_GDAL_layerOut).arg(i), new CT_StandardItemGroup(), tr("Layer %1").arg(i));

                CT_OutStdSingularItemModel *pointModel = NULL;
                CT_OutStdSingularItemModel *polygoneModel = NULL;
                CT_OutStdSingularItemModel *polylineModel = NULL;

                OGRLayer *poLayer = data->GetLayer(i);

                OGRFeature *poFeature;

                poLayer->ResetReading();

                QStringList canNotBeLoaded;

                while( ((poFeature = poLayer->GetNextFeature()) != NULL)
                       && ((pointModel == NULL)
                       || (polygoneModel == NULL)
                       || (polylineModel == NULL)))
                {
                    OGRGeometry *poGeometry = poFeature->GetGeometryRef();

                    if(poGeometry != NULL)
                        createGeometryModel(poGeometry, i, canNotBeLoaded, &pointModel, &polygoneModel, &polylineModel);

                    OGRFeature::DestroyFeature( poFeature );
                }

                if(pointModel != NULL) {
                    layerModel->addItem(pointModel);
                    m_models.insert(pointModel->uniqueName(), pointModel);
                }

                if(polygoneModel != NULL) {
                    layerModel->addItem(polygoneModel);
                    m_models.insert(polygoneModel->uniqueName(), polygoneModel);
                }

                if(polylineModel != NULL) {
                    layerModel->addItem(polylineModel);
                    m_models.insert(polylineModel->uniqueName(), polylineModel);
                }

                if(layerModel->isEmpty())
                    delete layerModel;
                else
                    addOutGroupModel(layerModel);

                foreach (QString st, canNotBeLoaded) {
                    PS_LOG->addDebugMessage(LogInterface::reader, tr("Impossible de charger des éléments du type : ") + st);
                }
            }
        }

        GDALClose(data);
    }
    #endif
}


#ifdef USE_GDAL
bool createPolyline(OGRLineString *line, CT_StandardItemGroup *group, int layerIndex, const QHash<QString, CT_OutStdSingularItemModel*> &models)
{
    int n = line->getNumPoints();

    if(n > 0) {

        QVector<Eigen::Vector2d *> vertices(n);

        OGRPoint point;

        for(int j=0; j<n; ++j) {
            line->getPoint(j, &point);
            vertices[j] = new Eigen::Vector2d(point.getX(), point.getY());
        }

        return group->addItemDrawable(new CT_Polyline2D(models.value(QString(DEF_CT_Reader_GDAL_polylineOut).arg(layerIndex)), NULL, new CT_Polyline2DData(vertices, false)));
    }

    return false;
}

bool createPolygon(OGRPolygon *polygon, CT_StandardItemGroup *group, int layerIndex, const QHash<QString, CT_OutStdSingularItemModel*> &models)
{
    const OGRLinearRing *ring = polygon->getExteriorRing();

    if(ring != NULL) {
        int n = ring->getNumPoints();

        QVector<Eigen::Vector2d *> vertices(n);

        OGRPoint point;

        for(int j=0; j<n; ++j) {
            ring->getPoint(j, &point);
            vertices[j] = new Eigen::Vector2d(point.getX(), point.getY());
        }

        return group->addItemDrawable(new CT_Polygon2D(models.value(QString(DEF_CT_Reader_GDAL_polygoneOut).arg(layerIndex)), NULL, new CT_Polygon2DData(vertices, false)));
    }

    return false;
}

bool createPoint(OGRPoint *point, CT_StandardItemGroup *group, int layerIndex, const QHash<QString, CT_OutStdSingularItemModel*> &models)
{
    return group->addItemDrawable(new CT_Point2D(models.value(QString(DEF_CT_Reader_GDAL_pointOut).arg(layerIndex)), NULL, new CT_Point2DData(point->getX(), point->getY())));
}
#endif

bool CT_Reader_GDAL::protectedReadFile()
{
    #ifdef USE_GDAL

    GDALDataset *data = getDataSet(filepath());

    if(data == NULL)
        return false;

    int count = data->GetRasterCount();

    if(count > 0) {
        for(int i=0; i<count; ++i) {

            GDALRasterBand *poBand = data->GetRasterBand(i+1);

            double padfTransform[6];
            poBand->GetDataset()->GetGeoTransform(&padfTransform[0]);

            int nXSize = poBand->GetXSize();
            int nYSize = poBand->GetYSize();

            double xMin = padfTransform[0];
            double yMin = padfTransform[3] - nYSize*padfTransform[1];


            double na = poBand->GetNoDataValue();

            CT_Image2D<float> *grid = new CT_Image2D<float>(NULL,
                                       NULL,
                                       xMin,
                                       yMin,
                                       nXSize,
                                       nYSize,
                                       padfTransform[1],
                                       0,
                                       na,
                                       na);

            float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

            size_t index = 0;

            for(int y=0; y<nYSize; ++y) {
                poBand->RasterIO( GF_Read, 0, y, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0 );

                for(int x=0; x<nXSize; ++x) {
                    grid->index(x, y, index);
                    grid->setValueAtIndex(index, pafScanline[x]);
                }
            }

            CPLFree(pafScanline);

            grid->computeMinMax();

            addOutItemDrawable(QString(DEF_CT_Reader_GDAL_rasterOut).arg(i), grid);
        }
    } else {
        count = data->GetLayerCount();

        for(int i=0; i<count; ++i) {

            OGRLayer *poLayer = data->GetLayer(i);

            OGRFeature *poFeature;

            poLayer->ResetReading();

            while( (poFeature = poLayer->GetNextFeature()) != NULL )
            {
                OGRGeometry *poGeometry = poFeature->GetGeometryRef();

                if(poGeometry != NULL)
                    convertGeometryToCT(poGeometry, i);

                OGRFeature::DestroyFeature( poFeature );
            }

        }
    }

    GDALClose(data);

    return true;
    #endif

    return false;
}

#ifdef USE_GDAL
bool CT_Reader_GDAL::canBeOpened(const QString &filepath) const
{
    GDALDataset *d = getDataSet(filepath);

    if(d == NULL)
        return false;

    GDALClose(d);
    return true;
}

GDALDataset* CT_Reader_GDAL::getDataSet(const QString &filepath) const
{
    GDALDataset *poDataset = (GDALDataset *) GDALOpenEx(filepath.toStdString().data(), GDAL_OF_RASTER | GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL);

    if((poDataset != NULL) && (poDataset->GetDriver() == m_driver))
        return poDataset;

    if(poDataset != NULL)
        GDALClose(poDataset);

    return NULL;
}

void CT_Reader_GDAL::convertGeometryToCT(OGRGeometry *poGeometry, int layerIndex)
{
    OGRwkbGeometryType geoT = wkbFlatten(poGeometry->getGeometryType());

    CT_StandardItemGroup *group = NULL;

    bool ok = false;

    if(geoT == wkbLineString) {
        ok = createPolyline((OGRLineString*)poGeometry, (group = new CT_StandardItemGroup(NULL, NULL)), layerIndex, m_models);
    } else if(geoT == wkbPolygon) {
        ok = createPolygon((OGRPolygon*)poGeometry, (group = new CT_StandardItemGroup(NULL, NULL)), layerIndex, m_models);
    } else if(geoT == wkbPoint) {
        ok = createPoint((OGRPoint*)poGeometry, (group = new CT_StandardItemGroup(NULL, NULL)), layerIndex, m_models);
    } else if((geoT == wkbGeometryCollection) || (geoT == wkbMultiLineString) || (geoT == wkbMultiPolygon) || (geoT == wkbMultiPoint)) {

        OGRGeometryCollection *multi = (OGRGeometryCollection*)poGeometry;

        int n = multi->getNumGeometries();

        for(int j=0; j<n; ++j)
            convertGeometryToCT(multi->getGeometryRef(j), layerIndex);
    }

    if(!ok)
        delete group;
    else
        addOutGroup(QString(DEF_CT_Reader_GDAL_layerOut).arg(layerIndex), group);
}
#endif
