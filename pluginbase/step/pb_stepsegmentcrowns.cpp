#include "pb_stepsegmentcrowns.h"

// Inclusion of in models
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_triangulation/ct_nodet.h"
#include "ct_itemdrawable/ct_metrict.h"

#include <limits>
#include <QMessageBox>
#include <QDebug>

// Alias for indexing in models
#define DEF_resultIn "result"
#define DEF_GroupScene "grp"
#define DEF_itemScene "scene"

#define DEF_resultOut "result"
#define DEF_SearchOutGroup "group"
#define DEF_SearchOutDensityGrid "density"
#define DEF_SearchOutMNSGrid "mns"
#define DEF_SearchOutClustersGrid "cluster"
#define DEF_SearchOutScene "outscene"
#define DEF_SearchOutConvexHull "hull"
#define DEF_SearchOutGroupScene "gscene"

#define DEF_SearchOutCrownArea "crarea"
#define DEF_SearchOutConvexCrownArea "cvxcrarea"
#define DEF_SearchOutZmax "zmax"

// Constructor : initialization of parameters
PB_StepSegmentCrowns::PB_StepSegmentCrowns(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepSegmentCrowns::getStepDescription() const
{
    return "Segment les houppiers";
}

// Step description (tooltip of contextual menu)
QString PB_StepSegmentCrowns::getStepDetailledDescription() const
{
    return tr("Cette étape permet de segmenter des houppiers manuellement dans un plan horizontal.<br>"
              "Une première phase manuelle permet de déteminer la tranche verticale sur laquelle les points seront analysés. "
              "On y définit un niveau Z minimum, un niveau Z maximum, ainsi qu'une résolution pour les rasters de travail :"
              "<ul>"
              "<li>Le Modèle Numérique de Surface (MNS) = hauteur du point le plus haut pour chaque case</li>"
              "<li>La densité de point par case</li>"
              "</ul>"
              "Les rasters sont ensuite utilisés dans une seconde phase, afin de segmenter les houppiers. "
              "Un système par couleurs permet de façon semi-automatique de délimiter l'emprise horizontale de chaque houppier. "
              "Sur la base d'une pré-segmentation, l'opérateur peut modifier les groupes (houppiers) en les fusionnant ou en les divisant.<br>"
              "En sortie, cette étapes produit :"
              "<ul>"
              "<li>Un raster avec une valeur entière différente pour chaque houppier</li>"
              "<li>Une scène de points extraite pour chaque houppier</li>"
              "<li>Un polygone horizontal correspondant à l'enveloppe convexe des pixels de chaque houppier</li>"
              "<li>Deux métriques donnant respectivement la surface des pixels, et la surface de l'enveloppe convexe pour chaque houppier</li>"
              "</ul>");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSegmentCrowns::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSegmentCrowns(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSegmentCrowns::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultModel = createNewInResultModel(DEF_resultIn, tr("Scène(s)"));

    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_GroupScene);
    resultModel->addItemModel(DEF_GroupScene, DEF_itemScene, CT_Scene::staticGetType(), tr("Scène"));
}

// Creation and affiliation of OUT models
void PB_StepSegmentCrowns::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *resultModel = createNewOutResultModel(DEF_resultOut, tr("Densité, MNS et clusters"));

    resultModel->setRootGroup(DEF_SearchOutGroup);
    resultModel->addItemModel(DEF_SearchOutGroup, DEF_SearchOutMNSGrid, new CT_Grid2DXY<float>(), tr("MNS"));
    resultModel->addItemModel(DEF_SearchOutGroup, DEF_SearchOutDensityGrid, new CT_Grid2DXY<int>(), tr("Densité"));
    resultModel->addItemModel(DEF_SearchOutGroup, DEF_SearchOutClustersGrid, new CT_Grid2DXY<int>(), tr("Clusters"));

    resultModel->addGroupModel(DEF_SearchOutGroup, DEF_SearchOutGroupScene);
    resultModel->addItemModel(DEF_SearchOutGroupScene, DEF_SearchOutScene, new CT_Scene(), tr("Scènes segmentées"));
    resultModel->addItemModel(DEF_SearchOutGroupScene, DEF_SearchOutConvexHull, new CT_Polygon2D(), tr("ConvexHull"));
    resultModel->addItemModel(DEF_SearchOutGroupScene, DEF_SearchOutCrownArea, new CT_MetricT<float>(), tr("Aire du houppier"));
    resultModel->addItemModel(DEF_SearchOutGroupScene, DEF_SearchOutConvexCrownArea, new CT_MetricT<float>(), tr("Aire du houppier convexe"));
    resultModel->addItemModel(DEF_SearchOutGroupScene, DEF_SearchOutZmax, new CT_MetricT<float>(), tr("Z max"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSegmentCrowns::createPostConfigurationDialog()
{
    //    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    //    configDialog->addDouble(tr("Résolution de la grille"),tr("m"),0.0001,10000,2, _res );
}


void PB_StepSegmentCrowns::compute()
{
    _gridContainer = new PB_ActionDefineHeightLayer_gridContainer();

    m_doc = NULL;

    CT_ResultGroup *resultIn = getInputResults().first();

    _outResult = getOutResultList().at(0);

    m_itemDrawableToAdd.clear();

    _xmin = std::numeric_limits<float>::max();
    _ymin = std::numeric_limits<float>::max();
    _zmin = std::numeric_limits<float>::max();

    _xmax = -std::numeric_limits<float>::max();
    _ymax = -std::numeric_limits<float>::max();
    _zmax = -std::numeric_limits<float>::max();

    _clustersGrid = NULL;


    CT_ResultItemIterator itRes(resultIn, this, DEF_itemScene);
    while(!isStopped() && itRes.hasNext())
    {
        CT_Scene *scene = (CT_Scene*) itRes.next();
        _sceneList.append(scene);

        if (scene->minX() < _xmin) {_xmin = scene->minX();}
        if (scene->minY() < _ymin) {_ymin = scene->minY();}
        if (scene->minZ() < _zmin) {_zmin = scene->minZ();}

        if (scene->maxX() > _xmax) {_xmax = scene->maxX();}
        if (scene->maxY() > _ymax) {_ymax = scene->maxY();}
        if (scene->maxZ() > _zmax) {_zmax = scene->maxZ();}

        m_itemDrawableToAdd.append(scene);
    }

    if (!m_itemDrawableToAdd.isEmpty())
    {
        PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Selection de l'épaisseur des houppiers"));

        m_status = 0;
        requestManualMode();

        CT_StandardItemGroup *group = new CT_StandardItemGroup(DEF_SearchOutGroup, _outResult);

        if (_gridContainer->_densityGrid!=NULL)
        {
            group->addItemDrawable(_gridContainer->_densityGrid);
        }

        if (_gridContainer->_mnsGrid!=NULL)
        {
            group->addItemDrawable(_gridContainer->_mnsGrid);

            _clustersGrid = new CT_Grid2DXY<int>(DEF_SearchOutClustersGrid, _outResult,
                                                 _gridContainer->_mnsGrid->minX(),
                                                 _gridContainer->_mnsGrid->minY(),
                                                 _gridContainer->_mnsGrid->colDim(),
                                                 _gridContainer->_mnsGrid->linDim(),
                                                 _gridContainer->_mnsGrid->resolution(),
                                                 _gridContainer->_mnsGrid->level(),
                                                 -1, -1);
        }
        _outResult->addGroup(group);

        if (_gridContainer->_densityGrid!=NULL &&
                _gridContainer->_mnsGrid!=NULL &&
                _clustersGrid!=NULL)
        {
            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Création des clusters"));

            m_status = 1;
            requestManualMode();

            group->addItemDrawable(_clustersGrid);
            _zmin = _gridContainer->_zmin;
            _zmax = _gridContainer->_zmax;

            m_status = 2;
            requestManualMode();


            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Démarrage des post_traitements"));
            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Elimination des clusters vides"));
            int newClusterNumber = dropEmptyClusters();


            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Création nuages de points par cluster"));
            QMap<int, CT_PointCloudIndexVector*> indexVectorMap;
            QMap<int, CT_StandardItemGroup*> sceneGroupMap;

            // Creation des scènes extraites et des groupes contenant les scènes
            for (int i = 0 ; i < newClusterNumber ; i++)
            {
                CT_PointCloudIndexVector *pointCloudIndex = new CT_PointCloudIndexVector();
                indexVectorMap.insert(i, pointCloudIndex);

                CT_StandardItemGroup *groupScene = new CT_StandardItemGroup(DEF_SearchOutGroupScene, _outResult);
                group->addGroup(groupScene);
                sceneGroupMap.insert(i, groupScene);
            }

            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Ajout des points aux scènes par cluster"));
            addPointsToExtractedScenes(resultIn, indexVectorMap);
            registerScenes(indexVectorMap, sceneGroupMap);


            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Enregistrement des clusters"));
            QMap<int, QList<QVector2D*>*> cellsMapByCluster;
            QMap<int, size_t> clusterCounts;
            QMap<int, float> clusterZMax;
            registerClusterCells(cellsMapByCluster, clusterCounts, clusterZMax);


            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Création des Convex Hulls"));
            QMap<int, CT_Polygon2DData*> convexHullsMap;
            createConvexHulls(cellsMapByCluster, sceneGroupMap, convexHullsMap);


            PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Calcul des métriques"));
            computeMetrics(sceneGroupMap,
                           convexHullsMap,
                           clusterCounts,
                           clusterZMax);

            QMapIterator<int, QList<QVector2D *> *> it(cellsMapByCluster);
            while (it.hasNext())
            {
                it.next();
                QList<QVector2D*> *cells = it.value();

                qDeleteAll(*cells);
                delete cells;
            }

        } else {
            QMessageBox::information(NULL, tr("Segmentation des houppiers"), tr("Phase 2 (Segmentation des houppiers) impossible à réaliser."), QMessageBox::Ok);
            m_status = 2;
            requestManualMode();
        }

        PS_LOG->addMessage(LogInterface::info, LogInterface::step,tr("Post-Traitements terminés"));
    }

    delete _gridContainer;
}

int PB_StepSegmentCrowns::dropEmptyClusters()
{
    // Cluster 0 : réservé (à ne pas prendre en compte)
    int newClusterNumber = 1;

    QMap<int, int> clustersMap;
    size_t ncells = _clustersGrid->nCells();
    for (size_t i = 0 ; i < ncells ; i++)
    {
        int value = _clustersGrid->valueAtIndex(i);
        if (value > 0 && !clustersMap.contains(value))
        {
            clustersMap.insert(value, newClusterNumber++);
        }
    }

    for (size_t i = 0 ; i < ncells ; i++)
    {
        int value = _clustersGrid->valueAtIndex(i);
        if (value > 0)
        {
            _clustersGrid->setValueAtIndex(i, clustersMap.value(value, -1));
        }
    }
    clustersMap.clear();

    // Calcul des limites
    _clustersGrid->computeMinMax();
    return newClusterNumber;
}

void PB_StepSegmentCrowns::addPointsToExtractedScenes(CT_ResultGroup *resultIn,
                                                      const QMap<int, CT_PointCloudIndexVector*> &indexVectorMap)
{
    CT_ResultItemIterator itRes(resultIn, this, DEF_itemScene);
    while(!isStopped() && itRes.hasNext())
    {
        CT_Scene *scene = (CT_Scene*) itRes.next();

        const CT_AbstractPointCloudIndex *pointCloudIndex = scene->getPointCloudIndex();
        size_t n_points = pointCloudIndex->size();

        size_t i = 0;
        while((i<n_points) && (!isStopped()))
        {
            size_t index;
            const CT_Point &point = pointCloudIndex->constTAt(i, index);

            if (point.getZ() >= _zmin && point.getZ() <= _zmax)
            {
                int cluster = _clustersGrid->valueAtXY(point.getX(), point.getY());

                if (cluster >= 0)
                {
                    CT_PointCloudIndexVector* outPointCloudIndex = indexVectorMap.value(cluster, NULL);
                    if (outPointCloudIndex != NULL)
                    {
                        outPointCloudIndex->addIndex(index);
                    }
                }
            }
            ++i;
        }
    }
}

void PB_StepSegmentCrowns::registerScenes(const QMap<int, CT_PointCloudIndexVector*> &indexVectorMap,
                                          const QMap<int, CT_StandardItemGroup*> &sceneGroupMap)
{
    QMapIterator<int, CT_PointCloudIndexVector*> it(indexVectorMap);
    while (it.hasNext())
    {
        it.next();

        int cluster = it.key();
        CT_PointCloudIndexVector* indexVector = it.value();
        CT_StandardItemGroup* groupScene = sceneGroupMap.value(cluster);

        CT_Scene *scene = new CT_Scene(DEF_SearchOutScene, _outResult);
        scene->setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(indexVector));
        scene->updateBoundingBox();

        groupScene->addItemDrawable(scene);
    }
}

void PB_StepSegmentCrowns::registerClusterCells(QMap<int, QList<QVector2D*> *> &cellsMapByCluster, QMap<int, size_t> &clusterCounts, QMap<int, float> &clusterZMax)
{
    for (size_t c = 0 ; c < _clustersGrid->colDim() ; c++)
    {
        for (size_t l = 0 ; l < _clustersGrid->linDim() ; l++)
        {
            QVector2D* point = new QVector2D(_clustersGrid->getCellCenterColCoord(c), _clustersGrid->getCellCenterLinCoord(l));

            int cluster = _clustersGrid->value(c, l);
            float z = _gridContainer->_mnsGrid->value(c, l);

            if (cluster >= 0)
            {
                clusterCounts.insert(cluster, clusterCounts.value(cluster, 0) + 1);

                QList<QVector2D*> *liste = cellsMapByCluster.value(cluster, NULL);
                if (liste == NULL)
                {
                    liste = new QList<QVector2D*>();
                    cellsMapByCluster.insert(cluster, liste);
                }
                liste->append(point);

                float zMax = clusterZMax.value(cluster, -std::numeric_limits<float>::max());
                if (z > zMax) {clusterZMax.insert(cluster, z);}
            }
        }
    }
}

void PB_StepSegmentCrowns::createConvexHulls(QMap<int, QList<QVector2D *> *> &cellsMapByCluster,
                                             const QMap<int, CT_StandardItemGroup*> &sceneGroupMap,
                                             QMap<int, CT_Polygon2DData*> &convexHullsMap)
{
    QMapIterator<int, QList<QVector2D *> *> it(cellsMapByCluster);
    while (it.hasNext())
    {
        it.next();
        int cluster = it.key();
        QList<QVector2D*> *cells = it.value();
        //CT_Polygon2DData::orderPointsByXY(cells); // Normalement inutile
        CT_Polygon2DData* polygonData = CT_Polygon2DData::createConvexHull(*cells);

        if (polygonData!=NULL)
        {
            convexHullsMap.insert(cluster, polygonData);
            CT_StandardItemGroup* groupScene = sceneGroupMap.value(cluster);
            CT_Polygon2D *convexHull = new CT_Polygon2D(DEF_SearchOutConvexHull, _outResult, polygonData);
            groupScene->addItemDrawable(convexHull);
        }
    }
}


void PB_StepSegmentCrowns::computeMetrics(const QMap<int, CT_StandardItemGroup*> &sceneGroupMap,
                                          const QMap<int, CT_Polygon2DData*> &convexHullsMap,
                                          const QMap<int, size_t> &clusterCounts,
                                          const QMap<int, float> &clusterZMax)
{
    QMap<int, size_t> clusterConvexCounts;

    // Calcul des aires convexes (en cellules)
    size_t sizeGrd = _clustersGrid->nCells();
    for (size_t i = 0 ; i < sizeGrd ; i++)
    {
        size_t col, row;
        _clustersGrid->indexToGrid(i, col, row);
        float x = _clustersGrid->getCellCenterColCoord(col);
        float y = _clustersGrid->getCellCenterLinCoord(row);
        int cluster = _clustersGrid->valueAtXY(x, y);

        QMapIterator<int, CT_Polygon2DData*> it_convexHullMap(convexHullsMap);
        while (it_convexHullMap.hasNext())
        {
            it_convexHullMap.next();
            int hullCluster = it_convexHullMap.key();
            CT_Polygon2DData* polygonData = it_convexHullMap.value();

            if ((polygonData != NULL && polygonData->contains(x, y)) || (hullCluster == cluster))
            {
                clusterConvexCounts.insert(hullCluster, clusterConvexCounts.value(hullCluster, 0) + 1);
            }
        }
    }

    // aire d'une cellule
    float base_area = _clustersGrid->resolution()*_clustersGrid->resolution();

    QMapIterator<int, CT_StandardItemGroup*> it(sceneGroupMap);
    while (it.hasNext())
    {
        it.next();
        int cluster = it.key();
        CT_StandardItemGroup* groupScene = it.value();

        float crownArea = clusterCounts.value(cluster, 0)*base_area;
        float convexHullArea = clusterConvexCounts.value(cluster, 0)*base_area;
        float zMax = clusterZMax.value(cluster, std::numeric_limits<float>::quiet_NaN());

        CT_MetricT<float> *metric1 = new CT_MetricT<float>(DEF_SearchOutCrownArea, _outResult, crownArea);
        CT_MetricT<float> *metric2 = new CT_MetricT<float>(DEF_SearchOutConvexCrownArea, _outResult, convexHullArea);
        CT_MetricT<float> *metric3 = new CT_MetricT<float>(DEF_SearchOutZmax, _outResult, zMax);
        groupScene->addItemDrawable(metric1);
        groupScene->addItemDrawable(metric2);
        groupScene->addItemDrawable(metric3);
    }
}


void PB_StepSegmentCrowns::initManualMode()
{
    if(m_doc == NULL)
    {
        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument();
    }

    m_doc->removeAllItemDrawable();

    QListIterator<CT_Scene*> it(m_itemDrawableToAdd);

    while(it.hasNext())
    {
        m_doc->addItemDrawable(*it.next());
    }

}

void PB_StepSegmentCrowns::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(!quit)
        {

            QMessageBox::information(NULL, tr("Segmentation des houppiers"), tr("Mode manuel.\n"
                                                                                "Phase 1 : Calcul de la carte de densité et du MNS.\n"
                                                                                "L'initialisation peut prendre un peu de temps..."), QMessageBox::Ok);

            _action = new PB_ActionDefineHeightLayer(_outResult,
                                                     DEF_SearchOutDensityGrid,
                                                     DEF_SearchOutMNSGrid,
                                                     _gridContainer,
                                                     _sceneList,
                                                     _xmin, _ymin, _zmin, _xmax, _ymax, _zmax);

            m_doc->setCurrentAction(_action, false);
        }
    }else if(m_status == 1)
    {
        if(!quit)
        {
            if (_action!= NULL) {delete _action;}

            getGuiContext()->actionsManager()->removeAction("PB_ActionDefineHeightLayer");

            QMessageBox::information(NULL, tr("Segmentation des houppiers"), tr("Mode manuel.\n"
                                                                                "Phase 2 : Segmentation des houppiers.\n"
                                                                                "La pré-segmentation automatique peut prendre un peu de temps..."), QMessageBox::Ok);

            _action = new PB_ActionSegmentCrowns(_gridContainer->_densityGrid,
                                                 _gridContainer->_mnsGrid,
                                                 _clustersGrid);

            m_doc->setCurrentAction(_action);
            _action = NULL;
        }
    }
    else if(m_status == 2)
    {
        if(!quit)
        {
            m_doc = NULL;

            quitManualMode();

            QMessageBox::information(NULL, tr("Segmentation des houppiers"), tr("Fin du mode manuel, démarrage des post-traitements\n"
                                                                                "Cela peut prendre un peu de temps..."), QMessageBox::Ok);
        }
    }
}
