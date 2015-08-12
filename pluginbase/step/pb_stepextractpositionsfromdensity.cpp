#include "pb_stepextractpositionsfromdensity.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_iterator/ct_pointiterator.h"

#include <limits>


// Alias for indexing models
#define DEFin_rpts "rpts"
#define DEFin_grp "grp"
#define DEFin_points "points"

// Constructor : initialization of parameters
PB_StepExtractPositionsFromDensity::PB_StepExtractPositionsFromDensity(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _resolution = 0.05;
    _threshold = 0.2;
    _thresholdN = 2;
    _relativeMode = 0;
}

// Step description (tooltip of contextual menu)
QString PB_StepExtractPositionsFromDensity::getStepDescription() const
{
    return tr("Créée des positions 2D à partir de la densité des points");
}

// Step detailled description
QString PB_StepExtractPositionsFromDensity::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepExtractPositionsFromDensity::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepExtractPositionsFromDensity::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepExtractPositionsFromDensity(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepExtractPositionsFromDensity::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_rpts = createNewInResultModelForCopy(DEFin_rpts, tr("Points"));
    resIn_rpts->setZeroOrMoreRootGroup();
    resIn_rpts->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rpts->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Points"));
}

// Creation and affiliation of OUT models
void PB_StepExtractPositionsFromDensity::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_rpts);

    res->addGroupModel(DEFin_grp, _grpPosition2D_ModelName, new CT_StandardItemGroup(), tr("Positions 2D (grp)"));
    res->addItemModel(_grpPosition2D_ModelName, _position2D_ModelName, new CT_Point2D(), tr("Positions 2D"));
    res->addItemAttributeModel(_position2D_ModelName, _position2DAtt_ModelName, new CT_StdItemAttributeT<int>(CT_AbstractCategory::DATA_NUMBER), tr("Densité"));
    res->addItemModel(DEFin_grp, _grid2D_ModelName, new CT_Grid2DXY<int>(), tr("Grille de densité"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExtractPositionsFromDensity::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Résolution du raster densité", "cm", 0.1, 1e+09, 1, _resolution, 100);

    CT_ButtonGroup &bg_relativeMode = configDialog->addButtonGroup(_relativeMode);

    configDialog->addExcludeValue(tr("Seuil en valeur relative"), "", "", bg_relativeMode, 0);
    configDialog->addDouble("Seuil de densité (inclus)", "% du max", 0, 100, 2, _threshold, 100);
    configDialog->addExcludeValue(tr("Seuil en valeur absolue"), "", "", bg_relativeMode, 1);
    configDialog->addInt("Seuil de densité (inclus)", "pts", 0, 10000, _thresholdN);
}

void PB_StepExtractPositionsFromDensity::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res = outResultList.at(0);

    // Calcul de la bounding box XY
    CT_ResultGroupIterator itIn_grp(res, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itIn_grp.next();

        const CT_AbstractItemDrawableWithPointCloud* scene = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_points);
        if (scene != NULL)
        {

            double xmin = scene->minX();
            double ymin = scene->minY();
            double xmax = scene->maxX();
            double ymax = scene->maxY();


            // Création de la grille de densité
            CT_Grid2DXY<int>* grid = CT_Grid2DXY<int>::createGrid2DXYFromXYCoords(_grid2D_ModelName.completeName(), res, xmin, ymin, xmax, ymax, _resolution, 0, -1, 0);
            grp->addItemDrawable(grid);

            CT_PointIterator itP(scene->getPointCloudIndex());
            while (itP.hasNext() && !isStopped())
            {
                itP.next();
                const CT_Point &point = itP.currentPoint();

                grid->addValueAtXY(point(0), point(1), 1);
            }
            grid->computeMinMax();

            //Seuillage
            double threshold = _threshold * grid->dataMax();
            if (_relativeMode == 1)
            {
                threshold = _thresholdN;
            }

            for (size_t indice = 0 ; indice < grid->nCells() ; indice++)
            {
                if (grid->valueAtIndex(indice) < threshold)
                {
                    grid->setValueAtIndex(indice, 0);
                }
            }

            CT_Grid2DXY<int>* clusters = new CT_Grid2DXY<int>(NULL, NULL, xmin, ymin, grid->colDim(), grid->linDim(), _resolution, 0, -1, -1);

            size_t colDim = grid->colDim();
            size_t linDim = grid->linDim();
            int lastCluster = 0;

            for (size_t cx = 0 ; cx < colDim ; cx++)
            {
                for (size_t ly = 0 ; ly < linDim ; ly++)
                {
                    int cluster = clusters->value(cx, ly);

                    if (cluster < 0)
                    {
                        QList<size_t> liste = computeColonize(cx, ly, grid);
                        int size = liste.size();
                        if (size > 0)
                        {
                            int density = 0;
                            fillCellsInList(liste, lastCluster++, clusters, grid, density);

                            double x = 0;
                            double y = 0;

                            for (int i = 0 ; i < size ; i++)
                            {
                                Eigen::Vector3d cell;
                                if (grid->getCellCenterCoordinates(liste.at(i), cell))
                                {
                                    x += cell(0);
                                    y += cell(1);
                                }
                            }

                            x /= size;
                            y /= size;

                            CT_StandardItemGroup* grpPos = new CT_StandardItemGroup(_grpPosition2D_ModelName.completeName(), res);
                            grp->addGroup(grpPos);

                            CT_Point2DData* point2dData = new CT_Point2DData(x, y);
                            CT_Point2D* point2d = new CT_Point2D(_position2D_ModelName.completeName(), res, point2dData);
                            grpPos->addItemDrawable(point2d);

                            point2d->addItemAttribute(new CT_StdItemAttributeT<int>(_position2DAtt_ModelName.completeName(),
                                                                                          CT_AbstractCategory::DATA_NUMBER,
                                                                                          res,
                                                                                          density));

                        }
                    }
                }
            }
            delete clusters;
        }                
    }

}


void PB_StepExtractPositionsFromDensity::fillCellsInList(QList<size_t> &liste, const int cluster, CT_Grid2DXY<int> *clustersGrid, CT_Grid2DXY<int> *densityGrid, int &density)
{
    if (liste.isEmpty()) {return;}

    density = 0;
    for (int i = 0 ; i < liste.size() ; i++)
    {
        size_t index = liste.at(i);
        clustersGrid->setValueAtIndex(index, cluster);
        density += densityGrid->valueAtIndex(index);
    }
}

QList<size_t> PB_StepExtractPositionsFromDensity::computeColonize(size_t originColumn, size_t originRow, const CT_Grid2DXY<int> *densityGrid)
{
    QList<size_t> result;
    size_t index;

    if (!densityGrid->index(originColumn, originRow, index))
    {
        return result;
    }

    if (densityGrid->valueAtIndex(index) > 0) {result.append(index);}

    int i = 0;
    while (i < result.size())
    {
        size_t current_col, current_row;
        densityGrid->indexToGrid(result.at(i), current_col, current_row);

        appendIfNotNulValue(result, current_col - 1, current_row, densityGrid);
        appendIfNotNulValue(result, current_col, current_row - 1, densityGrid);
        appendIfNotNulValue(result, current_col + 1, current_row, densityGrid);
        appendIfNotNulValue(result, current_col, current_row + 1, densityGrid);

        appendIfNotNulValue(result, current_col - 1, current_row - 1, densityGrid);
        appendIfNotNulValue(result, current_col - 1, current_row + 1, densityGrid);
        appendIfNotNulValue(result, current_col + 1, current_row - 1, densityGrid);
        appendIfNotNulValue(result, current_col + 1, current_row + 1, densityGrid);

        ++i;
    }

    return result;
}

void PB_StepExtractPositionsFromDensity::appendIfNotNulValue(QList<size_t> &result, size_t col, size_t lin, const CT_Grid2DXY<int> *densityGrid)
{
    size_t index;
    if (densityGrid->index(col, lin, index))
    {
        if (densityGrid->valueAtIndex(index) > 0 && !result.contains(index))
        {
            result.append(index);
        }
    }
}
