#include "pb_steploadmultixybfiles.h"

// Inclusion of in models
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_global/ct_context.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_reader/ct_reader_xyb.h"
#include "qdebug.h"

#include <limits>

// Alias for indexing out models
#define DEF_resultOut_individualScenes "rsce"
#define DEF_groupOut_g "g"
#define DEF_itemOut_individualScene "sce"
#define DEF_itemOut_individualIntensity "int"
#define DEF_itemOut_scanner "sca"
#define DEF_resultOut_mergedScene "rmsce"
#define DEF_groupOut_gm "gm"
#define DEF_itemOut_mergedScene "scm"

// Constructor : initialization of parameters
PB_StepLoadMultiXYBFiles::PB_StepLoadMultiXYBFiles(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _radiusFiltered = true;
    _radius = 20;
}

// Step description (tooltip of contextual menu)
QString PB_StepLoadMultiXYBFiles::getStepDescription() const
{
    return "Load multi-scans for a plot of given radius";
}

// Step copy method
CT_VirtualAbstractStep* PB_StepLoadMultiXYBFiles::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadMultiXYBFiles(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepLoadMultiXYBFiles::createInResultModelListProtected()
{
    setNotNeedInputResult();
}

// Creation and affiliation of OUT models
void PB_StepLoadMultiXYBFiles::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *resultOut_individualScenes = createNewOutResultModel(DEF_resultOut_individualScenes, tr("IndividualScenes"));
    resultOut_individualScenes->setRootGroup(DEF_groupOut_g, new CT_StandardItemGroup(), tr("g"));
    resultOut_individualScenes->addItemModel(DEF_groupOut_g, DEF_itemOut_individualScene, new CT_Scene(), tr("IndividualScene"));
    resultOut_individualScenes->addItemModel(DEF_groupOut_g, DEF_itemOut_individualIntensity, new CT_PointsAttributesScalarTemplated<quint16>(), tr("IndividualIntensity"));
    resultOut_individualScenes->addItemModel(DEF_groupOut_g, DEF_itemOut_scanner, new CT_Scanner(), tr("ScanPosition"));

    CT_OutResultModelGroup *resultOutModel_mergedScene = createNewOutResultModel(DEF_resultOut_mergedScene, tr("MergedScene"));
    resultOutModel_mergedScene->setRootGroup(DEF_groupOut_gm, new CT_StandardItemGroup(), tr("gm"));
    resultOutModel_mergedScene->addItemModel(DEF_groupOut_gm, DEF_itemOut_mergedScene, new CT_Scene(), tr("MergedScene"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepLoadMultiXYBFiles::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addFileChoice("Choisir les fichiers .xyb", CT_FileChoiceButton::OneOrMoreExistingFiles, "Fichier xyb (*.xyb)", _filesNames);
    configDialog->addBool("Extraire une placette de rayon fixé", "", "", _radiusFiltered);
    configDialog->addDouble("Rayon de la placette", "m", 0.1, 1000, 2, _radius, 0);
}

void PB_StepLoadMultiXYBFiles::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    CT_ResultGroup* resultOut_individualScenes = outResultList.at(0);
    CT_ResultGroup* resultOut_mergedScene = outResultList.at(1);

    if (!_radiusFiltered) {_radius = 0;}

    float xmin = std::numeric_limits<float>::max();
    float ymin = std::numeric_limits<float>::max();
    float zmin = std::numeric_limits<float>::max();

    float xmax = -std::numeric_limits<float>::max();
    float ymax = -std::numeric_limits<float>::max();
    float zmax = -std::numeric_limits<float>::max();


    QList< CT_ABSTRACT_PCIR > individualScenes;

    int baseProgress = 0;
    int fileNumber = _filesNames.size();
    int progressIncrement = 100/fileNumber;

    for (int i = 0 ; i < fileNumber ; ++i)
    {
        CT_Reader_XYB reader;
        bool readerValid = reader.setFilePath(_filesNames.at(i));

        if(readerValid)
        {
            reader.setRadiusFilter(_radius);
            reader.init();

            readerValid = reader.readFile();

            if(readerValid)
            {               
                CT_Scene* scene = (CT_Scene*)reader.takeFirstItemDrawableOfModel(DEF_CT_Reader_XYB_sceneOut, resultOut_individualScenes, DEF_itemOut_individualScene);
                individualScenes.append(scene->getPointCloudIndexRegistered());

                CT_StandardItemGroup* groupOut_individualScene = new CT_StandardItemGroup(DEF_groupOut_g, resultOut_individualScenes);
                groupOut_individualScene->addItemDrawable(scene);

                CT_PointsAttributesScalarTemplated<quint16>* intensity = (CT_PointsAttributesScalarTemplated<quint16>*)reader.takeFirstItemDrawableOfModel(DEF_CT_Reader_XYB_intensityOut, resultOut_individualScenes, DEF_itemOut_individualIntensity);
                groupOut_individualScene->addItemDrawable(intensity);

                CT_Scanner* scanner = (CT_Scanner*)reader.takeFirstItemDrawableOfModel(DEF_CT_Reader_XYB_scannerOut, resultOut_individualScenes, DEF_itemOut_scanner);

                if(scanner != NULL)
                {
                    scanner->setID(i);
                    groupOut_individualScene->addItemDrawable(scanner);
                }

                resultOut_individualScenes->addGroup(groupOut_individualScene);
                if (scene->minX()<xmin) {xmin = scene->minX();}
                if (scene->maxX()>xmax) {xmax = scene->maxX();}
                if (scene->minY()<ymin) {ymin = scene->minY();}
                if (scene->maxY()>ymax) {ymax = scene->maxY();}
                if (scene->minZ()<zmin) {zmin = scene->minZ();}
                if (scene->maxZ()>zmax) {zmax = scene->maxZ();}
            }
        }

        baseProgress += progressIncrement;
        setProgress(baseProgress);
    }


    CT_StandardItemGroup* groupOut_mergedScene = new CT_StandardItemGroup(DEF_groupOut_gm, resultOut_mergedScene);

    CT_Scene *mergedScene = new CT_Scene(DEF_itemOut_mergedScene, resultOut_mergedScene);
    mergedScene->setBoundingBox(xmin,ymin,zmin, xmax,ymax,zmax);
    mergedScene->setPointCloudIndexRegistered(PS_REPOSITORY->mergePointCloudContiguous(individualScenes));

    groupOut_mergedScene->addItemDrawable(mergedScene);
    resultOut_mergedScene->addGroup(groupOut_mergedScene);

    setProgress(100);

}
