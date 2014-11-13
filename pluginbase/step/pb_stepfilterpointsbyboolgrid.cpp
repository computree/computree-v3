#include "pb_stepfilterpointsbyboolgrid.h"

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include <limits>

// Alias for indexing in models
#define DEF_IN_Result_Scenes "rsc"
#define DEF_IN_Group_Scenes "grp"
#define DEF_IN_scene "sc"
#define DEF_IN_Result_Grids "rgr"
#define DEF_IN_GridGroup "gg"
#define DEF_IN_grid "gr"


// Constructor : initialization of parameters
PB_StepFilterPointsByBoolGrid::PB_StepFilterPointsByBoolGrid(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepFilterPointsByBoolGrid::getStepDescription() const
{
    return tr("Filtrage d'un nuage de points // grille 3D booléenne");
}

// Step description (tooltip of contextual menu)
QString PB_StepFilterPointsByBoolGrid::getStepDetailledDescription() const
{
    return tr("Cette étape teste pour chaque point des scènes d'entrée s'il est contenu dans une case \"vraie\" de la grille booléenne choisie. "
              "Si oui, le point est conservé. Sinon, il n'est pas conservé.<br>"
              "Plusieures scènes peuvent être traitées avec la même étape.<br>"
              "Chaque scène filtrée est ajoutée au groupe contenant la grille d'entrée."
              "Si le résultat d'entrée contient plusieurs grilles, une scène est produite pour chacune (sur la base du cumul de toutes les scènes d'entrée)");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepFilterPointsByBoolGrid::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepFilterPointsByBoolGrid(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepFilterPointsByBoolGrid::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultModel = createNewInResultModel(DEF_IN_Result_Scenes, tr("Scènes à filtrer"), "", true);

    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_IN_Group_Scenes);
    resultModel->addItemModel(DEF_IN_Group_Scenes, DEF_IN_scene, CT_Scene::staticGetType(), tr("Scène"));


    CT_InResultModelGroupToCopy *resultModelGrd = createNewInResultModelForCopy(DEF_IN_Result_Grids, tr("Grille(s) de filtrage"));

    resultModelGrd->setZeroOrMoreRootGroup();
    resultModelGrd->addGroupModel("", DEF_IN_GridGroup);
    resultModelGrd->addItemModel(DEF_IN_GridGroup, DEF_IN_grid, CT_Grid3D<bool>::staticGetType(), tr("Grille(s) de filtrage"));
}

// Creation and affiliation of OUT models
void PB_StepFilterPointsByBoolGrid::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEF_IN_Result_Grids);

    res->addItemModel(DEF_IN_GridGroup, _ModelOut_Scene, new CT_Scene(), tr("Scène filtrée"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepFilterPointsByBoolGrid::createPostConfigurationDialog()
{
    // No parameter dialog for this step
}

void PB_StepFilterPointsByBoolGrid::compute()
{
    // Gets IN result
    CT_ResultGroup* resultIn_Scenes = getInputResultsForModel(DEF_IN_Result_Scenes).first();

    // Gets OUT result
    CT_ResultGroup* resultOut = getOutResultList().first();

    int gridNum = 1;


    // create a iterator to find groups that user selected with the IN model named DEF_IN_GridGroup
    CT_ResultGroupIterator it(resultOut, this, DEF_IN_GridGroup);

    // iterate over all groups
    while(!isStopped() && it.hasNext())
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)it.next();
        CT_Grid3D<bool> *boolGrid = (CT_Grid3D<bool>*) group->firstItemByINModelName(this, DEF_IN_grid);

        if (boolGrid != NULL)
        {
            // Indices de la scène filtrée
            CT_PointCloudIndexVector *resPointCloudIndex = new CT_PointCloudIndexVector();

            // BoundingBox de la nouvelle scène
            float xmin = std::numeric_limits<float>::max();
            float ymin = std::numeric_limits<float>::max();
            float zmin = std::numeric_limits<float>::max();

            float xmax = -std::numeric_limits<float>::max();
            float ymax = -std::numeric_limits<float>::max();
            float zmax = -std::numeric_limits<float>::max();

            int sceneNum = 1;

            CT_ResultItemIterator itSc(resultIn_Scenes, this, DEF_IN_scene);
            while(!isStopped() && itSc.hasNext())
            {
                const CT_Scene *in_scene = (CT_Scene*) itSc.next();

                PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("Grille %1, Scène %2:")).arg(gridNum++).arg(sceneNum++));

                const CT_AbstractPointCloudIndex *pointCloudIndex = in_scene->getPointCloudIndex();
                size_t n_points = pointCloudIndex->size();

                PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("La scène %1 points...")).arg(n_points));


                size_t i = 0;

                size_t nbOfFilteredPoints = 0;
                while((i<n_points))
                {
                    size_t index;
                    const CT_Point &point = pointCloudIndex->constTAt(i, index);
                    float x = point(0);
                    float y = point(1);
                    float z = point(2);

                    if (boolGrid->valueAtXYZ(x, y, z))
                    {
                        resPointCloudIndex->addIndex(index);

                        if (x < xmin) {xmin = x;}
                        if (x > xmax) {xmax = x;}
                        if (y < ymin) {ymin = y;}
                        if (y > ymax) {ymax = y;}
                        if (z < zmin) {zmin = z;}
                        if (z > zmax) {zmax = z;}
                        ++nbOfFilteredPoints;
                    }

                    ++i;
                }

                PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("...%1 points ont été conservés")).arg(nbOfFilteredPoints));
            }

            if (resPointCloudIndex->size() > 0)
            {
                // creation et ajout de la scene
                CT_Scene *outScene = new CT_Scene(_ModelOut_Scene.completeName(), resultOut);

                outScene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);
                outScene->setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(resPointCloudIndex));
                group->addItemDrawable(outScene);
            }
        }
    }
}



