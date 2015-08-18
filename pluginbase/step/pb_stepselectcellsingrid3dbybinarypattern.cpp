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

    _pattern =  "1,1,0,0,0\n"
                "1,1,0,0,0\n"
                "1,1,0,0,0\n"
                "1,1,0,0,0\n"
                "1,1,0,0,0\n"
                "1,0,0,0,0\n"
                "0,0,0,0,0\n"
                "0,0,0,0,0\n"
                "0,0,0,0,0\n"
                "0,0,0,0,0\n"
                "0,0,0,0,0\n"
                "0,0,0,0,0\n"
                "0,0,0,0,0\n";

    _outThresholdAbsolute = 20;
    _outThresholdRelative = 0.9;
    _selectMode = 1;
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
    res_rgrid->addItemModel(DEFout_grp, DEFout_grid, new CT_Grid3D<double>(), tr("Grille de comptage"));
    res_rgrid->addItemModel(DEFout_grp, DEFout_gridBool, new CT_Grid3D<bool>(), tr("Grille de séléction"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSelectCellsInGrid3DByBinaryPattern::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Valeur minimale de la grille d'entrée"), "", -1e+09, 1e+09, 4, _inThreshold, 1);
    configDialog->addString(tr("Motif binaire"), "", _pattern);
    configDialog->addEmpty();
    configDialog->addTitle(tr("Choix du mode de fitrage :"));

    CT_ButtonGroup &bg_gridMode = configDialog->addButtonGroup(_selectMode);
    configDialog->addExcludeValue("", "", tr("En valeur absolue :"), bg_gridMode, 0);
    configDialog->addInt(tr("Seuil de séléction"), "", 0, 1e+09, _outThresholdAbsolute);

    configDialog->addExcludeValue("", "", tr("En % de la valeur maximale :"), bg_gridMode, 1);
    configDialog->addDouble(tr("Seuil de séléction (en % du max)"), "", 0, 100, 2, _outThresholdRelative, 100);

}

void PB_StepSelectCellsInGrid3DByBinaryPattern::compute()
{
    QList<PatternCell> parsedPattern;
    if (!parsePattern(parsedPattern)) {return;}


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

            CT_StandardItemGroup* grp= new CT_StandardItemGroup(DEFout_grp, res_rgrid);
            res_rgrid->addGroup(grp);

            CT_Grid3D<double>* outGrid = new CT_Grid3D<double>(DEFout_grid, res_rgrid, inGrid->minX(), inGrid->minY(), inGrid->minZ(), inGrid->xdim(), inGrid->ydim(), inGrid->zdim(), inGrid->resolution(), -1, -1);
            grp->addItemDrawable(outGrid);


            for (size_t indexOut = 0 ; indexOut < outGrid->nCells() ; indexOut++)
            {
                double sum = 0;

                size_t row, col, lev;
                if (inGrid->indexToGrid(indexOut, col, row, lev))
                {
                    if (inGrid->valueAtIndexAsDouble(indexOut) > 0)
                    {
                        QListIterator<PatternCell> it(parsedPattern);
                        while (it.hasNext())
                        {
                            const PatternCell &pat = it.next();

                            size_t modifiedIndex;
                            if (inGrid->index(col + pat._colRel, row + pat._rowRel, lev + pat._levRel, modifiedIndex))
                            {
                                if (inGrid->valueAtIndexAsDouble(modifiedIndex) > _inThreshold)
                                {
                                    sum += pat._val;
                                }
                            }
                        }
                        outGrid->setValueAtIndex(indexOut, sum);
                    }
                }

            }
            outGrid->computeMinMax();


            CT_Grid3D<bool>* outGridBool = new CT_Grid3D<bool>(DEFout_gridBool, res_rgrid, inGrid->minX(), inGrid->minY(), inGrid->minZ(), inGrid->xdim(), inGrid->ydim(), inGrid->zdim(), inGrid->resolution(), false, false);
            grp->addItemDrawable(outGridBool);

            int threshold = _outThresholdAbsolute;
            if (_selectMode == 1)
            {
                threshold = outGrid->dataMax()*_outThresholdRelative;
            }

            PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString("Seuil de séléction : %1 (valeurs observées : %2 - %3)").arg(threshold).arg(outGrid->dataMin()).arg(outGrid->dataMax()));

            for (size_t indice = 0 ; indice < outGrid->nCells() ; indice++)
            {
                double value = outGrid->valueAtIndexAsDouble(indice);
                if (value >= threshold)
                {
                    outGridBool->setValueAtIndex(indice, true);
                }
            }
            setProgress(100.0);
        }
    }

}

bool PB_StepSelectCellsInGrid3DByBinaryPattern::parsePattern(QList<PatternCell> &parsedPattern)
{
    // Pattern parsing
    QStringList levels = _pattern.split("\n", QString::SkipEmptyParts);
    int nlevels = levels.size();
    if (nlevels < 1) {return false;}
    if (nlevels % 2 == 0) {return false;}

    int maxLevel = nlevels / 2;

    for (int lev = 0 ; lev < nlevels ; lev++)
    {
        int levelNumber = maxLevel - lev;

        QStringList cells = levels.at(lev).split(",");
        int ncells = cells.size();

        if (ncells > 0)
        {
            bool ok;
            double val = cells.at(0).toDouble(&ok);
            if (ok && val != 0)
            {
                parsedPattern.append(PatternCell(0, 0, levelNumber, val));
            }

            for (int c = 1 ; c < ncells ; c++)
            {
                val = cells.at(c).toDouble(&ok);
                if (ok && val != 0)
                {
                    for (int j = -c ; j < c ; j++)
                    {
                        parsedPattern.append(PatternCell(-c,  j, levelNumber, val));
                        parsedPattern.append(PatternCell( j,  c, levelNumber, val));
                        parsedPattern.append(PatternCell( c, -j, levelNumber, val));
                        parsedPattern.append(PatternCell(-j, -c, levelNumber, val));
                    }
                }
            }
        }
    }
    return true;
}

