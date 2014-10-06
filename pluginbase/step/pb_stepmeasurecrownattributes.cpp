#include "pb_stepmeasurecrownattributes.h"

// Inclusion of in models
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"

//Inclusion of actions
#include "actions/pb_actionmeasurecrownattributes.h"
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include <limits>
#include <QMessageBox>

// Alias for indexing in models
#define DEF_resultIn "result"
#define DEF_itemScene "scene"

#define DEF_resultOut "result"
#define DEF_SearchOutGroup "group"
#define DEF_SearchOutGrid "grid"

// Constructor : initialization of parameters
PB_StepMeasureCrownAttributes::PB_StepMeasureCrownAttributes(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    _res = 0.5;
    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepMeasureCrownAttributes::getStepDescription() const
{
    return tr("Mesure des attributs d'un houppier");
}

// Step description (tooltip of contextual menu)
QString PB_StepMeasureCrownAttributes::getStepDetailledDescription() const
{
    return tr("Cette étape permet de mesurer des attributs d'un houppier");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepMeasureCrownAttributes::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepMeasureCrownAttributes(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepMeasureCrownAttributes::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultInModel = createNewInResultModel(DEF_resultIn,
                                                                  tr("Result"),
                                                                  tr(""),
                                                                  false);
    resultInModel->setZeroOrMoreRootGroup();
    resultInModel->addItemModel("",
                                DEF_itemScene,
                                CT_Scene::staticGetType(),
                                tr("Scène(s)"));
}

// Creation and affiliation of OUT models
void PB_StepMeasureCrownAttributes::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *resultOutModel = createNewOutResultModel(DEF_resultOut, tr("Projection"));
    resultOutModel->setRootGroup(DEF_SearchOutGroup);
    resultOutModel->addItemModel(DEF_SearchOutGroup, DEF_SearchOutGrid, new CT_Grid2DXY<float>(), tr("Projection"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepMeasureCrownAttributes::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addDouble(tr("Résolution de la grille"),tr("meters"),0.0001,10000,2, _res );
}

void PB_StepMeasureCrownAttributes::compute()
{
    m_doc = NULL;
    m_status = 0;

    CT_ResultGroup *resultIn = getInputResults().first();

    const QList<CT_ResultGroup*> &outResList = getOutResultList();
    CT_ResultGroup *outResult = outResList.at(0);

    m_itemDrawableToAdd.clear();

    _xmin = std::numeric_limits<float>::max();
    _ymin = std::numeric_limits<float>::max();
    _zmin = std::numeric_limits<float>::max();

    _xmax = -std::numeric_limits<float>::max();
    _ymax = -std::numeric_limits<float>::max();
    _zmax = -std::numeric_limits<float>::max();

    CT_ResultItemIterator it(resultIn, this, DEF_itemScene);

    // Parcours des groupes contenant les scènes à filtrer
    while(it.hasNext()
          && !isStopped())
    {
        CT_Scene *scene = (CT_Scene*)it.next();

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
        CT_StandardItemGroup *group = new CT_StandardItemGroup(DEF_SearchOutGroup, outResult);
        _grid = CT_Grid2DXY<int>::createGrid2DXYFromXYCoords(DEF_SearchOutGrid, outResult, _xmin, _ymin, _xmax, _ymax, _res, _zmin, -1, 0);
        group->addItemDrawable(_grid);
        outResult->addGroup(group);

        // request the manual mode
        requestManualMode();

        m_status = 1;
        requestManualMode();
    }

}

void PB_StepMeasureCrownAttributes::initManualMode()
{
    if(m_doc == NULL)
    {
        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument();

        PB_ActionMeasureCrownAttributes *action = new PB_ActionMeasureCrownAttributes(_grid, _sceneList, _xmin, _ymin, _zmin, _xmax, _ymax, _zmax);

        // check if don't exist in the action manager
        if(!getGuiContext()->actionsManager()->existAction(action->uniqueName()))
        {
            // if not we add it to the manager. The action is deleted by the manager.
            getGuiContext()->actionsManager()->addAction(action);
        }

        // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
        m_doc->setCurrentAction(action);
    }

    m_doc->removeAllItemDrawable();

    QListIterator<CT_Scene*> it(m_itemDrawableToAdd);

    while(it.hasNext())
    {
        m_doc->addItemDrawable(*it.next());
    }

    QMessageBox::information(NULL, tr("Mesure des houppiers"), tr("Bienvenue dans cet outils de mesure des houppiers.\n"
                                                                  "Laisser la souris au-dessus d'un bouton pour avoir des infos."), QMessageBox::Ok);
}

void PB_StepMeasureCrownAttributes::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
            // nothing to do
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            getGuiContext()->documentManager()->closeDocument(m_doc);
            m_doc = NULL;

            quitManualMode();
        }
    }
}
