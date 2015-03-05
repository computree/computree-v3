#include "pb_stepselectcellsingrid3dbybinarypattern.h"

#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_rgrid "rgrid"
#define DEFin_grp "grp"
#define DEFin_grid "grid"

#define DEFout_rgrid "rgrid"
#define DEFout_grp "grp"
#define DEFout_grid "grid"
#define DEFout_gridBool "gridBool"


// Constructor : initialization of parameters
PB_StepSelectCellsInGrid3DByBinaryPattern::PB_StepSelectCellsInGrid3DByBinaryPattern(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _inThreshold = 1;

    _pattern =  "1,1,0,0\n"
                "1,1,0,0\n"
                "1,1,0,0\n"
                "0,1,0,0\n"
                "1,1,0,0\n"
                "1,1,0,0\n"
                "1,1,0,0\n";

    _outThreshold = 20;
}

// Step description (tooltip of contextual menu)
QString PB_StepSelectCellsInGrid3DByBinaryPattern::getStepDescription() const
{
    return tr("Créée une grille booléenne de séléction à partir d'un motif binaire");
}

// Step detailled description
QString PB_StepSelectCellsInGrid3DByBinaryPattern::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepSelectCellsInGrid3DByBinaryPattern::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSelectCellsInGrid3DByBinaryPattern::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSelectCellsInGrid3DByBinaryPattern(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSelectCellsInGrid3DByBinaryPattern::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_rgrid = createNewInResultModel(DEFin_rgrid, tr("Grille"));
    resIn_rgrid->setRootGroup(DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rgrid->addItemModel(DEFin_grp, DEFin_grid, CT_AbstractGrid3D::staticGetType(), tr("Grille"));

}

// Creation and affiliation of OUT models
void PB_StepSelectCellsInGrid3DByBinaryPattern::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_rgrid = createNewOutResultModel(DEFout_rgrid, tr("Grille de séléction"));
    res_rgrid->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_rgrid->addItemModel(DEFout_grp, DEFout_grid, new CT_Grid3D<int>(), tr("Grille de comptage"));
    res_rgrid->addItemModel(DEFout_grp, DEFout_gridBool, new CT_Grid3D<bool>(), tr("Grille de séléction"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSelectCellsInGrid3DByBinaryPattern::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Valeur minimale de la grille d'entrée", "", -1e+09, 1e+09, 4, _inThreshold, 1);
    configDialog->addString("Motif binaire", "", _pattern);
    configDialog->addInt("Seuil de sortie", "", 0, 1e+09, _outThreshold);
}

void PB_StepSelectCellsInGrid3DByBinaryPattern::compute()
{
    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_rgrid = inResultList.at(0);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_rgrid = outResultList.at(0);

    // IN results browsing
    CT_ResultGroupIterator itIn_grp(resIn_rgrid, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grp = (CT_AbstractItemGroup*) itIn_grp.next();
        
        const CT_AbstractGrid3D* inGrid = (CT_AbstractGrid3D*)grpIn_grp->firstItemByINModelName(this, DEFin_grid);
        if (inGrid != NULL)
        {
            QList<int> rowNb;
            QList<int> colNb;
            QList<int> levzNb;
            QList<int> vals;

            if(computeLists(_pattern, rowNb, colNb, levzNb, vals))
            {
                CT_StandardItemGroup* grp= new CT_StandardItemGroup(DEFout_grp, res_rgrid);
                res_rgrid->addGroup(grp);

                CT_Grid3D<int>* outGrid = new CT_Grid3D<int>(DEFout_grid, res_rgrid, inGrid->minX(), inGrid->minY(), inGrid->minZ(), inGrid->xdim(), inGrid->ydim(), inGrid->zdim(), inGrid->resolution(), -1, -1);
                grp->addItemDrawable(outGrid);

                int listsSize = rowNb.size();

                for (size_t indice = 0 ; indice < inGrid->nCells() ; indice++)
                {
                    double value = inGrid->valueAtIndexAsDouble(indice);
                    if (value >= _inThreshold)
                    {
                        if (outGrid->valueAtIndex(indice) == outGrid->NA()) {outGrid->setValueAtIndex(indice, 0);}

                        size_t row, col, levz;
                        inGrid->indexToGrid(indice, col, row, levz);

                        for (int i = 0 ; i <  listsSize ; i++)
                        {
                            int deltaRow = rowNb.at(i);
                            int deltaCol = colNb.at(i);
                            int deltaLevz = levzNb.at(i);
                            int val = vals.at(i);

                            size_t index;
                            if (outGrid->index(col + deltaCol, row + deltaRow, levz + deltaLevz, index))
                            {
                                if (inGrid->valueAtIndexAsDouble(index) >= _inThreshold)
                                {
                                    outGrid->addValueAtIndex(index, val);
                                }
                            }
                        }
                    }

                    setProgress(90.0 * (float)indice / (float)inGrid->nCells());
                }
                outGrid->computeMinMax();


                CT_Grid3D<bool>* outGridBool = new CT_Grid3D<bool>(DEFout_gridBool, res_rgrid, inGrid->minX(), inGrid->minY(), inGrid->minZ(), inGrid->xdim(), inGrid->ydim(), inGrid->zdim(), inGrid->resolution(), false, false);
                grp->addItemDrawable(outGridBool);

                for (size_t indice = 0 ; indice < outGrid->nCells() ; indice++)
                {
                    double value = outGrid->valueAtIndexAsDouble(indice);
                    if (value >= _outThreshold)
                    {
                        outGridBool->setValueAtIndex(indice, true);
                    }
                }
                setProgress(100.0);
            }
        }
    }
}

bool PB_StepSelectCellsInGrid3DByBinaryPattern::computeLists(const QString &pattern, QList<int> &rowNb, QList<int> &colNb, QList<int> &levzNb, QList<int> &vals)
{
    QStringList levels = pattern.split("\n", QString::SkipEmptyParts);
    int nlevels = levels.size();

    if (nlevels < 1) {return false;}
    if (nlevels % 2 == 0) {return false;}

    QStringList cells = levels.at(0).split(",");

    int ncells = cells.size();

    if (ncells < 1) {return false;}

    int centerLevel = (nlevels % 2);

    for (int l = 0 ; l < nlevels ; l++)
    {
        cells = levels.at(l).split(",");

        if (cells.size() != ncells) {return false;}

        for (int c = 0 ; c < ncells ; c++)
        {
            bool ok;
            int val = cells.at(c).toInt(&ok);
            if (!ok) {return false;}

            if (val != 0)
            {
                for (int col = -c ; col <= c ; col++)
                {
                    levzNb.append(l - centerLevel);
                    rowNb.append(-c);
                    colNb.append(col);
                    vals.append(val);
                }

                for (int col = -c + 1 ; col <= c - 1; col++)
                {
                    levzNb.append(l - centerLevel);
                    rowNb.append(c);
                    colNb.append(col);
                    vals.append(val);
                }

                for (int row = -c + 1 ; row <= c - 1; row++)
                {
                    levzNb.append(l - centerLevel);
                    rowNb.append(row);
                    colNb.append(-c);
                    vals.append(val);
                }

                for (int row = -c + 1 ; row <= c - 1; row++)
                {
                    levzNb.append(l - centerLevel);
                    rowNb.append(row);
                    colNb.append(c);
                    vals.append(val);
                }

            }
        }
    }

    return true;
}
