#include "pb_stepfiltermaximabyclusterpositions.h"

#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_itemdrawable/ct_image2d.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_image "image"
#define DEFin_maxima "maxima"
#define DEFin_grpPos "grpPos"
#define DEFin_position "pos"

// Constructor : initialization of parameters
PB_StepFilterMaximaByClusterPositions::PB_StepFilterMaximaByClusterPositions(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepFilterMaximaByClusterPositions::getStepDescription() const
{
    return tr("Filtre les maxima par des rayons d'exclusion");
}

// Step detailled description
QString PB_StepFilterMaximaByClusterPositions::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepFilterMaximaByClusterPositions::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepFilterMaximaByClusterPositions::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepFilterMaximaByClusterPositions(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepFilterMaximaByClusterPositions::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Maxima"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_image, CT_Image2D<float>::staticGetType(), tr("Image (hauteurs)"));
    resIn_res->addItemModel(DEFin_grp, DEFin_maxima, CT_Image2D<qint32>::staticGetType(), tr("Maxima"));

    resIn_res->addGroupModel(DEFin_grp, DEFin_grpPos, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grpPos, DEFin_position, CT_PointCluster::staticGetType(), tr("Cluster Position"));
}

// Creation and affiliation of OUT models
void PB_StepFilterMaximaByClusterPositions::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);
    resCpy_res->addItemModel(DEFin_grp, _filteredMaxima_ModelName, new CT_Image2D<qint32>(), tr("Maxima filtrés"));

    resCpy_res->addItemModel(DEFin_grpPos, _attMaximaItem_ModelName, new CT_AttributesList(), tr("MaximaID"));
    resCpy_res->addItemAttributeModel(_attMaximaItem_ModelName, _attMaxima_ModelName, new CT_StdItemAttributeT<qint32>(CT_AbstractCategory::DATA_ID), tr("MaximaID"));
    resCpy_res->addItemAttributeModel(_attMaximaItem_ModelName, _attClusterID_ModelName, new CT_StdItemAttributeT<quint64>(CT_AbstractCategory::DATA_ID), tr("PointClusterID"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepFilterMaximaByClusterPositions::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice(tr("Rayons de recherche"),CT_FileChoiceButton::OneExistingFile , "Fichier de paramètres (*.*)", _fileNameSearchRadii);
    configDialog->addFileChoice(tr("Rauons d'exclusion"),CT_FileChoiceButton::OneExistingFile , "Fichier de paramètres (*.*)", _fileNameExclusionRadii);
}

void PB_StepFilterMaximaByClusterPositions::readRadii(QString fileName, QMap<double, double> &radii)
{
    QFile parameterFile(fileName);
    if (parameterFile.exists() && parameterFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&parameterFile);

        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if (!line.isEmpty())
            {
                QStringList values = line.split("\t");

                if (values.size() > 1)
                {
                    bool ok1, ok2;
                    double height = values.at(0).toDouble(&ok1);
                    double radius = values.at(1).toDouble(&ok2);

                    if (ok1 && ok2)
                    {
                        radii.insert(height, radius);
                    }
                }
            }
        }
        parameterFile.close();
    }

    if (!radii.contains(0)) {radii.insert(0, radii.first());}
    radii.insert(std::numeric_limits<double>::max(), radii.last());
}

void PB_StepFilterMaximaByClusterPositions::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res = outResultList.at(0);


    QMap<double, double> searchRadii;
    if (_fileNameSearchRadii.size() > 0) {readRadii(_fileNameSearchRadii.first(), searchRadii);}

    QMap<double, double> exclusionRadii;
    if (_fileNameExclusionRadii.size() > 0) {readRadii(_fileNameExclusionRadii.first(), exclusionRadii);}


    CT_ResultGroupIterator itCpy_grp(res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        CT_Image2D<qint32>* maximaIn = (CT_Image2D<qint32>*)grp->firstItemByINModelName(this, DEFin_maxima);
        CT_Image2D<float>* imageIn = (CT_Image2D<float>*)grp->firstItemByINModelName(this, DEFin_image);

        if (maximaIn != NULL)
        {                        
            // List of positions
            QList<CT_PointCluster*> positions;
            QMap<CT_PointCluster*, CT_StandardItemGroup*> positionsGroups;
            CT_GroupIterator itCpy_grpPos(grp, this, DEFin_grpPos);
            while (itCpy_grpPos.hasNext() && !isStopped())
            {
                CT_StandardItemGroup* grpPos = (CT_StandardItemGroup*) itCpy_grpPos.next();
                CT_PointCluster* position = (CT_PointCluster*) grpPos->firstItemByINModelName(this, DEFin_position);
                if (position != NULL)
                {
                    positions.append(position);
                    positionsGroups.insert(position, grpPos);
                }
            }


            Eigen::Vector2d min;
            maximaIn->getMinCoordinates(min);
            CT_Image2D<qint32>* filteredMaxima = new CT_Image2D<qint32>(_filteredMaxima_ModelName.completeName(), res, min(0), min(1), maximaIn->colDim(), maximaIn->linDim(), maximaIn->resolution(), maximaIn->level(), maximaIn->NA(), 0);
            grp->addItemDrawable(filteredMaxima);

            filteredMaxima->getMat() = maximaIn->getMat().clone();

            setProgress(20);

            // Get maxima coordinates list
            QMultiMap<qint32, Eigen::Vector3d*> maximaCoords;
            QMultiMap<double, qint32> maximaHeights;

            for (size_t xx = 0 ; xx < maximaIn->colDim() ; xx++)
            {
                for (size_t yy = 0 ; yy < maximaIn->linDim() ; yy++)
                {
                    qint32 maximaID = maximaIn->value(xx, yy);

                    if (maximaID > 0 && maximaID != maximaIn->NA())
                    {
                        Eigen::Vector3d* coords = new Eigen::Vector3d();
                        if (maximaIn->getCellCenterCoordinates(xx, yy, *coords))
                        {
                            (*coords)(2) = imageIn->value(xx, yy);
                            maximaCoords.insert(maximaID, coords);
                            maximaHeights.insert((*coords)(2), maximaID);
                        }
                    }
                }
            }

            setProgress(25);


            // Compute ordered vector of maxima ids
            QList<qint32> validMaxima;

            QMapIterator<double, qint32> itH(maximaHeights);
            itH.toBack();
            while (itH.hasPrevious())
            {
                itH.previous();
                qint32 cl = itH.value();
                if (!validMaxima.contains(cl)) {validMaxima.append(cl);}
            }

            QVector<qint32> orderedMaxima = validMaxima.toVector();
            int mxSize = orderedMaxima.size();
            validMaxima.clear();

            // Create maxima coords vector
            QVector<Eigen::Vector3d> coords(mxSize);
            for (int i = 0 ; i < mxSize ; i++)
            {
                qint32 id = orderedMaxima.at(i);

                QList<Eigen::Vector3d*> coordinates = maximaCoords.values(id);
                coords[i] = *(coordinates.at(0));

                // Compute position of the current maxima if more than one pixel
                int size = coordinates.size();
                if (size > 1)
                {
                    for (int j = 1 ; j < size ; j++)
                    {
                        Eigen::Vector3d* pos = coordinates.at(j);
                        coords[i](0) += (*pos)(0);
                        coords[i](1) += (*pos)(1);
                        if ((*pos)(2) > coords[i](2)) {coords[i](2) = (*pos)(2);}
                    }

                    coords[i](0) /= size;
                    coords[i](1) /= size;
                }
            }


            setProgress(30);

            // affect clusters positions to maxima

            QMap<qint32, CT_PointCluster*> maximaClusters;

            for (int i = 0 ; i < mxSize ; i++)
            {
                qint32 id = orderedMaxima.at(i);

                if (id > 0)
                {
                    double x = coords[i](0);
                    double y = coords[i](1);
                    double z = coords[i](2);
                    double radius = getRadius(z, searchRadii);

                    CT_PointCluster* bestPosition = NULL;
                    size_t bestCount = 0;
                    for (int j = 0 ; j < positions.size() ; j++)
                    {
                        CT_PointCluster* position = positions.at(j);

                        double dist = sqrt(pow(position->getCenterX() - x, 2) + pow(position->getCenterY() - y, 2));
                        if (dist < radius)
                        {
                            if (position->getPointCloudIndexSize() > bestCount)
                            {
                                bestCount = position->getPointCloudIndexSize();
                                bestPosition = position;
                            }
                        }
                    }

                    if (bestPosition != NULL)
                    {
                        positions.removeOne(bestPosition);
                        maximaClusters.insert(id, bestPosition);
                    }
                }
            }


            // For each radius, test others
            for (int i = 0 ; i < mxSize ; i++)
            {
                qint32 id = orderedMaxima.at(i);

                if (id > 0)
                {
                    double x = coords[i](0);
                    double y = coords[i](1);
                    double z = coords[i](2);
                    double radius = getRadius(z, exclusionRadii);

                    // detect the maximum to remove
                    for (int j = 0 ; j < mxSize ; j++)
                    {
                        qint32 idTested = orderedMaxima.at(j);

                        if (idTested > 0 && !maximaClusters.contains(idTested))
                        {
                            double dist = sqrt(pow(x - coords[j](0), 2) + pow(y - coords[j](1), 2));
                            if (dist < radius)
                            {
                                orderedMaxima[j] = 0;
                            }
                        }
                    }
                }

                setProgress(29.0*(float)i / (float)mxSize + 30.0);
            }

            setProgress(60);

            for (int i = 0 ; i < mxSize ; i++)
            {
                qint32 cl = orderedMaxima.at(i);
                if (cl > 0)
                {
                    validMaxima.append(cl);
                }
            }

            setProgress(70);

            QMap<qint32, qint32> newIds;
            qint32 cpt = 1;
            // effectively delete toRemove maximum and numbers them in a continuous way
            for (size_t xx = 0 ; xx < filteredMaxima->colDim() ; xx++)
            {
                for (size_t yy = 0 ; yy < filteredMaxima->linDim() ; yy++)
                {
                    qint32 maximaID = filteredMaxima->value(xx, yy);

                    if (maximaID > 0)
                    {
                        if (validMaxima.contains(maximaID))
                        {
                            qint32 newId = newIds.value(maximaID, 0);
                            if (newId == 0)
                            {
                                newId = cpt++;
                                newIds.insert(maximaID, newId);

                                CT_PointCluster *position = maximaClusters.value(maximaID, NULL);
                                if (position != NULL)
                                {
                                    CT_StandardItemGroup *grpPos = positionsGroups.value(position);
                                    if (grpPos != NULL)
                                    {
                                        CT_AttributesList *item = new CT_AttributesList(_attMaximaItem_ModelName.completeName(), res);
                                        item->addItemAttribute(new CT_StdItemAttributeT<qint32>(_attMaxima_ModelName.completeName(), CT_AbstractCategory::DATA_ID, res, newId));
                                        item->addItemAttribute(new CT_StdItemAttributeT<quint64>(_attClusterID_ModelName.completeName(), CT_AbstractCategory::DATA_ID, res, position->id()));
                                        grpPos->addItemDrawable(item);
                                    }
                                }
                            }
                            filteredMaxima->setValue(xx, yy, newId);
                        } else {
                            filteredMaxima->setValue(xx, yy, 0);
                        }
                    }
                }
            }

            for (int i = 0 ; i < positions.size() ; i++)
            {
                CT_StandardItemGroup *grp = positionsGroups.value(positions.at(i));
                if (grp != NULL)
                {
                    CT_AttributesList *item = new CT_AttributesList(_attMaximaItem_ModelName.completeName(), res);
                    item->addItemAttribute(new CT_StdItemAttributeT<qint32>(_attMaxima_ModelName.completeName(), CT_AbstractCategory::DATA_ID, res, 0));
                    item->addItemAttribute(new CT_StdItemAttributeT<quint64>(_attClusterID_ModelName.completeName(), CT_AbstractCategory::DATA_ID, res, positions.at(i)->id()));
                    grp->addItemDrawable(item);
                }
            }

            newIds.clear();
            setProgress(90);

            filteredMaxima->computeMinMax();

            qDeleteAll(maximaCoords.values());

            positions.clear();
            positionsGroups.clear();

            setProgress(99);
        }
    }
    setProgress(100);
}

double PB_StepFilterMaximaByClusterPositions::getRadius(double height, const QMap<double, double> &radii)
{
    double radius = 0;
    bool stop = false;
    QMapIterator<double, double> it(radii);
    while (it.hasNext() && !stop)
    {
        it.next();
        double h = it.key();
        double r = it.value();

        if (height >= h)
        {
            radius = r;
        } else {
            stop = true;
        }
    }

    return radius;
}
