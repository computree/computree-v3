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

#include "ct_iterator/ct_pointiterator.h"
#include "ct_iterator/ct_mutablepointiterator.h"

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
            QMap<CT_AbstractCoordinateSystem*, GLuint> coordSysCorresp;

            CT_ResultItemIterator itScene(resin_Scene, this, DEFin_scene);
            while (itScene.hasNext() && !isStopped())
            {
                const CT_Scene *inScene = (const CT_Scene*) itScene.next();

                CT_PointIterator itP(inScene->getPointCloudIndex());

                if (itP.hasNext())
                {
                    // limites de la bounding-box de la scène de sortie
                    double minX = std::numeric_limits<double>::max();
                    double minY = minX;
                    double minZ = minX;

                    double maxX = -minX;
                    double maxY = -minX;
                    double maxZ = -minX;

                    CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(itP.size());
                    CT_Scene *outScene = new CT_Scene(DEFout_scene, resout_Scene);

                    // create a mutable point iterator to change points of this cloud
                    CT_MutablePointIterator outItP(pcir);

                    while(itP.hasNext())
                    {
                        CT_Point point = itP.next().currentPoint();

                        // get the coordinate system of this point
                        CT_AbstractCoordinateSystem* currentSystem = itP.currentCoordinateSystem();

                        GLuint transSystem = 0;

                        // if the transformed coordinate system don't exist
                        if (!coordSysCorresp.contains(currentSystem))
                        {
                            // get the current offset
                            Eigen::Vector3d offset;
                            currentSystem->offset(offset);

                            // transform it
                            trMat->transform(offset);

                            // create the coordinate system transformed and get it's index
                            transSystem = (new CT_DefaultCoordinateSystem(offset, this))->indexInManager();

                            // and backup it
                            coordSysCorresp.insert(currentSystem, transSystem);
                        } else {
                            transSystem = coordSysCorresp.value(currentSystem, 0);
                        }

                        // transform the current point
                        trMat->transform(point);

                        // set it to the new point cloud
                        outItP.next().replaceCurrentPoint(point, transSystem);

                        if (point(0) < minX) {minX = point(0);}
                        if (point(1) < minY) {minY = point(1);}
                        if (point(2) < minZ) {minZ = point(2);}
                        if (point(0) > maxX) {maxX = point(0);}
                        if (point(1) > maxY) {maxY = point(1);}
                        if (point(2) > maxZ) {maxZ = point(2);}
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
