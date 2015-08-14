#include "pb_stepdetectverticalalignments.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_itemdrawable/ct_line.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_iterator/ct_pointiterator.h"
#include "ct_math/ct_sphericalline3d.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_scene "scene"



// Constructor : initialization of parameters
PB_StepDetectVerticalAlignments::PB_StepDetectVerticalAlignments(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _maxAngle = 30.0;
    _distThreshold = 4.0;
    _lineDistThreshold = 1.5;
    _minPtsNb = 3;
    _lengthThreshold = 0.2;
}

// Step description (tooltip of contextual menu)
QString PB_StepDetectVerticalAlignments::getStepDescription() const
{
    return tr("Détecte des alignements verticaux");
}

// Step detailled description
QString PB_StepDetectVerticalAlignments::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepDetectVerticalAlignments::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepDetectVerticalAlignments::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepDetectVerticalAlignments(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepDetectVerticalAlignments::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Scènes"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Scènes (grp)"));
    resIn_res->addItemModel(DEFin_grp, DEFin_scene, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène"));

}

// Creation and affiliation of OUT models
void PB_StepDetectVerticalAlignments::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    resCpy_res->addGroupModel(DEFin_grp, _grpCluster_ModelName, new CT_StandardItemGroup(), tr("Clusters"));
    resCpy_res->addItemModel(_grpCluster_ModelName, _cluster_ModelName, new CT_PointCluster(), tr("Cluster"));
    resCpy_res->addItemModel(_grpCluster_ModelName, _line_ModelName, new CT_Line(), tr("Ligne"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepDetectVerticalAlignments::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Angle zénithal maximal"), "°", 0, 180, 2, _maxAngle);
    configDialog->addDouble(tr("Distance maximum entre deux points d'une ligne"), "m", 0, 1000, 2, _distThreshold);
    configDialog->addDouble(tr("Distance maximum XY entre deux lignes"), "m", 0, 1000, 2, _lineDistThreshold);
    configDialog->addInt(tr("Nombre de points minimum dans un cluster"), "", 3, 1000, _minPtsNb);
    configDialog->addDouble(tr("Longueur relative minimale des segments"), "%", 0, 100, 0, _lengthThreshold, 100);
}

void PB_StepDetectVerticalAlignments::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res = outResultList.at(0);

    double maxAngleRadians = M_PI*_maxAngle/180.0;

    QList<PB_StepDetectVerticalAlignments::LineData*> candidateLines;

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractItemDrawableWithPointCloud* scene = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_scene);
        if (scene != NULL)
        {
            const CT_AbstractPointCloudIndex* pointCloudIndex = scene->getPointCloudIndex();

            double deltaZ = scene->maxZ() - scene->minZ();

            double lengthThreshold = _lengthThreshold*deltaZ;


            // Parcours tous les couples de points 2 à deux
            CT_PointIterator itP1(pointCloudIndex);
            while(itP1.hasNext() && (!isStopped()))
            {
                const CT_Point &point1 = itP1.next().currentPoint();
                size_t index1 = itP1.currentGlobalIndex();

                bool before = true;
                CT_PointIterator itP2(pointCloudIndex);
                while(itP2.hasNext() && (!isStopped()))
                {
                    const CT_Point &point2 = itP2.next().currentPoint();
                    size_t index2 = itP2.currentGlobalIndex();

                    // Les deux points doivent être disctinct dans l'espace
                    if (!before && ((point1(0) != point2(0) || point1(1) != point2(1) || point1(2) != point2(2))))
                    {
                        // Les deux points doivent être à moins de _distThreshold
                        double dist = sqrt(pow(point1(0) - point2(0), 2) + pow(point1(1) - point2(1), 2) + pow(point1(2) - point2(2), 2));
                        if (dist < _distThreshold)
                        {
                            Eigen::Vector3d pointLow  = point1;
                            Eigen::Vector3d pointHigh = point2;

                            if (point2(2) < point1(2))
                            {
                                pointLow  = point2;
                                pointHigh = point1;
                            }

                            float phi, theta, length;
                            CT_SphericalLine3D::convertToSphericalCoordinates(&pointLow, &pointHigh, phi, theta, length);

                            if (phi < maxAngleRadians)
                            {
                                candidateLines.append(new PB_StepDetectVerticalAlignments::LineData(pointLow, pointHigh, index1, index2, phi, scene->minZ(), scene->maxZ()));
                            }
                        }
                    }

                    if (index1 == index2) {before = false;}
                }
            }

            // Tri par Phi croissant
            qSort(candidateLines.begin(), candidateLines.end(), PB_StepDetectVerticalAlignments::lessThan);


            // Affiliation des lignes proches deux à deux
            QMultiMap<PB_StepDetectVerticalAlignments::LineData*, PB_StepDetectVerticalAlignments::LineData*> linePairs;

            for (int i1 = 0 ; i1 < candidateLines.size() ; i1++)
            {
                PB_StepDetectVerticalAlignments::LineData* line1 = candidateLines.at(i1);

                for (int i2 = i1+1 ; i2 < candidateLines.size() ; i2++)
                {
                    PB_StepDetectVerticalAlignments::LineData* line2 = candidateLines.at(i2);

                    double distLow = sqrt(pow(line1->_lowCoord(0) - line2->_lowCoord(0), 2) + pow(line1->_lowCoord(1) - line2->_lowCoord(1), 2));

                    if (distLow < _lineDistThreshold)
                    {
                        double distHigh = sqrt(pow(line1->_highCoord(0) - line2->_highCoord(0), 2) + pow(line1->_highCoord(1) - line2->_highCoord(1), 2));

                        if (distHigh < _lineDistThreshold)
                        {
                            double maxDist = 0;

                            if (line1->_pLow(2) > line2->_pHigh(2))
                            {
                                maxDist = sqrt(pow(line1->_pLow(0) - line2->_pHigh(0), 2) + pow(line1->_pLow(1) - line2->_pHigh(1), 2)+ pow(line1->_pLow(2) - line2->_pHigh(2), 2));
                            } else if (line2->_pLow(2) > line1->_pHigh(2))
                            {
                                maxDist = sqrt(pow(line2->_pLow(0) - line1->_pHigh(0), 2) + pow(line2->_pLow(1) - line1->_pHigh(1), 2)+ pow(line2->_pLow(2) - line1->_pHigh(2), 2));
                            }

                            if (maxDist < _distThreshold)
                            {
                                linePairs.insert(line1, line2);
                                linePairs.insert(line2, line1);
                            }
                        }
                    }
                }
            }



            // Constitution des clusters de points alignés
            QMultiMap<size_t, QList<size_t>* > pointsClusters;
            QList<PB_StepDetectVerticalAlignments::LineData*> attributedLines;
            QList<size_t> insertedPoints;

            for (int i = 0 ; i < candidateLines.size() ; i++)
            {
                PB_StepDetectVerticalAlignments::LineData* candidateLine = candidateLines.at(i);

                if (!attributedLines.contains(candidateLine))
                {
                    QList<size_t>* indexList = new QList<size_t>();

                    if (!insertedPoints.contains(candidateLine->_index1) && !indexList->contains(candidateLine->_index1))
                    {
                        indexList->append(candidateLine->_index1);
                        insertedPoints.append(candidateLine->_index1);
                    }
                    if (!insertedPoints.contains(candidateLine->_index2) && !indexList->contains(candidateLine->_index2))
                    {
                        indexList->append(candidateLine->_index2);
                        insertedPoints.append(candidateLine->_index2);
                    }
                    attributedLines.append(candidateLine);

                    QList<PB_StepDetectVerticalAlignments::LineData*> validLines = linePairs.values(candidateLine);
                    for (int j = 0 ; j < validLines.size() ; j++)
                    {
                        PB_StepDetectVerticalAlignments::LineData* validLine = validLines.at(j);
                        if (!attributedLines.contains(validLine))
                        {
                            if (!insertedPoints.contains(validLine->_index1) && !indexList->contains(validLine->_index1))
                            {
                                indexList->append(validLine->_index1);
                                insertedPoints.append(validLine->_index1);
                            }
                            if (!insertedPoints.contains(validLine->_index2) && !indexList->contains(validLine->_index2))
                            {
                                indexList->append(validLine->_index2);
                                insertedPoints.append(validLine->_index2);
                            }
                            attributedLines.append(validLine);
                        }
                    }

                    pointsClusters.insert(indexList->size(), indexList);
                }
            }


            qDeleteAll(candidateLines);
            candidateLines.clear();
            linePairs.clear();
            attributedLines.clear();
            insertedPoints.clear();

            // Création des pointClusters
            QMapIterator<size_t, QList<size_t>* > itCl(pointsClusters);
            itCl.toBack();
            while (itCl.hasPrevious())
            {
                itCl.previous();
                QList<size_t>* list = itCl.value();

                if (list->size() >= _minPtsNb)
                {
                    CT_StandardItemGroup* grpCl = new CT_StandardItemGroup(_grpCluster_ModelName.completeName(), res);

                    CT_PointCluster* cluster = new CT_PointCluster(_cluster_ModelName.completeName(), res);

                    QListIterator<size_t> itPts(*list);
                    while (itPts.hasNext())
                    {
                        size_t index = itPts.next();
                        cluster->addPoint(index);
                    }

                    const CT_AbstractPointCloudIndex* cloudIndex = cluster->getPointCloudIndex();
                    CT_LineData* lineData = CT_LineData::staticCreateLineDataFromPointCloud(*cloudIndex);


                    if (lineData != NULL)
                    {
                        Eigen::Vector3d pointLow  = lineData->getP1();
                        Eigen::Vector3d pointHigh = lineData->getP2();

                        if (lineData->getP1()(2) < lineData->getP2()(2))
                        {
                            pointLow  = lineData->getP2();
                            pointHigh = lineData->getP1();
                        }

                        float phi, theta, length;
                        CT_SphericalLine3D::convertToSphericalCoordinates(&pointLow, &pointHigh, phi, theta, length);

                        if (phi < _maxAngle && lineData->length() > lengthThreshold)
                        {
                            CT_Line* line = new CT_Line(_line_ModelName.completeName(), res, lineData);

                            grp->addGroup(grpCl);
                            grpCl->addItemDrawable(cluster);
                            grpCl->addItemDrawable(line);
                        } else {
                            delete cluster;
                            delete grpCl;
                            delete lineData;
                        }

                    } else {
                        delete cluster;
                        delete grpCl;
                        qDebug() << "Problème";
                    }
                }
                delete list;
            }
        }       
    }

}
