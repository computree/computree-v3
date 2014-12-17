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

#include "ct_step/ct_stepbeginloop.h"
#include "ct_step/ct_stependloop.h"

#include "actions/pb_actionselectitemdrawablegv.h"
#include "actions/pb_actionshowitemdatagv.h"

#include "exporters/csv/pb_csvexporter.h"
#include "exporters/profile/pb_profileexporter.h"
#include "exporters/grid2d/pb_grid2dexporter.h"
#include "exporters/grid3d/pb_grid3dexporter.h"
#include "exporters/grid3d/pb_grid3dastableexporter.h"
#include "exporters/xyb/pb_xybexporter.h"
#include "exporters/ascrgb/pb_ascrgbexporter.h"
#include "exporters/groupdata/pb_groupdataexporter.h"
#include "exporters/mesh/pb_meshobjexporter.h"
#include "exporters/topology/pb_opfexporter.h"
#include "exporters/pbm/pb_pbmexporter.h"
#include "exporters/pgm/pb_pgmexporter.h"
#include "exporters/polygon2d/pb_polygon2dexporter.h"
#include "exporters/las/pb_lasexporter.h"

#include "ct_reader/ct_reader_xyb.h"
#include "ct_reader/ct_reader_ascrgb.h"
#include "ct_reader/ct_reader_larchitect_grid.h"
#include "ct_reader/ct_reader_opf.h"
#include "ct_reader/ct_reader_las.h"
#include "ct_reader/ct_reader_gdal.h"

#include "ct_step/ct_stepinitializedata.h"

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

QString PB_StepPluginManager::getKeyForStep(const CT_VirtualAbstractStep &step) const
{
    const PB_StepGenericExporter *sEx = dynamic_cast<const PB_StepGenericExporter*>(&step);

    if(sEx != NULL)
        return QString("PB_StepGenericExporter__") + sEx->getStepName();

    return CT_AbstractStepPlugin::getKeyForStep(step);
}

CT_VirtualAbstractStep* PB_StepPluginManager::getStepFromKey(QString key) const
{
    if(key.startsWith("PB_StepGenericExporter"))
    {
        key = key.remove("PB_StepGenericExporter__");

        QList<CT_StepSeparator*> separators = getGenericsStepAvailable();
        QListIterator<CT_StepSeparator*> it(separators);

        while(it.hasNext())
        {
            CT_StepSeparator *separator = it.next();

            if(separator->getTitle() == DEF_ExporterSeparatorTitle)
            {
                QList<CT_VirtualAbstractStep*> steps = separator->getStepList();
                QListIterator<CT_VirtualAbstractStep*> itS(steps);

                while(itS.hasNext())
                {
                    PB_StepGenericExporter *step = dynamic_cast<PB_StepGenericExporter*>(itS.next());

                    if(key == step->getStepName())
                        return step;
                }

                return NULL;
            }
        }
    }

    return CT_AbstractStepPlugin::getStepFromKey(key);
}

QSettings* PB_StepPluginManager::initQSettings()
{
    QSettings *settings = new QSettings("pluginBase.ini", QSettings::IniFormat);

    return settings;
}

bool PB_StepPluginManager::loadGenericsStep()
{
    clearGenericsStep();

    CT_StepSeparator *sep = addNewSeparator(new CT_StepSeparator());

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Nuages de points")));
    sep->addStep(new PB_StepReducePointsDensity(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSlicePointCloud(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Voxels")));
    sep->addStep(new PB_StepComputeHitGrid(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSelectCellsInGrid3D(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepFilterPointsByBoolGrid(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Houppiers, Trouées")));
    sep->addStep(new PB_StepSegmentCrowns(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSegmentGaps(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Gestion des items")));
    sep->addStep(new PB_StepAddAffiliationID(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSetAffiliationIDFromReference(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepUserItemSelection(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepUserItemCopy(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepManualInventory(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepSelectGroupsByReferenceHeight(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepValidateInventory(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepMatchItemsPositions(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepSeparator(QObject::tr("Boucles de scripts")));
    sep->addStep(new CT_StepBeginLoop(*createNewStepInitializeData(NULL)));
    sep->addStep(new CT_StepEndLoop(*createNewStepInitializeData(NULL)));

    return true;
}

bool PB_StepPluginManager::loadOpenFileStep()
{
    clearOpenFileStep();

    CT_StepLoadFileSeparator *sep = addNewSeparator(new CT_StepLoadFileSeparator(("ASCII Files")));
    sep->addStep(new PB_StepLoadAsciiFile02(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepLoadFileSeparator(("OBJ file")));
    sep->addStep(new PB_StepLoadObjFile(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepLoadFileSeparator(("Grid3D file")));
    sep->addStep(new PB_StepLoadGrid3dFile(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepLoadFileSeparator(("pbm file")));
    sep->addStep(new PB_StepLoadPbmFile(*createNewStepInitializeData(NULL)));

    sep = addNewSeparator(new CT_StepLoadFileSeparator(("pgm file")));
    sep->addStep(new PB_StepLoadPgmFile(*createNewStepInitializeData(NULL)));

    return true;
}

bool PB_StepPluginManager::loadCanBeAddedFirstStep()
{
    clearCanBeAddedFirstStep();

    // Ajoute une nouvelle section d'étapes pouvant être ajouter en tête de script
    CT_StepCanBeAddedFirstSeparator *sep = addNewSeparator(new CT_StepCanBeAddedFirstSeparator());

    // Ajout d'une étape
    sep->addStep(new CT_StepBeginLoop(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepLoadMultiXYBFiles(*createNewStepInitializeData(NULL)));
    sep->addStep(new PB_StepImportSegmaFilesForMatching(*createNewStepInitializeData(NULL)));

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

    CT_StandardExporterSeparator *sep = addNewSeparator(new CT_StandardExporterSeparator("CSV"));
    sep->addExporter(new PB_CSVExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("GroupData"));
    sep->addExporter(new PB_GroupDataExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("XYB"));
    sep->addExporter(new PB_XYBExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("ASCRGB"));
    sep->addExporter(new PB_ASCRGBExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("PROFILE"));
    sep->addExporter(new PB_ProfileExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("GRID2D"));
    sep->addExporter(new PB_Grid2DExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("GRID3D"));
    sep->addExporter(new PB_Grid3DExporter());
    sep->addExporter(new PB_Grid3DAsTableExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("POLYGON2D"));
    sep->addExporter(new PB_Polygon2DExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("MESH"));
    sep->addExporter(new PB_MeshObjExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("TOPOLOGY"));
    sep->addExporter(new PB_OPFExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("PBM"));
    sep->addExporter(new PB_PbmExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("PGM"));
    sep->addExporter(new PB_PgmExporter());

    sep = addNewSeparator(new CT_StandardExporterSeparator("LAS"));
    sep->addExporter(new PB_LASExporter());

    return true;
}

bool PB_StepPluginManager::loadReaders()
{
    clearReaders();

    CT_StandardReaderSeparator *sep = addNewSeparator(new CT_StandardReaderSeparator("Binary Files"));
    sep->addReader(new CT_Reader_XYB());

    sep = addNewSeparator(new CT_StandardReaderSeparator("LArchitect Grids"));
    sep->addReader(new CT_Reader_LArchitect_Grid());

    sep = addNewSeparator(new CT_StandardReaderSeparator("ASCII Files"));
    sep->addReader(new CT_Reader_ASCRGB());

    sep = addNewSeparator(new CT_StandardReaderSeparator("OPF Files"));
    sep->addReader(new CT_Reader_OPF());

    sep = addNewSeparator(new CT_StandardReaderSeparator("LAS Files"));
    sep->addReader(new CT_Reader_LAS());

    return true;
}

bool PB_StepPluginManager::loadAfterAllPluginsLoaded()
{
    // load gdal drivers and create readers
#ifdef USE_GDAL
    GDALAllRegister();
    GDALDriverManager *driverManager = GetGDALDriverManager();

    int count = driverManager->GetDriverCount();

    if(count > 0) {

        CT_StandardReaderSeparator *sep;

        for(int i=0; i<count; ++i) {
            GDALDriver *driver = driverManager->GetDriver(i);
            QString suffix = QString(driver->GetMetadataItem(GDAL_DMD_EXTENSION));
            QString name = QString(driver->GetMetadataItem(GDAL_DMD_LONGNAME));
            name.remove(QRegExp("\\(\\..*\\)"));

            if(!suffix.isEmpty() && !name.isEmpty()) {
                sep = addNewSeparator(new CT_StandardReaderSeparator(name));
                sep->addReader(new CT_Reader_GDAL(driver));
            }
        }
    }
#endif

    // create step for exporter and readers
    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();

    CT_StepSeparator *sepGE = addNewSeparator(new CT_StepSeparator(DEF_ExporterSeparatorTitle));

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
                sepGE->addStep(new PB_StepGenericExporter(*createNewStepInitializeData(NULL), pluginName, exporter->copy()));
            }
        }

        QList<CT_StandardReaderSeparator*> rsl = p->getReadersAvailable();
        QListIterator<CT_StandardReaderSeparator*> itR(rsl);

        while(itR.hasNext())
        {
            CT_StandardReaderSeparator *rs = itR.next();

            CT_StepLoadFileSeparator *sepReaders = addNewSeparator(new CT_StepLoadFileSeparator(rs->getTitle()));

            QListIterator<CT_AbstractReader*> itE(rs->readers());

            while(itE.hasNext())
            {
                CT_AbstractReader *reader = itE.next();
                sepReaders->addStep(new PB_StepGenericLoadFile(*createNewStepInitializeData(NULL), reader->copy()));
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
