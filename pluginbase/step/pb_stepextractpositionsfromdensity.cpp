#include "pb_stepextractpositionsfromdensity.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_iterator/ct_pointiterator.h"

#include <limits>


// Alias for indexing models
#define DEFin_rpts "rpts"
#define DEFin_grp "grp"
#define DEFin_points "points"

#define DEFout_rpos "rpos"
#define DEFout_grp "grp"
#define DEFout_point2d "point2d"

#define DEFout_rgrd "rgrd"
#define DEFout_grpGrd "grpGrd"
#define DEFout_grid "grid"



// Constructor : initialization of parameters
PB_StepExtractPositionsFromDensity::PB_StepExtractPositionsFromDensity(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _resolution = 0.05;
    _threshold = 0.2;
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
    CT_InResultModelGroup *resIn_rpts = createNewInResultModel(DEFin_rpts, tr("Points"));
    resIn_rpts->setZeroOrMoreRootGroup();
    resIn_rpts->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rpts->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Points"));
}

// Creation and affiliation of OUT models
void PB_StepExtractPositionsFromDensity::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_rpos = createNewOutResultModel(DEFout_rpos, tr("Positions"));
    res_rpos->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_rpos->addItemModel(DEFout_grp, DEFout_point2d, new CT_Point2D(), tr("Positions 2D"));

    CT_OutResultModelGroup *res_rgrd = createNewOutResultModel(DEFout_rgrd, tr("Grille de densité"));
    res_rgrd->setRootGroup(DEFout_grpGrd, new CT_StandardItemGroup(), tr("Groupe"));
    res_rgrd->addItemModel(DEFout_grpGrd, DEFout_grid, new CT_Grid2DXY<int>(), tr("Grille de densité"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExtractPositionsFromDensity::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Résolution du raster densité", "cm", 0.1, 1e+09, 1, _resolution, 100);
    configDialog->addDouble("Seuil de densité", "%", 0, 100, 2, _threshold, 100);
}

void PB_StepExtractPositionsFromDensity::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_rpts = inResultList.at(0);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_pos = outResultList.at(0);
    CT_ResultGroup* res_grid = outResultList.at(1);

    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();

    double xmax = -std::numeric_limits<double>::max();
    double ymax = -std::numeric_limits<double>::max();

    bool found = false;


    // Calcul de la bounding box XY
    CT_ResultGroupIterator itIn_grp(resIn_rpts, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grp = (CT_AbstractItemGroup*) itIn_grp.next();

        const CT_AbstractItemDrawableWithPointCloud* itemIn_points = (CT_AbstractItemDrawableWithPointCloud*)grpIn_grp->firstItemByINModelName(this, DEFin_points);
        if (itemIn_points != NULL)
        {
            found = true;

            if (itemIn_points->minX() < xmin) {xmin = itemIn_points->minX();}
            if (itemIn_points->minY() < ymin) {ymin = itemIn_points->minY();}
            if (itemIn_points->maxX() > xmax) {xmax = itemIn_points->maxX();}
            if (itemIn_points->maxY() > ymax) {ymax = itemIn_points->maxY();}
        }
    }


    if (found)
    {
        // Création de la grille de densité
        CT_StandardItemGroup* grp_Grd= new CT_StandardItemGroup(DEFout_grpGrd, res_grid);
        res_grid->addGroup(grp_Grd);
        CT_Grid2DXY<int>* grid = CT_Grid2DXY<int>::createGrid2DXYFromXYCoords(DEFout_grid, res_grid, xmin, ymin, xmax, ymax, _resolution, 0, -1, 0);
        grp_Grd->addItemDrawable(grid);


        // IN results browsing
        CT_ResultGroupIterator itIn_grp2(resIn_rpts, this, DEFin_grp);
        while (itIn_grp2.hasNext() && !isStopped())
        {
            const CT_AbstractItemGroup* grpIn_grp = (CT_AbstractItemGroup*) itIn_grp2.next();

            const CT_AbstractItemDrawableWithPointCloud* itemIn_points = (CT_AbstractItemDrawableWithPointCloud*)grpIn_grp->firstItemByINModelName(this, DEFin_points);
            if (itemIn_points != NULL)
            {
                CT_PointIterator itP(itemIn_points->getPointCloudIndex());
                while (itP.hasNext() && !isStopped())
                {
                    itP.next();
                    const CT_Point &point = itP.currentPoint();

                    grid->addValueAtXY(point(0), point(1), 1);
                }
            }
        }
        grid->computeMinMax();

        //Seuillage
        double threshold = _threshold * grid->dataMax();

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
                    if (size)
                    {
                        fillCellsInList(liste, lastCluster++, clusters);

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

                        CT_StandardItemGroup* grpPos= new CT_StandardItemGroup(DEFout_grp, res_pos);
                        res_pos->addGroup(grpPos);

                        CT_Point2DData* point2dData = new CT_Point2DData(x, y);
                        CT_Point2D* point2d = new CT_Point2D(DEFout_point2d, res_pos, point2dData);
                        grpPos->addItemDrawable(point2d);
                    }
                }
            }
        }
        delete clusters;
    }

}


void PB_StepExtractPositionsFromDensity::fillCellsInList(QList<size_t> &liste, const int cluster, CT_Grid2DXY<int> *clustersGrid)
{
    if (liste.isEmpty()) {return;}

    for (int i = 0 ; i < liste.size() ; i++)
    {
        size_t index = liste.at(i);
        clustersGrid->setValueAtIndex(index, cluster);
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
