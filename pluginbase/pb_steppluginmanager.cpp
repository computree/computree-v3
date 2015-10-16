/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "pb_steppluginmanager.h"

#include "ct_stepseparator.h"
#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "ct_actions/ct_actionsseparator.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "step/pb_steploadasciifile.h"
#include "step/pb_steploadasciifile02.h"
#include "step/pb_stepgenericexporter.h"
#include "step/pb_steploadmultixybfiles.h"
#include "step/pb_stepuseritemselection.h"
#include "step/pb_stepcomputehitgrid.h"
#include "step/pb_stepselectcellsingrid3d.h"
#include "step/pb_stepsegmentcrowns.h"
#include "step/pb_stepsegmentgaps.h"
#include "step/pb_stepfilterpointsbyboolgrid.h"
#include "step/pb_steploadobjfile.h"
#include "step/pb_steploadgrid3dfile.h"
#include "step/pb_steploadpbmfile.h"
#include "step/pb_steploadpgmfile.h"
#include "step/pb_stepgenericloadfile.h"
#include "step/pb_stepreducepointsdensity.h"
#include "step/pb_stepaddaffiliationid.h"
#include "step/pb_stepsetaffiliationidfromreference.h"
#include "step/pb_stepuseritemcopy.h"
#include "step/pb_stepslicepointcloud.h"
#include "step/pb_stepmanualinventory.h"
#include "step/pb_stepvalidateinventory.h"
#include "step/pb_stepselectgroupsbyreferenceheight.h"
#include "step/pb_stepmatchitemspositions.h"
#include "step/pb_stepimportsegmafilesformatching.h"
#include "step/pb_stepcreatedatasource.h"
#include "step/pb_steptransformpointcloud.h"
#include "step/pb_stepfilteritemsbyposition.h"
#include "step/pb_stepcomputepointmetrics.h"
#include "step/pb_stepcomputerastermetrics.h"

#include "ct_step/ct_stepbeginloop.h"
#include "ct_step/ct_stependloop.h"
#include "step/pb_stepbeginloopthroughgroups.h"
#include "step/pb_stepbeginloopthroughdatasource.h"
#include "step/pb_steploaddatafromitemposition.h"
#include "step/pb_stepcreateplotmanagerfromfile.h"
#include "step/pb_stepcreateplotmanagergrid.h"
#include "step/pb_steploadpositionsformatching.h"
#include "step/pb_steploadtreemap.h"
#include "step/pb_stepcompare3dgridscontents.h"
#include "step/pb_stepselectcellsingrid3dbybinarypattern.h"
#include "step/pb_stepextractlogbuffer.h"
#include "step/pb_stepfitcylinderoncluster.h"
#include "step/pb_stepextractpositionsfromdensity.h"
#include "step/pb_stepmergeclustersfrompositions.h"
#include "step/pb_stepmergeclustersfrompositions02.h"
#include "step/pb_stepmodifypositions2d.h"
#include "step/pb_stepcomputecrownprojection.h"
#include "step/pb_stepapplypointfilters.h"
#include "step/pb_steploadplotareas.h"
#include "step/pb_stepcorrectalsprofile.h"
#include "step/pb_stepexportitemlist.h"
#include "step/pb_stepaddattributevalue.h"
#include "step/pb_stepdetectverticalalignments.h"
#include "step/pb_stepselectbboxbyfilename.h"

#ifdef USE_OPENCV
#include "step/pb_stepconvertfloatimagetoqint32.h"
#include "step/pb_stepcomputeattributemapfromclusters.h"
#endif

#include "actions/pb_actionselectitemdrawablegv.h"
#include "actions/pb_actionshowitemdatagv.h"

#include "exporters/csv/pb_csvexporter.h"
#include "exporters/profile/pb_profileexporter.h"
#include "exporters/grid2d/pb_grid2dexporter.h"
#include "exporters/grid3d/pb_grid3dexporter.h"
#include "exporters/grid3d/pb_grid3dastableexporter.h"
#include "exporters/xyb/pb_xybexporter.h"
#include "exporters/xyb/pb_multixybexporter.h"
#include "exporters/ascrgb/pb_ascrgbexporter.h"
#include "exporters/groupdata/pb_groupdataexporter.h"
#include "exporters/mesh/pb_meshobjexporter.h"
#include "exporters/topology/pb_opfexporter.h"
#include "exporters/pbm/pb_pbmexporter.h"
#include "exporters/pgm/pb_pgmexporter.h"
#include "exporters/polygon2d/pb_polygon2dexporter.h"
#include "exporters/las/pb_lasexporter.h"
#include "exporters/gdal/pb_gdalexporter.h"
#include "exporters/ascid/pb_ascidexporter.h"
#include "exporters/grid3dhist/pb_grid3dhistexporter.h"

#include "ct_reader/ct_reader_xyb.h"
#include "ct_reader/ct_reader_ascrgb.h"
#include "ct_reader/ct_reader_larchitect_grid.h"
#include "ct_reader/ct_reader_opf.h"
#include "ct_reader/ct_reader_las.h"
#include "ct_reader/ct_reader_gdal.h"
#include "ct_reader/ct_reader_terrascanprj.h"

#include "filter/pb_filterbyreturntype.h"
#include "filter/pb_filterremoveupperoutliers.h"

#include "metric/pb_metricquantiles.h"
#include "metric/pb_metriccomputestats.h"


#include "ct_step/ct_stepinitializedata.h"

#include "ct_tools/ct_gdaltools.h"

#include <QMessageBox>

#ifdef USE_GDAL
#include "gdal_priv.h"
#endif

#define DEF_ExporterSeparatorTitle "Exporters"

PB_StepPluginManager::PB_StepPluginManager() : CT_AbstractStepPlugin()
{
    m_fileLog.setFilePath("./logPB.txt");
    m_fileLog.setSeverityAccepted(QVector<int>() << LogInterface::debug);
    m_fileLog.setFilter("pb");

    #ifdef USE_GDAL
    CPLSetErrorHandler(PB_StepPluginManager::staticGdalErrorHandler);
    #endif
}

PB_StepPluginManager::~PB_StepPluginManager()
{
    PS_LOG->removeLogListener(&m_fileLog);
}

bool PB_StepPluginManager::init()
{
    PS_LOG->addNormalLogListener(&m_fileLog);
    PS_LOG->addMessage(LogInterface::debug, LogInterface::plugin, QObject::tr("Plugin_Base initialized"), "pb");

    return CT_AbstractStepPlugin::init();
}

QSettings* PB_StepPluginManager::initQSettings()
{
    QSettings *settings = new QSettings("pluginBase.ini", QSettings::IniFormat);

    return settings;
}

bool PB_StepPluginManager::loadGenericsStep()
{
    addNewFilterStep<PB_StepReducePointsDensity>(CT_StepsMenu::LP_PointCloud);
    addNewFilterStep<PB_StepSlicePointCloud>(CT_StepsMenu::LP_PointCloud);
    addNewGeometryTransformStep<PB_StepTransformPointCloud>(CT_StepsMenu::LP_PointCloud);
    addNewExtractStep<PB_StepExtractLogBuffer>(CT_StepsMenu::LP_PointCloud);
    addNewOtherStep<PB_StepComputeCrownProjection>(QObject::tr("Houppiers"));        

    CT_StepSeparator *sep;

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Voxels")));
    sep->addStep(new PB_StepComputeHitGrid(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepCorrectALSProfile(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSelectCellsInGrid3D(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepFilterPointsByBoolGrid(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSelectCellsInGrid3DByBinaryPattern(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepCompare3DGridsContents(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Houppiers, Trouées")));
    sep->addStep(new PB_StepSegmentCrowns(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSegmentGaps(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepMergeClustersFromPositions(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepMergeClustersFromPositions02(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Gestion des items")));
    sep->addStep(new PB_StepAddAffiliationID(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSetAffiliationIDFromReference(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepUserItemSelection(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepUserItemCopy(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepManualInventory(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSelectGroupsByReferenceHeight(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepValidateInventory(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepFitCylinderOnCluster(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepFilterItemsByPosition(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepMatchItemsPositions(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepModifyPositions2D(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepExportItemList(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepAddAttributeValue(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepDetectVerticalAlignments(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Traitement par lots")));
    sep->addStep(new PB_StepLoadDataFromItemPosition(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepBeginLoopThroughDataSource(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepBeginLoopThroughGroups(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepCreatePlotManagerFromFile(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepCreatePlotManagerGrid(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSelectBBoxByFileName(*createNewStepInitializeData(NULL)));

    sep->addStep(new PB_StepLoadPlotAreas(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepApplyPointFilters(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepComputePointMetrics(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepComputeRasterMetrics(*createNewStepInitializeData(NULL)));
    sep->addStep(new CT_StepEndLoop(*createNewStepInitializeData(NULL)));

    #ifdef USE_OPENCV
    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Test")));
    sep->addStep(new CT_StepBeginLoop(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepConvertFloatImageToqint32(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepComputeAttributeMapFromClusters(*createNewStepInitializeData(NULL)));
    #endif

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Etapes de chargement")));
    sep->addStep(new PB_StepLoadAsciiFile02(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepLoadObjFile(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepLoadGrid3dFile(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepLoadPbmFile(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepLoadPgmFile(*createNewStepInitializeData(NULL)));

    return true;
}

bool PB_StepPluginManager::loadOpenFileStep()
{
    clearOpenFileStep();

    addNewLoadStep<PB_StepLoadAsciiFile02>(QObject::tr("ASCII Files"));
    addNewLoadStep<PB_StepLoadObjFile>(QObject::tr("OBJ Files"));
    addNewLoadStep<PB_StepLoadGrid3dFile>(QObject::tr("Grid3D Files"));
    addNewLoadStep<PB_StepLoadPbmFile>(QObject::tr("PBM Files"));
    addNewLoadStep<PB_StepLoadPgmFile>(QObject::tr("PGM Files"));

    return true;
}

bool PB_StepPluginManager::loadCanBeAddedFirstStep()
{
    clearCanBeAddedFirstStep();

    addNewCreateStep<PB_StepCreateDataSource>(CT_StepsMenu::LP_DataSource);
    addNewLoadStep<PB_StepLoadPositionsForMatching>();
    addNewLoadStep<PB_StepLoadTreeMap>(QObject::tr("Tree maps"));
    addNewLoadStep<PB_StepImportSegmaFilesForMatching>(QObject::tr("Segma"));
    addNewLoadStep<PB_StepLoadMultiXYBFiles>(CT_StepsMenu::LP_PointCloud);
    addNewWorkflowStep<CT_StepBeginLoop>();

    // Si toutes les étapes ont pu être ajoutées, la méthode renvoie true (on peut continuer)
    return true;
}

bool PB_StepPluginManager::loadActions()
{
    clearActions();

    CT_ActionsSeparator *sep = addNewSeparator(new CT_ActionsSeparator(CT_AbstractAction::TYPE_SELECTION));
    sep->addAction(new PB_ActionSelectItemDrawableGV());

    sep = addNewSeparator(new CT_ActionsSeparator(CT_AbstractAction::TYPE_INFORMATION));
    sep->addAction(new PB_ActionShowItemDataGV());
    return true;
}

bool PB_StepPluginManager::loadExporters()
{
    clearExporters();

    CT_StandardExporterSeparator *sep = addNewSeparator(new CT_StandardExporterSeparator("Exporters"));
    sep->addExporter(new PB_CSVExporter());
    sep->addExporter(new PB_GroupDataExporter());
    sep->addExporter(new PB_XYBExporter());
    sep->addExporter(new PB_MultiXYBExporter());
    sep->addExporter(new PB_ASCRGBExporter());
    sep->addExporter(new PB_ASCIDExporter());
    sep->addExporter(new PB_ProfileExporter());
    sep->addExporter(new PB_Grid3DHistExporter());
    sep->addExporter(new PB_Grid2DExporter());
    sep->addExporter(new PB_Grid3DExporter());
    sep->addExporter(new PB_Grid3DAsTableExporter());
    sep->addExporter(new PB_Polygon2DExporter());
    sep->addExporter(new PB_MeshObjExporter());
    sep->addExporter(new PB_OPFExporter());
    sep->addExporter(new PB_PbmExporter());
    sep->addExporter(new PB_PgmExporter());
    sep->addExporter(new PB_LASExporter());

    return true;
}

bool PB_StepPluginManager::loadReaders()
{
    clearReaders();

    CT_StandardReaderSeparator *sep = addNewSeparator(new CT_StandardReaderSeparator("Readers"));
    sep->addReader(new CT_Reader_XYB());
    sep->addReader(new CT_Reader_LArchitect_Grid());
    sep->addReader(new CT_Reader_ASCRGB());
    sep->addReader(new CT_Reader_OPF());
    sep->addReader(new CT_Reader_LAS());
    sep->addReader(new CT_Reader_TerraScanPrj());

    return true;
}

bool PB_StepPluginManager::loadFilters()
{
    addNewFilter(new PB_FilterByReturnType());
    addNewFilter(new PB_FilterRemoveUpperOutliers());
    return true;
}

bool PB_StepPluginManager::loadMetrics()
{
    addNewMetric(new PB_MetricQuantiles());
    addNewMetric(new PB_MetricComputeStats());return true;
}

#ifdef USE_GDAL
bool GDALExporterLessThan(const PB_GDALExporter *s1, const PB_GDALExporter *s2)
{
    return s1->getExporterCustomName() < s2->getExporterCustomName();
}

bool GDALReaderLessThan(const CT_Reader_GDAL *s1, const CT_Reader_GDAL *s2)
{
    return CT_GdalTools::staticGdalDriverName(s1->getDriver()) < CT_GdalTools::staticGdalDriverName(s2->getDriver());
}
#endif

bool PB_StepPluginManager::loadAfterAllPluginsLoaded()
{
    // load gdal drivers and create readers and exporters
#ifdef USE_GDAL
    GDALAllRegister();
    GDALDriverManager *driverManager = GetGDALDriverManager();

    int count = driverManager->GetDriverCount();

    if(count > 0) {

        CT_StandardReaderSeparator *sepR;
        CT_StandardExporterSeparator *sepE = NULL;

        QList<CT_Reader_GDAL*> gdalReaderC;
        QList<PB_GDALExporter*> gdalExpoC;

        for(int i=0; i<count; ++i) {
            GDALDriver *driver = driverManager->GetDriver(i);
            QString name = CT_GdalTools::staticGdalDriverName(driver);

            if(!name.isEmpty()) {
                gdalReaderC.append(new CT_Reader_GDAL(driver));
                gdalExpoC.append(new PB_GDALExporter(driver));
            }
        }

        if(!gdalExpoC.isEmpty()) {
            sepE = addNewSeparator(new CT_StandardExporterSeparator("GDAL"));

            qSort(gdalExpoC.begin(), gdalExpoC.end(), GDALExporterLessThan);

            QListIterator<PB_GDALExporter*> itGD(gdalExpoC);

            while(itGD.hasNext())
                sepE->addExporter(itGD.next());
        }

        if(!gdalReaderC.isEmpty()) {


            qSort(gdalReaderC.begin(), gdalReaderC.end(), GDALReaderLessThan);

            QListIterator<CT_Reader_GDAL*> itGD(gdalReaderC);

            while(itGD.hasNext()) {
                CT_Reader_GDAL *reader = itGD.next();
                sepR = addNewSeparator(new CT_StandardReaderSeparator(CT_GdalTools::staticGdalDriverName(reader->getDriver())));
                sepR->addReader(reader);
            }
        }
    }
#endif

    // create step for exporter and readers
    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();

    int s = pm->countPluginLoaded();
    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);
        QString pluginName = pm->getPluginName(i);

        QList<CT_StandardExporterSeparator*> esl = p->getExportersAvailable();
        QListIterator<CT_StandardExporterSeparator*> it(esl);

        while(it.hasNext())
        {
            CT_StandardExporterSeparator *es = it.next();

            QListIterator<CT_AbstractExporter*> itE(es->exporters());

            while(itE.hasNext())
            {
                CT_AbstractExporter *exporter = itE.next();
                QString subMenu = "Exporters";
                if (dynamic_cast<PB_GDALExporter*>(exporter) != NULL)
                {
                    subMenu = "GDAL Exporters";
                }
                addNewStep(new PB_StepGenericExporter(*createNewStepInitializeData(NULL), pluginName, exporter->copy()), CT_StepsMenu::LO_Export, subMenu);
            }
        }

        QList<CT_StandardReaderSeparator*> rsl = p->getReadersAvailable();
        QListIterator<CT_StandardReaderSeparator*> itR(rsl);
        while(itR.hasNext())
        {
            CT_StandardReaderSeparator *rs = itR.next();

            QListIterator<CT_AbstractReader*> itE(rs->readers());
            while(itE.hasNext())
            {
                CT_AbstractReader *reader = itE.next();
                QString subMenu = "Readers";
                if (dynamic_cast<CT_Reader_GDAL*>(reader) != NULL)
                {
                    subMenu = "GDAL Readers";
                }
                addNewStep(new PB_StepGenericLoadFile(*createNewStepInitializeData(NULL), reader->copy()), CT_StepsMenu::LO_Load, subMenu);
            }
        }

    }

    return true;
}

void PB_StepPluginManager::aboutToBeUnloaded()
{
    clearGenericsStep();
    clearOpenFileStep();
    clearCanBeAddedFirstStep();
}

#ifdef USE_GDAL
void PB_StepPluginManager::staticGdalErrorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
    int severity = LogInterface::trace;

    switch(eErrClass) {
    case CE_Debug : severity = LogInterface::debug;
        break;

    case CE_Warning: severity = LogInterface::warning;
        break;

    case CE_Failure: severity = LogInterface::error;
        break;

    case CE_Fatal: severity = LogInterface::fatal;
        break;
    }

    PS_LOG->addMessage(severity, LogInterface::plugin, QString(msg));
}
#endif
