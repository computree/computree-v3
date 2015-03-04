#include "pb_stepcompare3dgridscontents.h"

#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_rgrid1 "rgrid1"
#define DEFin_grp1 "grp1"
#define DEFin_grid1 "grid1"

#define DEFin_rgrid2 "rgrid2"
#define DEFin_grp2 "grp2"
#define DEFin_grid2 "grid2"

#define DEFout_rgrid "rgrid"
#define DEFout_grp "grp"
#define DEFout_grid "grid"


// Constructor : initialization of parameters
PB_StepCompare3DGridsContents::PB_StepCompare3DGridsContents(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _threshold = 0;
}

// Step description (tooltip of contextual menu)
QString PB_StepCompare3DGridsContents::getStepDescription() const
{
    return tr("Compare le contenu de deux grilles 3D");
}

// Step detailled description
QString PB_StepCompare3DGridsContents::getStepDetailledDescription() const
{
    return tr("Il est préférable que les grilles aient la même résolution et le même calage spatial.<br>"
              "En sortie l'étape renvoie une grille contenant :<br>"
              "* 0 si les deux grilles sont inférieures au seuil (vide durable)<br>"
              "* 1 si la première grille est supérieur au seuil mais pas la seconde (perte)<br>"
              "* 2 si les deux grilles sont supérieures au seuil (maintien)<br>"
              "* 3 si la seconde grille est supérieure au seuil mais pas la première (accroissement)");
}

// Step URL
QString PB_StepCompare3DGridsContents::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCompare3DGridsContents::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepCompare3DGridsContents(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepCompare3DGridsContents::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_rgrid1 = createNewInResultModel(DEFin_rgrid1, tr("Grille 1 (avant)"));
    resIn_rgrid1->setRootGroup(DEFin_grp1, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rgrid1->addItemModel(DEFin_grp1, DEFin_grid1, CT_AbstractGrid3D::staticGetType(), tr("Grille 1 (avant)"));

    CT_InResultModelGroup *resIn_rgrid2 = createNewInResultModel(DEFin_rgrid2, tr("Grille 2 (après)"));
    resIn_rgrid2->setRootGroup(DEFin_grp2, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rgrid2->addItemModel(DEFin_grp2, DEFin_grid2,CT_AbstractGrid3D::staticGetType(), tr("Grille 2 (après)"));

}

// Creation and affiliation of OUT models
void PB_StepCompare3DGridsContents::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_rgrid = createNewOutResultModel(DEFout_rgrid, tr("Grille"));
    res_rgrid->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_rgrid->addItemModel(DEFout_grp, DEFout_grid, new CT_Grid3D<short>(), tr("Grille"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepCompare3DGridsContents::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Valeur minimum pour considérer une case remplie", "", -1e+09, 1e+09, 4, _threshold, 1);
}

void PB_StepCompare3DGridsContents::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_rgrid1 = inResultList.at(0);
    CT_ResultGroup* resIn_rgrid2 = inResultList.at(1);


    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_rgrid = outResultList.at(0);


    // IN results browsing
    CT_ResultGroupIterator itIn_grp1(resIn_rgrid1, this, DEFin_grp1);
    if (itIn_grp1.hasNext())
    {
        const CT_AbstractItemGroup* grpIn_grp1 = (CT_AbstractItemGroup*) itIn_grp1.next();
        const CT_AbstractGrid3D* grid1 = (CT_AbstractGrid3D*)grpIn_grp1->firstItemByINModelName(this, DEFin_grid1);
        if (grid1 != NULL)
        {
            CT_ResultGroupIterator itIn_grp2(resIn_rgrid2, this, DEFin_grp2);
            if (itIn_grp2.hasNext())
            {
                const CT_AbstractItemGroup* grpIn_grp2 = (CT_AbstractItemGroup*) itIn_grp2.next();
                const CT_AbstractGrid3D* grid2 = (CT_AbstractGrid3D*)grpIn_grp2->firstItemByINModelName(this, DEFin_grid2);
                if (grid2 != NULL)
                {
                    double xmin = grid1->minX();
                    double ymin = grid1->minY();
                    double zmin = grid1->minZ();
                    double xmax = grid1->maxX();
                    double ymax = grid1->maxY();
                    double zmax = grid1->maxZ();

                    double res = grid1->resolution();

                    if (grid2->minX() < xmin) {xmin = grid2->minX();}
                    if (grid2->minY() < ymin) {ymin = grid2->minY();}
                    if (grid2->minZ() < zmin) {zmin = grid2->minZ();}
                    if (grid2->maxX() > xmax) {xmax = grid2->maxX();}
                    if (grid2->maxY() > ymax) {ymax = grid2->maxY();}
                    if (grid2->maxZ() > zmax) {zmax = grid2->maxZ();}

                    if (grid2->resolution() < res) {res = grid2->resolution();}

                    // Création de la grille de sortie
                    CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_rgrid);
                    res_rgrid->addGroup(grp_grp);

                    //CT_Grid3D<short>* gridOut = new CT_Grid3D<short>::createGrid3DFromXYZCoords(DEFout_grid, res_rgrid, xmin, ymin, zmin, xmax, ymax, zmax, res, -1, 0);
//                    grp_grp->addItemDrawable(gridOut);

//                    for (size_t n = 0 ; n < gridOut->nCells() ; n++)
//                    {
//                        double x = gridOut->getCellCenterX(gridOut->get)
//                    }


                }
            }
        }
    }


}
