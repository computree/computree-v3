#include "pb_stepexportattributesinloop.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_iterator/ct_groupiterator.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_itemdrawable/ct_plotlistingrid.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#ifdef USE_OPENCV
#include "ct_itemdrawable/ct_image2d.h"
#include "exporters/grid2d/pb_grid2dexporter.h"
#endif

#include <QFile>
#include <QTextStream>
#include <QDebug>

#define DEF_ESRI_ASCII_Grid "ESRI ASCII Grid"
#define DEF_ESRI_SHP "GDAL ESRI Shapefile"
#define DEF_NA -9999999

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grpMain "grpMain"
#define DEFin_plotListInGrid "plotListInGrid"
#define DEFin_grp "grp"

#define DEFin_itemWithAttribute "itemWithAttribute"
#define DEFin_attribute "attribute"

#define DEFin_itemWithXY "itemWithXY"
#define DEFin_Xattribute "Xattribute"
#define DEFin_Yattribute "Yattribute"
#define DEFin_attributeInItemXY "attributeInItemXY"

#define DEFout_res "res"
#define DEFout_grp "grp"


// Constructor : initialization of parameters
PB_StepExportAttributesInLoop::PB_StepExportAttributesInLoop(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _asciiExport = true;
    _vectorExport = false;
    _rasterExport = false;

    _vectorPrefix = "";
    _rasterPrefix = "";

    _rasterDriverName = DEF_ESRI_ASCII_Grid;
    _vectorDriverName = DEF_ESRI_SHP;


#ifdef USE_GDAL
    //GDALAllRegister();
    GDALDriverManager *driverManager = GetGDALDriverManager();

    int count = driverManager->GetDriverCount();

    for(int i = 0 ; i < count ; ++i)
    {
        GDALDriver *driver = driverManager->GetDriver(i);
        QString name = CT_GdalTools::staticGdalDriverName(driver);

        if(!name.isEmpty() && driver->GetMetadataItem(GDAL_DCAP_RASTER) != NULL && driver->GetMetadataItem(GDAL_DCAP_CREATE) != NULL) {
            _gdalRasterDrivers.insert(name, driver);
        }

        if(!name.isEmpty() && driver->GetMetadataItem(GDAL_DCAP_VECTOR) != NULL && driver->GetMetadataItem(GDAL_DCAP_CREATE) != NULL) {
            _gdalVectorDrivers.insert(name, driver);
        }
    }
#endif
}

PB_StepExportAttributesInLoop::~PB_StepExportAttributesInLoop()
{
}


// Step description (tooltip of contextual menu)
QString PB_StepExportAttributesInLoop::getStepDescription() const
{
    return tr("Export d'attributs dans une boucle");
}

// Step detailled description
QString PB_StepExportAttributesInLoop::getStepDetailledDescription() const
{
    return tr("");
}

// Step URL
QString PB_StepExportAttributesInLoop::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepExportAttributesInLoop::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepExportAttributesInLoop(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepExportAttributesInLoop::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn = createNewInResultModel(DEFin_res, tr("Résultat"));
    resIn->setZeroOrMoreRootGroup();
    resIn->addGroupModel("", DEFin_grpMain, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));

#ifdef USE_OPENCV
    if (_rasterExport)
    {
        resIn->addItemModel(DEFin_grpMain, DEFin_plotListInGrid, CT_PlotListInGrid::staticGetType(), tr("Grille de placettes"), "", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsObligatory);
    }
#endif

    resIn->addGroupModel(DEFin_grpMain, DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn->addItemModel(DEFin_grp, DEFin_itemWithXY, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item (avec XY)"), "", CT_InAbstractModel::C_ChooseOneIfMultiple);
    resIn->addItemAttributeModel(DEFin_itemWithXY, DEFin_Xattribute, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::DOUBLE, tr("X"), "", CT_InAbstractModel::C_ChooseOneIfMultiple);
    resIn->addItemAttributeModel(DEFin_itemWithXY, DEFin_Yattribute, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::DOUBLE, tr("Y"), "", CT_InAbstractModel::C_ChooseOneIfMultiple);
    resIn->addItemAttributeModel(DEFin_itemWithXY, DEFin_attributeInItemXY, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Attribut"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsOptional);

    resIn->addItemModel(DEFin_grp, DEFin_itemWithAttribute, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsOptional);
    resIn->addItemAttributeModel(DEFin_itemWithAttribute, DEFin_attribute, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Attribut"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsOptional);
}


// Creation and affiliation of OUT models
void PB_StepExportAttributesInLoop::createOutResultModelListProtected()
{
    //createNewOutResultModel(DEFout_res, tr("Resultat vide"));
}

void PB_StepExportAttributesInLoop::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = newStandardPreConfigurationDialog();
    configDialog->addBool(tr("Activer export ASCII tabulaire (1 fichier en tout)"), "", tr("Activer"), _asciiExport);

#ifdef USE_OPENCV
    configDialog->addEmpty();
    configDialog->addBool(tr("Activer export raster (1 fichier / tour / métrique)"), "", tr("Activer"), _rasterExport);
    configDialog->addTitle(tr("L'export raster nécessite une grille de placettes (désactiver si pas de résultat valide)"));
#endif

#ifdef USE_GDAL
    configDialog->addEmpty();
    configDialog->addBool(tr("Activer export vectoriel (1 fichier / tour)"), "", tr("Activer"), _vectorExport);
#endif

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExportAttributesInLoop::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = newStandardPostConfigurationDialog();

    if (_asciiExport)
    {
        configDialog->addTitle(tr("Export ASCII tabulaire (1 fichier en tout)"));
        configDialog->addFileChoice(tr("Choix du fichier"), CT_FileChoiceButton::OneNewFile, tr("Fichier texte (*.txt)"), _outASCIIFileName);
    }

#ifdef USE_OPENCV
    if (_rasterExport)
    {
        QStringList driversR;
        driversR.append(DEF_ESRI_ASCII_Grid);

#ifdef USE_GDAL
        driversR.append(_gdalRasterDrivers.keys());
#endif

        configDialog->addEmpty();
        configDialog->addTitle(tr("Export raster (1 fichier / tour / métrique)"));
        configDialog->addString(tr("Prefixe pour les fichiers exportés"), "", _rasterPrefix);
        configDialog->addStringChoice(tr("Choix du format d'export"), "", driversR, _rasterDriverName);
        configDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outRasterFolder);
    }
#endif

#ifdef USE_GDAL
    if (_vectorExport)
    {
        QStringList driversV;
        driversV.append(_gdalVectorDrivers.keys());

        configDialog->addEmpty();
        configDialog->addTitle(tr("Export vectoriel (1 fichier / tour)"));
        configDialog->addString(tr("Prefixe pour les fichiers exportés"), "", _vectorPrefix);
        configDialog->addStringChoice(tr("Choix du format d'export"), "", driversV, _vectorDriverName);
        configDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outVectorFolder);
    }
#endif
}

void PB_StepExportAttributesInLoop::compute()
{
    bool firstTurn = true;
    _modelsKeys.clear();
    _names.clear();

    QFile* fileASCII = NULL;
    QTextStream* streamASCII = NULL;

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn = inResultList.at(0);

    CT_ModelSearchHelper::SplitHash hash;
    QString xKey = "";
    QString yKey = "";

    CT_ModelSearchHelper::SplitHash hash1 = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_attribute, DEFin_itemWithAttribute, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ith1(hash1);
    while (ith1.hasNext())
    {
        ith1.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ith1.key()->originalModel()), ith1.value());
    }


    CT_ModelSearchHelper::SplitHash hash2 = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_attributeInItemXY, DEFin_itemWithXY, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ith2(hash2);
    while (ith2.hasNext())
    {
        ith2.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ith2.key()->originalModel()), ith2.value());
    }

    CT_ModelSearchHelper::SplitHash hashX = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_Xattribute, DEFin_itemWithXY, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ithX(hashX);
    while (ithX.hasNext())
    {
        ithX.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ithX.key()->originalModel()), ithX.value());

        CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) (ithX.key()->originalModel());
        CT_OutAbstractItemAttributeModel *attrModel = ithX.value();
        if (attrModel->isADefaultItemAttributeModel() && attrModel->originalModel() != NULL) {attrModel = (CT_OutAbstractItemAttributeModel*) (attrModel->originalModel());}
        xKey = QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrModel->uniqueName());
    }

    CT_ModelSearchHelper::SplitHash hashY = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_Yattribute, DEFin_itemWithXY, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ithY(hashY);
    while (ithY.hasNext())
    {
        ithY.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ithY.key()->originalModel()), ithY.value());

        CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) (ithY.key()->originalModel());
        CT_OutAbstractItemAttributeModel *attrModel = ithY.value();
        if (attrModel->isADefaultItemAttributeModel() && attrModel->originalModel() != NULL) {attrModel = (CT_OutAbstractItemAttributeModel*) (attrModel->originalModel());}
        yKey = QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrModel->uniqueName());
    }

    if (firstTurn)
    {
        QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> itModels(hash);
        while (itModels.hasNext())
        {
            itModels.next();

            CT_OutAbstractSingularItemModel  *itemModel = itModels.key();
            CT_OutAbstractItemAttributeModel *attrModel = itModels.value();

            QString itemDN = itemModel->displayableName();
            QString itemUN = itemModel->uniqueName();

            QString attrDN = attrModel->displayableName();
            QString attrUN = attrModel->uniqueName();

            if (attrModel->isADefaultItemAttributeModel() && attrModel->originalModel() != NULL) {attrUN = attrModel->originalModel()->uniqueName();}

            QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);
            _modelsKeys.append(key);

            _names.insert(key, QString("%2_%1").arg(itemDN).arg(attrDN));

#ifdef USE_GDAL
            if (_vectorExport && _outVectorFolder.size() > 0)
            {
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
#endif
        }
        replaceBadCharacters(_names);
        qSort(_modelsKeys.begin(), _modelsKeys.end());

        if (_vectorExport && _outVectorFolder.size() > 0)
        {
            _shortNames = computeShortNames(_names);

            QFile ffields(QString("%1/fields_names.txt").arg(_outVectorFolder.first()));
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

        if (_asciiExport && _outASCIIFileName.size() > 0)
        {
            fileASCII = new QFile(_outASCIIFileName.first());
            streamASCII = new QTextStream(fileASCII);

            if (fileASCII->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                for (int i = 0 ; i < _modelsKeys.size() ; i++)
                {
                    (*streamASCII) << _names.value(_modelsKeys.at(i));
                    if (i < _modelsKeys.size() - 1) {(*streamASCII) << "\t";} else {(*streamASCII) << "\n";}
                }

            } else {
                delete streamASCII; streamASCII = NULL;
                delete fileASCII; fileASCII = NULL;
                PS_LOG->addMessage(LogInterface::error, LogInterface::step, getStepCustomName() + tr("Impossible de créer le fichier d'export ASCII. Arrêt des traitements."));
                stop();
                return;
            }
        }
    }

#ifdef USE_GDAL
        GDALDataset *vectorDataSet;
        OGRLayer *vectorLayer;

        GDALDriver* driverVector = _gdalVectorDrivers.value(_vectorDriverName, NULL);

        if (_vectorExport && driverVector != NULL && _outVectorFolder.size() > 0)
        {
            vectorDataSet = driverVector->Create((QString("%1/point_out.shp").arg(_outVectorFolder.first())).toLatin1(), 0, 0, 0, GDT_Unknown, NULL );

            if (vectorDataSet != NULL)
            {
                vectorLayer = vectorDataSet->CreateLayer( "point_out", NULL, wkbPoint, NULL );
            }

            for (int i = 0 ; vectorLayer != NULL && i < _modelsKeys.size() ; i++)
            {
                QString key = _modelsKeys.at(i);
                if (_ogrTypes.contains(key))
                {
                    OGRFieldType ogrType = _ogrTypes.value(key);

                    QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
                    const char* fieldName = fieldNameBA;

                    OGRFieldDefn oField(fieldName, ogrType );

                    if (vectorLayer->CreateField( &oField ) != OGRERR_NONE)
                    {
                        //  erreur
                    }
                }
            }
        }
#endif

    // IN results browsing
    CT_ResultGroupIterator itIn_grpMain(resIn, this, DEFin_grpMain);
    while (itIn_grpMain.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpMain = (CT_AbstractItemGroup*) itIn_grpMain.next();

#ifdef USE_OPENCV
        QMap<QString, CT_Image2D<double>*> rasters;
        if (_rasterExport)
        {
            CT_PlotListInGrid* plotListInGrid = (CT_PlotListInGrid*) grpMain->firstItemByINModelName(this, DEFin_plotListInGrid);

            Eigen::Vector2d min, max;
            plotListInGrid->getBoundingBox2D(min, max);
            double resolution = plotListInGrid->getSpacing();

            if (plotListInGrid != NULL)
            {
                for (int i = 0 ; i < _modelsKeys.size() ; i++)
                {
                    QString key = _modelsKeys.at(i);

                    if (key != xKey && key != yKey)
                    {
                        rasters.insert(key, CT_Image2D<double>::createImage2DFromXYCoords(NULL, NULL, min(0), min(1), max(0), max(1), resolution, 0, DEF_NA, DEF_NA));
                    }
                }
            }
        }
#endif

        CT_GroupIterator itIn_grp(grpMain, this, DEFin_grp);
        while (itIn_grp.hasNext() && !isStopped())
        {
            const CT_AbstractItemGroup* grp = (CT_AbstractItemGroup*) itIn_grp.next();

            QMap<QString, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*> > indexedAttributes;

            double x = std::numeric_limits<double>::max();
            double y = std::numeric_limits<double>::max();

            CT_AbstractSingularItemDrawable* itemXY = (CT_AbstractSingularItemDrawable*) grp->firstItemByINModelName(this, DEFin_itemWithXY);
            if (itemXY != NULL)
            {
                CT_AbstractItemAttribute* attX = itemXY->firstItemAttributeByINModelName(resIn, this, DEFin_Xattribute);
                CT_AbstractItemAttribute* attY = itemXY->firstItemAttributeByINModelName(resIn, this, DEFin_Yattribute);

                if (attX != NULL) {x = attX->toDouble(itemXY, NULL);}
                if (attY != NULL) {y = attY->toDouble(itemXY, NULL);}

                CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*)itemXY->model();


                QList<CT_OutAbstractItemAttributeModel *> attributesModel = hash.values(itemModel);
                QList<CT_AbstractItemAttribute *> attributes = itemXY->itemAttributes(attributesModel);

                for (int i = 0 ; i < attributes.size() ; i++)
                {
                    CT_AbstractItemAttribute* attribute = attributes.at(i);
                    if (attribute != NULL)
                    {
                        CT_OutAbstractItemAttributeModel* attrModel = (CT_OutAbstractItemAttributeModel*) attribute->model();

                        QString attrUN = attrModel->uniqueName();

                        indexedAttributes.insert(QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrUN), QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*>(itemXY, attribute));
                    }
                }
            }

            CT_ItemIterator itItem(grp, this, DEFin_itemWithAttribute);
            while (itItem.hasNext())
            {
                CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) itItem.next();

                if (item != NULL)
                {
                    CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*)item->model();
                    QList<CT_OutAbstractItemAttributeModel *> attributesModel = hash.values(itemModel);
                    QList<CT_AbstractItemAttribute *> attributes = item->itemAttributes(attributesModel);

                    for (int i = 0 ; i < attributes.size() ; i++)
                    {
                        CT_AbstractItemAttribute* attribute = attributes.at(i);
                        if (attribute != NULL)
                        {
                            CT_OutAbstractItemAttributeModel* attrModel = (CT_OutAbstractItemAttributeModel*) attribute->model();

                            QString attrUN = attrModel->uniqueName();

                            indexedAttributes.insert(QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrUN), QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*>(item, attribute));
                        }
                    }
                }
            }

            bool hasMetricsToExport = !(indexedAttributes.isEmpty());

#ifdef USE_GDAL
            OGRFeature *vectorFeature = NULL;
            if (_vectorExport && hasMetricsToExport && vectorLayer != NULL)
            {
                vectorFeature = OGRFeature::CreateFeature(vectorLayer->GetLayerDefn());
                OGRPoint pt;
                pt.setX(x);
                pt.setY(y);
                vectorFeature->SetGeometry(&pt);
            }
#endif

            for (int i = 0 ; i < _modelsKeys.size() ; i++)
            {
                QString key = _modelsKeys.at(i);

                const QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*> &pair = indexedAttributes.value(key);

#ifdef USE_OPENCV
                CT_Image2D<double>* raster = rasters.value(key, NULL);
#endif

                if (pair.first != NULL && pair.second != NULL)
                {
                    if (hasMetricsToExport && _asciiExport && streamASCII != NULL)
                    {
                        (*streamASCII) << pair.second->toString(pair.first, NULL);
                    }
#ifdef USE_GDAL
                    if (_vectorExport)
                    {

                        QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
                        const char* fieldName = fieldNameBA;

                        if      (_ogrTypes.value(key) == OFTBinary)
                        {
                            vectorFeature->SetField(fieldName, pair.second->toInt(pair.first, NULL));
                        } else if (_ogrTypes.value(key) == OFTString)
                        {
                            QString text = replaceBadCharacters(pair.second->toString(pair.first, NULL));
                            QByteArray textBA = text.toLatin1();
                            const char* textChar = textBA;
                            vectorFeature->SetField(fieldName, textChar);
                        } else if (_ogrTypes.value(key) == OFTInteger)
                        {
                            vectorFeature->SetField(fieldName, pair.second->toInt(pair.first, NULL));
//                        } else if (_ogrTypes.value(key) == OFTInteger64)
//                        {
//                            vectorFeature->SetField(fieldName, pair.second->toInt(pair.first, NULL));
                        } else
                        {
                            vectorFeature->SetField(fieldName, pair.second->toDouble(pair.first, NULL));
                        }

                    }
#endif


#ifdef USE_OPENCV
                    if (_rasterExport && raster != NULL)
                    {
                        double val = pair.second->toDouble(pair.first, NULL);
                        if (val == NAN) {val = DEF_NA;}
                        raster->setValueAtCoords(x, y, val);
                    }
#endif
                }

                if (hasMetricsToExport && _asciiExport && streamASCII != NULL)
                {
                    if(i < _modelsKeys.size() - 1) {(*streamASCII) << "\t";} else {(*streamASCII) << "\n";}
                }

            }

#ifdef USE_GDAL
            if (_vectorExport && vectorLayer != NULL)
            {
                if (vectorLayer->CreateFeature(vectorFeature) != OGRERR_NONE)
                {
                    OGRFeature::DestroyFeature(vectorFeature);
                }
            }
#endif

        }

#ifdef USE_OPENCV
        if (_rasterExport)
        {
            QMapIterator<QString, CT_Image2D<double>*> itRaster(rasters);
            while (itRaster.hasNext())
            {
                itRaster.next();
                QString key = itRaster.key();
                QList<CT_AbstractItemDrawable* > rasterList;
                rasterList.append(itRaster.value());

                QString metricName = _names.value(key);
                QString fileName = QString("%1/%2%3%4").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName).arg("");

                if (_rasterDriverName == DEF_ESRI_ASCII_Grid)
                {
                    PB_Grid2DExporter exporter;
                    exporter.init();

                    if (exporter.setExportFilePath(fileName))
                    {
                        exporter.setItemDrawableToExport(rasterList);
                        exporter.exportToFile();
                    }

                } else {
#ifdef USE_GDAL
                    GDALDriver* driver = _gdalRasterDrivers.value(_rasterDriverName, NULL);

                    if (driver != NULL)
                    {
                        PB_GDALExporter exporter(driver);
                        exporter.init();

                        if (exporter.setExportFilePath(fileName))
                        {
                            exporter.setItemDrawableToExport(rasterList);
                            exporter.exportToFile();
                        }
                    }
#endif
                }
            }
        }
#endif

    }


    if (fileASCII != NULL) {fileASCII->close(); delete fileASCII;}
    if (streamASCII != NULL) {delete streamASCII;}
#ifdef USE_GDAL
    if (_vectorExport && vectorDataSet != NULL)
    {
        GDALClose(vectorDataSet);
    }
#endif

}

void PB_StepExportAttributesInLoop::replaceBadCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(replaceBadCharacters(it.value()));
    }
}

QString PB_StepExportAttributesInLoop::replaceBadCharacters(const QString &name) const
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

QMap<QString, QString> PB_StepExportAttributesInLoop::computeShortNames(const QMap<QString, QString> &names) const
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
           existing.append(value);
       } else {
           QString newValue = value.left(10);
           int cpt = 2;
           while (existing.contains(newValue))
           {
               QString number = QVariant(cpt++).toString();
               newValue = QString("%1%2").arg(value.left(10 - number.length())).arg(number);
           }
           shortNames.insert(key, newValue);
           existing.append(newValue);
       }
   }

   return shortNames;
}
