#include "pb_stepcomputecrownprojection.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_beam.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_rscene "rscene"
#define DEFin_grpsc "grpsc"
#define DEFin_scene "scene"



// Constructor : initialization of parameters
PB_StepComputeCrownProjection::PB_StepComputeCrownProjection(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _computeSlices = true;
    _spacing = 0.5;
    _thickness = 2;
    _zmin = std::numeric_limits<double>::max();
    _zmax = -std::numeric_limits<double>::max();

    _computeDirs = false;
    _nbDir = 8;
}

// Step description (tooltip of contextual menu)
QString PB_StepComputeCrownProjection::getStepDescription() const
{
    return tr("Projection de houppiers");
}

// Step detailled description
QString PB_StepComputeCrownProjection::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepComputeCrownProjection::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepComputeCrownProjection::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepComputeCrownProjection(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepComputeCrownProjection::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_rscene = createNewInResultModelForCopy(DEFin_rscene, tr("Scène(s)"));
    resIn_rscene->setZeroOrMoreRootGroup();
    resIn_rscene->addGroupModel("", DEFin_grpsc, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rscene->addItemModel(DEFin_grpsc, DEFin_scene, CT_Scene::staticGetType(), tr("Scène"));

}

// Creation and affiliation of OUT models
void PB_StepComputeCrownProjection::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_rscene = createNewOutResultModelToCopy(DEFin_rscene);

    resCpy_rscene->addItemModel(DEFin_grpsc, _convexHull_ModelName, new CT_Polygon2D(), tr("Enveloppe Convexe au sol"));
    resCpy_rscene->addItemModel(DEFin_grpsc, _directionalHull_ModelName, new CT_Polygon2D(), tr("Enveloppe Directionnelle au sol"));
    resCpy_rscene->addGroupModel(DEFin_grpsc, _grpSlice_ModelName, new CT_StandardItemGroup(), tr("Groupe"));

    resCpy_rscene->addItemModel(_grpSlice_ModelName, _scliceCvx_ModelName, new CT_Polygon2D(), tr("Enveloppe Convexe d'une tranche"));
    resCpy_rscene->addItemModel(_grpSlice_ModelName, _scliceDir_ModelName, new CT_Polygon2D(), tr("Enveloppe Directionnelle d'une tranche"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepComputeCrownProjection::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addBool(tr("Calculer les enveloppes convexes par tranches"), "", "", _computeSlices);
    configDialog->addDouble(tr("Espacement des tranches"), "m", 0, 1e+09, 2, _spacing, 1);
    configDialog->addDouble(tr("Epaisseur des tranches"), "m", 0, 1e+09, 2, _thickness, 1);
    configDialog->addBool(tr("Calculer les enveloppes directionnelles par tranches"), "", "", _computeDirs);
    configDialog->addInt(tr("Nombre de directions"), "", 3, 99999, _nbDir);
}

void PB_StepComputeCrownProjection::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    _rscene = outResultList.at(0);

    // parcours des scènes pour calculer z min et max
    QList<CT_StandardItemGroup*> groups;
    CT_ResultGroupIterator itSc(_rscene, this, DEFin_grpsc);
    while (itSc.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itSc.next();
        const CT_Scene* scene = (CT_Scene*)grp->firstItemByINModelName(this, DEFin_scene);
        if (scene != NULL)
        {
            Eigen::Vector3d min, max;
            scene->getBoundingBox(min, max);
            if (min(2) < _zmin) {_zmin = min(2);}
            if (max(2) > _zmax) {_zmax = max(2);}
            groups.append(grp);
        }
    }

    QListIterator<CT_StandardItemGroup*> itGrp(groups);
    while (itGrp.hasNext() && !isStopped())
    {
        computeConvexHullForOneSceneGroup(itGrp.next());
    }
}

void PB_StepComputeCrownProjection::computeConvexHullForOneSceneGroup(CT_StandardItemGroup* group) const
{
    const CT_Scene* scene = (CT_Scene*)group->firstItemByINModelName(this, DEFin_scene);
    if (scene != NULL)
    {
        // création des niveaux
        QList<Eigen::Vector2d *> allPoints;
        QList<PB_StepComputeCrownProjection::level> pointsByLevel;
        for (double z = _zmin ; z < _zmax ; z += _spacing)
        {
            pointsByLevel.append(PB_StepComputeCrownProjection::level(z - _thickness, z + _thickness, z));
        }

        QMap<Eigen::Vector2d*, double> zValues;
        // création de la liste complète des points
        CT_PointIterator itP(scene->getPointCloudIndex());
        while(itP.hasNext() && (!isStopped()))
        {
            const CT_Point &point = itP.next().currentPoint();
            Eigen::Vector2d *point2D = new Eigen::Vector2d(point(0), point(1));
            allPoints.append(point2D);
            zValues.insert(point2D, point(2));
        }

        // tri par (X,Y) de la liste des points
        CT_Polygon2DData::orderPointsByXY(allPoints);

        if (_computeSlices)
        {
            // Création de la liste des points pour chaque tranche
            QListIterator<Eigen::Vector2d *> itPoints(allPoints);
            while (itPoints.hasNext())
            {
                Eigen::Vector2d *point = itPoints.next();
                double z = zValues.value(point);

                QListIterator<PB_StepComputeCrownProjection::level> it(pointsByLevel);
                while (it.hasNext())
                {
                    PB_StepComputeCrownProjection::level &levelInfo = (PB_StepComputeCrownProjection::level &) it.next();
                    if (z >= levelInfo._zmin && z < levelInfo._zmax)
                    {
                        levelInfo._pointList.append(point);
                    }
                }
            }
        }

        CT_Polygon2DData *data = CT_Polygon2DData::createConvexHull(allPoints);
        CT_Polygon2D* convexHull = new CT_Polygon2D(_convexHull_ModelName.completeName(), _rscene, data);
        group->addItemDrawable(convexHull);

        // Calcul des angles limites
        double angle = 2*M_PI / (double)_nbDir;
        QMap<double, int> dirMax;

        dirMax.insert(angle / 2.0, 0);
        for (int i = 1 ; i < _nbDir ; i++)
        {
            dirMax.insert(i*angle + angle / 2.0, i);
        }
        dirMax.insert(2*M_PI + 0.1, 0);


        if (_computeSlices)
        {
            QListIterator<PB_StepComputeCrownProjection::level> itPtBLev(pointsByLevel);
            while (itPtBLev.hasNext())
            {
                PB_StepComputeCrownProjection::level &currentLevel = (PB_StepComputeCrownProjection::level&) itPtBLev.next();
                CT_Polygon2DData *dataSlice = CT_Polygon2DData::createConvexHull(currentLevel._pointList);

                if (dataSlice != NULL)
                {
                    CT_StandardItemGroup* grpSlice= new CT_StandardItemGroup(_grpSlice_ModelName.completeName(), _rscene);
                    group->addGroup(grpSlice);

                    CT_Polygon2D* slice = new CT_Polygon2D(_scliceCvx_ModelName.completeName(), _rscene, dataSlice);
                    slice->setZValue(currentLevel._zlevel);

                    grpSlice->addItemDrawable(slice);

                    if (_computeDirs)
                    {
                        // Calcul de l'enveloppe directionnelle
                        const Eigen::Vector2d &massCenter = dataSlice->getCenter();

                        QMap<double, double> azToDist;
                        QListIterator<Eigen::Vector2d*> itPts(currentLevel._pointList);
                        while (itPts.hasNext())
                        {
                            Eigen::Vector2d* pt = itPts.next();
                            Eigen::Vector2d dir = (*pt) - massCenter;

                            double distance = dir.norm();
                            double asinx = asin(dir(0) / distance);
                            double acosy = acos(dir(1) / distance);
                            double azimut;
                            if (asinx >= 0) {
                                azimut = acosy;
                            } else {
                                azimut = 2*M_PI - acosy;
                            }
                            azToDist.insert(azimut, distance);
                        }


                    }
                }
            }
        }


        qDeleteAll(allPoints);
    }
}
