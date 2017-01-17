#include "pb_gdalexporter.h"

#ifdef USE_GDAL
#include "gdal.h"
#include "ogrsf_frmts.h"
#endif

#include "ct_tools/ct_gdaltools.h"

#include <QFileInfo>
#include <QDebug>

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

        QString driverType = getTypeOfDriver();

        QString toolTip = tr("Exporter GDAL de type : %1").arg(driverType.isEmpty()?tr("Inconnu"):driverType);
        if (driverType == "Raster")
        {
            toolTip.append("<br>");
            toolTip.append("<br>");
            toolTip.append(tr("Pour plus de détails voir : http://www.gdal.org/formats_list.html"));
        } else if (driverType == "Vector")
        {
            toolTip.append("<br>");
            toolTip.append("<br>");
            toolTip.append(tr("Pour plus de détails voir : http://www.gdal.org/ogr_formats.html"));
        }
        toolTip.append("<br>");
        toolTip.append("<br>");
        toolTip.append(tr("Extension : "));
        for (int i = 0 ; i < ext.size() ; i++)
        {
            toolTip.append("*.");
            toolTip.append(ext.at(i));
            if ((i + 1) < ext.size()) {toolTip.append(" ");}
        }
        setToolTip(toolTip);

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

QString PB_GDALExporter::getExporterName() const
{
#ifdef USE_GDAL
    if(m_driver != NULL) {
        return CT_GdalTools::staticGdalDriverName(m_driver);
    }
#endif
    return CT_AbstractExporter::getExporterName();
}

CT_StepsMenu::LevelPredefined PB_GDALExporter::getExporterSubMenuName() const
{
    QString driverType = getTypeOfDriver();
    if (driverType == "Raster") {return CT_StepsMenu::LP_Raster;}
    if (driverType == "Vector") {return CT_StepsMenu::LP_Vector;}
    return CT_StepsMenu::LP_Others;
}

QString PB_GDALExporter::getTypeOfDriver() const
{
#ifdef USE_GDAL
    if (m_driver->GetMetadataItem(GDAL_DCAP_RASTER) != NULL && m_driver->GetMetadataItem(GDAL_DCAP_VECTOR) == NULL) {return "Raster";}
    if (m_driver->GetMetadataItem(GDAL_DCAP_VECTOR) != NULL && m_driver->GetMetadataItem(GDAL_DCAP_RASTER) == NULL) {return "Vector";}
#endif

    return "";
}


bool PB_GDALExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    clearErrorMessage();

#ifdef USE_GDAL
    m_exportRaster = false;

    QList<CT_AbstractItemDrawable*> myVectorList;
    QList<CT_AbstractItemDrawable*> myRasterList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        CT_AbstractShape2D* shape2d = dynamic_cast<CT_AbstractShape2D*>(item);
        if(shape2d != NULL)
        {
            myVectorList.append(item);

            QList<CT_AbstractItemAttribute *> attributes = shape2d->itemAttributes();
            for (int i = 0 ; i < attributes.size() ; i++)
            {
                CT_AbstractItemAttribute* att = attributes.at(i);

                CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) shape2d->model();
                CT_OutAbstractItemAttributeModel *attrModel = att->model();

                //QString itemDN = itemModel->displayableName();
                QString itemUN = itemModel->uniqueName();

                QString attrDN = attrModel->displayableName();
                QString attrUN = attrModel->uniqueName();

                if (attrModel->isADefaultItemAttributeModel() && attrModel->originalModel() != NULL) {attrUN = attrModel->originalModel()->uniqueName();}

                QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);

                if (!_modelsKeys.contains(key))
                {
                    _modelsKeys.append(key);
                    _names.insert(key, attrDN);

                    CT_AbstractCategory::ValueType type = attrModel->itemAttribute()->type();

                    if      (type == CT_AbstractCategory::BOOLEAN) {_ogrTypes.insert(key, OFTInteger);}
                    else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
                    else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
                    else if (type == CT_AbstractCategory::INT8)    {_ogrTypes.insert(key, OFTInteger);}
                    else if (type == CT_AbstractCategory::UINT8)   {_ogrTypes.insert(key, OFTInteger);}
                    else if (type == CT_AbstractCategory::INT16)   {_ogrTypes.insert(key, OFTInteger);}
                    else if (type == CT_AbstractCategory::UINT16)  {_ogrTypes.insert(key, OFTInteger);}
                    else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger);}
                    //                else if (type == CT_AbstractCategory::UINT32)  {_ogrTypes.insert(key, OFTInteger64);}
                    //                else if (type == CT_AbstractCategory::INT64)   {_ogrTypes.insert(key, OFTInteger64);}
                    //                else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger64);}
                    else                                           {_ogrTypes.insert(key, OFTReal);}
                }
            }
        }
        else if(dynamic_cast<CT_AbstractImage2D*>(item) != NULL)
        {
            myRasterList.append(item);
        }
    }

    if (!_names.isEmpty())
    {
        replaceBadCharacters(_names);
        qSort(_modelsKeys.begin(), _modelsKeys.end());
        _shortNames = computeShortNames(_names);


        QFileInfo exportPathInfo = QFileInfo(exportFilePath());

        QFile ffields(QString("%1/fields_names.txt").arg(exportPathInfo.absolutePath()));
        QTextStream fstream(&ffields);
        if (ffields.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMapIterator<QString, QString> itF(_shortNames);
            while (itF.hasNext())
            {
                itF.next();
                QString key = itF.key();
                QString shortName = itF.value();
                QString longName = _names.value(key);
                fstream << shortName << "\t";
                fstream << longName << "\n";
            }
            ffields.close();
        }

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
#endif
    return false;
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
    CT_AbstractImage2D *grid = dynamic_cast<CT_AbstractImage2D*>(itemDrawableToExport().first());

    size_t nXSize = grid->colDim();
    size_t nYSize = grid->linDim();

    GDALDataset *dataset = m_driver->Create(filepath.toLatin1(), nXSize, nYSize, 1, GDT_Float32, NULL);

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
    padfTransform[5] = -grid->resolution();
    padfTransform[3] = grid->minY() + nYSize*padfTransform[1];

    dataset->SetGeoTransform( padfTransform );

    poBand->SetNoDataValue(grid->NAAsDouble());


    float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

    size_t index = 0;

    for(int y = 0 ;  y < nYSize ; ++y)
    {
        for(int x = 0 ; x < nXSize ; ++x)
        {
            grid->index(x, y, index);
            pafScanline[x] = grid->valueAtIndexAsDouble(index);
        }
        poBand->RasterIO( GF_Write, 0, y, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0);
    }

    CPLFree(pafScanline);

    double min, max, mean, stdDev;
    poBand->FlushCache();
    poBand->ComputeStatistics(0, &min, &max, &mean, &stdDev, NULL, NULL);
    poBand->SetStatistics(min, max, mean, stdDev);
    poBand->FlushCache();

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

    for (int i = 0 ; layer != NULL && i < _modelsKeys.size() ; i++)
    {
        QString key = _modelsKeys.at(i);
        if (_ogrTypes.contains(key))
        {
            OGRFieldType ogrType = _ogrTypes.value(key);

            QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
            const char* fieldName = fieldNameBA;

            OGRFieldDefn oField(fieldName, ogrType );

            if (layer->CreateField( &oField ) != OGRERR_NONE)
            {
                //  erreur
            }
        }
    }


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

    CT_AbstractShape2D* shape2d = dynamic_cast<CT_AbstractShape2D*>(item);
    if(shape2d != NULL)
    {
        QList<CT_AbstractItemAttribute *> attributes = shape2d->itemAttributes();
        for (int i = 0 ; i < attributes.size() ; i++)
        {
            CT_AbstractItemAttribute* att = attributes.at(i);

            CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) shape2d->model();
            CT_OutAbstractItemAttributeModel *attrModel = att->model();

            QString itemUN = itemModel->uniqueName();
            QString attrUN = attrModel->uniqueName();

            if (attrModel->isADefaultItemAttributeModel() && attrModel->originalModel() != NULL) {attrUN = attrModel->originalModel()->uniqueName();}

            QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);


            QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
            const char* fieldName = fieldNameBA;

            if      (_ogrTypes.value(key) == OFTBinary)
            {
                poFeature->SetField(fieldName, att->toInt(shape2d, NULL));
            } else if (_ogrTypes.value(key) == OFTString)
            {
                //QString text = replaceAccentCharacters(att->toString(shape2d, NULL));
                QString text = att->toString(shape2d, NULL);
                QByteArray textBA = text.toLatin1();
                const char* textChar = textBA;
                poFeature->SetField(fieldName, textChar);
            } else if (_ogrTypes.value(key) == OFTInteger)
            {
                poFeature->SetField(fieldName, att->toInt(shape2d, NULL));
            } else
            {
                poFeature->SetField(fieldName, att->toDouble(shape2d, NULL));
            }
        }
    }


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

void PB_GDALExporter::replaceBadCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(replaceBadCharacters(it.value()));
    }
}

QString PB_GDALExporter::replaceBadCharacters(const QString &name) const
{
    QString value = name;
    value.replace(QRegExp("[àáâãäå]"), "a");
    value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
    value.replace(QRegExp("[éèëê]"), "e");
    value.replace(QRegExp("[ÈÉÊË]"), "E");
    value.replace(QRegExp("[ìíîï]"), "i");
    value.replace(QRegExp("[ÌÍÎÏ]"), "I");
    value.replace(QRegExp("[òóôõöø]"), "o");
    value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
    value.replace(QRegExp("[ùúûü]"), "u");
    value.replace(QRegExp("[ÙÚÛÜ]"), "U");
    value.replace(QRegExp("[ñ]"), "n");
    value.replace(QRegExp("[Ñ]"), "N");
    value.replace(QRegExp("[ç]"), "c");
    value.replace(QRegExp("[Ç]"), "C");
    value.replace(QRegExp("[\\W]"), "_");
    return value;
}

QString PB_GDALExporter::replaceAccentCharacters(const QString &name) const
{
        QString value = name;
        value.replace(QRegExp("[àáâãäå]"), "a");
        value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
        value.replace(QRegExp("[éèëê]"), "e");
        value.replace(QRegExp("[ÈÉÊË]"), "E");
        value.replace(QRegExp("[ìíîï]"), "i");
        value.replace(QRegExp("[ÌÍÎÏ]"), "I");
        value.replace(QRegExp("[òóôõöø]"), "o");
        value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
        value.replace(QRegExp("[ùúûü]"), "u");
        value.replace(QRegExp("[ÙÚÛÜ]"), "U");
        value.replace(QRegExp("[ñ]"), "n");
        value.replace(QRegExp("[Ñ]"), "N");
        value.replace(QRegExp("[ç]"), "c");
        value.replace(QRegExp("[Ç]"), "C");
        return value;
}


QMap<QString, QString> PB_GDALExporter::computeShortNames(const QMap<QString, QString> &names) const
{
    QMap<QString, QString> shortNames;
    QList<QString> existing;

    QMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        QString key = it.key();
        QString value = it.value();

        if (value.size() <= 10)
        {
            shortNames.insert(key, value);
            existing.append(value.toLower());
        } else {
            QString newValue = value.left(10);
            int cpt = 2;
            while (existing.contains(newValue.toLower()))
            {
                QString number = QVariant(cpt++).toString();
                newValue = QString("%1%2").arg(value.left(10 - number.length())).arg(number);
            }
            shortNames.insert(key, newValue);
            existing.append(newValue.toLower());
        }
    }

    return shortNames;
}

#endif
