#include "pb_steptransformpointcloud.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/tools/iterator/ct_resultitemiterator.h"

#include "ct_global/ct_context.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include "ct_coordinates/ct_defaultcoordinatesystem.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_transformationmatrix.h"

#define DEFin_resScene "resScene"
#define DEFin_grpScene "grpScene"
#define DEFin_scene "scene"

#define DEFin_resMat "resMat"
#define DEFin_grpMat "grpMat"
#define DEFin_mat "mat"

#define DEFout_resScene "outresScene"
#define DEFout_grpScene "outgrpScene"
#define DEFout_scene "outscene"


// Constructor : initialization of parameters
PB_StepTransformPointCloud::PB_StepTransformPointCloud(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepTransformPointCloud::getStepDescription() const
{
    return tr("Applique une matrice de transformation à une (des) scène(s)");
}

// Step detailled description
QString PB_StepTransformPointCloud::getStepDetailledDescription() const
{
    return tr("");
}

// Step URL
QString PB_StepTransformPointCloud::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepTransformPointCloud::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepTransformPointCloud(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepTransformPointCloud::createInResultModelListProtected()
{
    CT_InResultModelGroup* resScene = createNewInResultModel(DEFin_resScene, tr("Scène(s)"), "", true);
    resScene->setZeroOrMoreRootGroup();
    resScene->addGroupModel("", DEFin_grpScene, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resScene->addItemModel(DEFin_grpScene, DEFin_scene, CT_Scene::staticGetType(), tr("Scène"));

    CT_InResultModelGroup* resMat = createNewInResultModel(DEFin_resMat, tr("Matrice de transformation"), "", true);
    resMat->setZeroOrMoreRootGroup();
    resMat->addGroupModel("", DEFin_grpMat, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resMat->addItemModel(DEFin_grpMat, DEFin_mat, CT_TransformationMatrix::staticGetType(), tr("Matrice de transformation"));
}

// Creation and affiliation of OUT models
void PB_StepTransformPointCloud::createOutResultModelListProtected()
{
    CT_OutResultModelGroup* res = createNewOutResultModel(DEFout_resScene, tr("Scène(s) transformée(s)"));
    res->setRootGroup(DEFout_grpScene, new CT_StandardItemGroup(), tr("Groupe"));
    res->addItemModel(DEFout_grpScene, DEFout_scene, new CT_Scene(), tr("Scène(s) transformée(s)"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepTransformPointCloud::createPostConfigurationDialog()
{
    // No parameter dialog for this step
}

void PB_StepTransformPointCloud::compute()
{
    CT_ResultGroup *resin_Scene = getInputResults().at(0);
    CT_ResultGroup *resin_Mat = getInputResults().at(1);

    CT_ResultGroup *resout_Scene = getOutResultList().at(0);


    CT_ResultItemIterator itMat(resin_Mat, this, DEFin_mat);
    if (itMat.hasNext())
    {
        const CT_TransformationMatrix *trMat = (const CT_TransformationMatrix*) itMat.next();

        if (trMat != NULL)
        {
            QMap<CT_AbstractCoordinateSystem*, CT_AbstractCoordinateSystem*> coordSysCorresp;



            CT_ResultItemIterator itScene(resin_Scene, this, DEFin_scene);
            while (itScene.hasNext() && !isStopped())
            {
                const CT_Scene *inScene = (const CT_Scene*) itScene.next();

                const CT_AbstractPointCloudIndex* cloudIndex = inScene->getPointCloudIndex();
                size_t n_points = cloudIndex->size();

                if (n_points > 0)
                {
                    // limites de la bounding-box de la scène de sortie
                    double minX = std::numeric_limits<double>::max();
                    double minY = std::numeric_limits<double>::max();
                    double minZ = std::numeric_limits<double>::max();

                    double maxX = -std::numeric_limits<double>::max();
                    double maxY = -std::numeric_limits<double>::max();
                    double maxZ = -std::numeric_limits<double>::max();

                    CT_Repository::CT_AbstractNotModifiablePCIR pcir = PS_REPOSITORY->createNewPointCloud(n_points);
                    CT_Scene *outScene = new CT_Scene(DEFout_scene, resout_Scene);

                    for (size_t i = 0 ; i < n_points ; i++)
                    {
                        size_t globalIndex;
                        const CT_Point &pointFloat = cloudIndex->constTAt(i, globalIndex);

                        CT_AbstractCoordinateSystem* currentSystem = PS_COORDINATES_SYS_MANAGER->coordinateSystemForPointAt(globalIndex);                        
                        Eigen::Vector3d point;
                        currentSystem->convertExport(pointFloat(0), pointFloat(1), pointFloat(2), point(0), point(1), point(2));

                        CT_AbstractCoordinateSystem* transSystem = coordSysCorresp.value(currentSystem, NULL);
                        if (transSystem == NULL)
                        {
                            Eigen::Vector3d offset;
                            currentSystem->convertExport(0, 0, 0, offset(0), offset(1), offset(2));

                            trMat->transform(offset);

                            transSystem = new CT_DefaultCoordinateSystem(offset(0), offset(1), offset(2));

                            outScene->registerCoordinateSystem(PS_COORDINATES_SYS_MANAGER->registerCoordinateSystem(transSystem));
                            coordSysCorresp.insert(currentSystem, transSystem);
                        }

                        Eigen::Vector3d trPoint = point;
                        trMat->transform(trPoint);

                        size_t globalIndexTr;
                        CT_Point &trPointFloat = pcir->tAt(i, globalIndexTr);
                        transSystem->convertImport(trPoint(0), trPoint(1), trPoint(2), trPointFloat(0), trPointFloat(1), trPointFloat(2));

                        PS_COORDINATES_SYS_MANAGER->setCoordinateSystemForPointAt(globalIndexTr, PS_COORDINATES_SYS_MANAGER->indexOfCoordinateSystem(transSystem));

                        if (trPoint(0) < minX) {minX = trPoint(0);}
                        if (trPoint(1) < minY) {minY = trPoint(1);}
                        if (trPoint(2) < minZ) {minZ = trPoint(2);}
                        if (trPoint(0) > maxX) {maxX = trPoint(0);}
                        if (trPoint(1) > maxY) {maxY = trPoint(1);}
                        if (trPoint(2) > maxZ) {maxZ = trPoint(2);}
                    }



                    CT_StandardItemGroup *outGroup = new CT_StandardItemGroup(DEFout_grpScene, resout_Scene);
                    outScene->setPointCloudIndexRegistered(pcir);
                    outScene->setBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);

                    outGroup->addItemDrawable(outScene);
                    resout_Scene->addGroup(outGroup);
                }


            }
        }


    }






}
