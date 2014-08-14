/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginONFENSAM library 2.0.

 PluginONFENSAM is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginONFENSAM is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginONFENSAM.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "pb_stepsetaffiliationidfromreference.h"
#include "actions/pb_actionmodifyaffiliations.h"

#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h"

#include "ct_itemdrawable/ct_affiliationid.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"

#include "qdebug.h"

#include <QMessageBox>

#define DEF_SearchInSourceResult      "rs"
#define DEF_SearchInSourceGroup       "gs"
#define DEF_SearchInSourceId          "ids"
#define DEF_SearchInSourceItem        "its"
#define DEF_SearchInTargetResult      "rt"
#define DEF_SearchInTargetGroup       "gt"
#define DEF_SearchInTargetId          "idt"
#define DEF_SearchInTargetItem        "itt"

PB_StepSetAffiliationIDFromReference::PB_StepSetAffiliationIDFromReference(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _2Dsearch = true;
    _manualModeActivated = false;
    m_doc = NULL;
    setManual(true);
}

QString PB_StepSetAffiliationIDFromReference::getStepDescription() const
{
    return tr("Modifie les identifiants d'affiliation de groupes, sur la base d'une référence");
}

QString PB_StepSetAffiliationIDFromReference::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

CT_VirtualAbstractStep* PB_StepSetAffiliationIDFromReference::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepSetAffiliationIDFromReference(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepSetAffiliationIDFromReference::createInResultModelListProtected()
{
    // a besoin en entrée d'un résultat contenant un ou plusieurs groupes
    CT_InZeroOrMoreGroupModel *groupSource = new CT_InZeroOrMoreGroupModel();

    CT_InStdGroupModel* refgroupSource = new CT_InStdGroupModel(DEF_SearchInSourceGroup, "Groupe de référence", "", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple);
    CT_InStandardItemDrawableModel *idItemSource = new CT_InStandardItemDrawableModel(DEF_SearchInSourceId, CT_AffiliationID::staticGetType(), tr("ID de référence"));
    CT_InStandardItemDrawableModel *refItemSource = new CT_InStandardItemDrawableModel(DEF_SearchInSourceItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item de référence"));

    groupSource->addGroup(refgroupSource);
    refgroupSource->addItem(idItemSource);
    refgroupSource->addItem(refItemSource);

    // le résultat est du type ResultGroupToCopy car nous voulons
    // un résultat du type ResultGroup que nous allons copier et modifier
    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInSourceResult, groupSource, tr("Résultat de référence"), "", true));


    // a besoin en entrée d'un résultat contenant un ou plusieurs groupes
    CT_InZeroOrMoreGroupModel *groupTarget = new CT_InZeroOrMoreGroupModel();

    CT_InStdGroupModel* refgroupTarget = new CT_InStdGroupModel(DEF_SearchInTargetGroup, "Groupe à affilier", "", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple);
    CT_InStandardItemDrawableModel *idItemTarget = new CT_InStandardItemDrawableModel(DEF_SearchInTargetId, CT_AffiliationID::staticGetType(), tr("ID à affilier"));
    CT_InStandardItemDrawableModel *refItemTarget = new CT_InStandardItemDrawableModel(DEF_SearchInTargetItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item à affilier"));

    groupTarget->addGroup(refgroupTarget);
    refgroupTarget->addItem(idItemTarget);
    refgroupTarget->addItem(refItemTarget);

    // le résultat est du type ResultGroupToCopy car nous voulons
    // un résultat du type ResultGroup que nous allons copier et modifier
    addInResultModel(new CT_InResultModelGroupToCopy(DEF_SearchInTargetResult, groupTarget, tr("Résultat à affilier"), "", true));
}

void PB_StepSetAffiliationIDFromReference::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addBool(tr("Affiliation par position 2D (3D sinon)"), "", "", _2Dsearch);
    configDialog->addBool(tr("Correction des affiliations en mode manuel"), "", "", _manualModeActivated);
}

void PB_StepSetAffiliationIDFromReference::createOutResultModelListProtected()
{
    setManual(_manualModeActivated);

    addOutResultModelCopy((CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInSourceResult));
    addOutResultModelCopy((CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInTargetResult));
}

void PB_StepSetAffiliationIDFromReference::compute()
{
    QList<CT_ResultGroup*> resultList = getOutResultList();
    CT_ResultGroup *sourceRes = resultList.at(0);
    CT_InAbstractGroupModel* refgroupSourceModel = (CT_InAbstractGroupModel*)getInModelForResearchIfUseCopy(DEF_SearchInSourceResult, DEF_SearchInSourceGroup);
    CT_InAbstractItemDrawableModel* idItemSourceModel = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_SearchInSourceResult, DEF_SearchInSourceId);
    CT_InAbstractItemDrawableModel* refItemSourceModel = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_SearchInSourceResult, DEF_SearchInSourceItem);


    CT_ResultGroup *targetRes = resultList.at(1);
    CT_InAbstractGroupModel* refgroupTargetModel = (CT_InAbstractGroupModel*)getInModelForResearchIfUseCopy(DEF_SearchInTargetResult, DEF_SearchInTargetGroup);
    CT_InAbstractItemDrawableModel* idItemTargetModel = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_SearchInTargetResult, DEF_SearchInTargetId);
    CT_InAbstractItemDrawableModel* refItemTargetModel = (CT_InAbstractItemDrawableModel*)getInModelForResearchIfUseCopy(DEF_SearchInTargetResult, DEF_SearchInTargetItem);


    QMap<CT_AbstractSingularItemDrawable*, size_t> sourceMap;

    // Parcours des groupes contenant les scènes à filtrer
    for (CT_StandardItemGroup *groupSource = (CT_StandardItemGroup*) sourceRes->beginGroup(refgroupSourceModel)
          ; groupSource != NULL  && !isStopped()
          ; groupSource = (CT_StandardItemGroup*) sourceRes->nextGroup())
    {
        CT_AffiliationID* idSource = (CT_AffiliationID*) groupSource->findFirstItem(idItemSourceModel);
        CT_AbstractSingularItemDrawable* refItemSource = groupSource->findFirstItem(refItemSourceModel);

        if (idSource!=NULL && refItemSource!=NULL)
        {
            sourceMap.insert(refItemSource, idSource->getValue());
            _sourceList.append(refItemSource);
        }
    }


    QMap<CT_AbstractSingularItemDrawable*, CT_AffiliationID*> targetMap;
    QMap<float, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> > correspondances;

    // Parcours des groupes
    for (CT_StandardItemGroup *groupTarget = (CT_StandardItemGroup*) targetRes->beginGroup(refgroupTargetModel)
          ; groupTarget != NULL  && !isStopped()
          ; groupTarget = (CT_StandardItemGroup*) targetRes->nextGroup())
    {
        CT_AffiliationID* idTarget = (CT_AffiliationID*) groupTarget->findFirstItem(idItemTargetModel);
        CT_AbstractSingularItemDrawable* refItemTarget = groupTarget->findFirstItem(refItemTargetModel);



        if (idTarget!=NULL && refItemTarget!=NULL)
        {
            targetMap.insert(refItemTarget, idTarget);
            _targetList.append(refItemTarget);

            float xTarget = refItemTarget->getCenterX();
            float yTarget = refItemTarget->getCenterY();
            float zTarget = refItemTarget->getCenterZ();
            if (_2Dsearch) {zTarget = 0;}

            QMapIterator<CT_AbstractSingularItemDrawable*, size_t> it(sourceMap);
            while (it.hasNext() && !isStopped())
            {
                it.next();
                float xSource = it.key()->getCenterX();
                float ySource = it.key()->getCenterY();
                float zSource = it.key()->getCenterZ();
                if (_2Dsearch) {zSource = 0;}

                float distance = pow(xTarget - xSource, 2) + pow(yTarget - ySource, 2) + pow(zTarget - zSource, 2);

                correspondances.insertMulti(distance, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*>(refItemTarget, it.key()));
            }
        }
    }

    while (!correspondances.isEmpty())
    {
        QPair<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> &pair = correspondances.begin().value();

        _pairs.insert(pair.first, pair.second);

        QMutableMapIterator<float, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> > it(correspondances);
        while (it.hasNext())
        {
            it.next();
            if ((it.value().first == pair.first) || (it.value().second == pair.second))
            {
                it.remove();
            }
        }
    }

    // Mode manuel
    if (_manualModeActivated)
    {
        m_status = 0;
        requestManualMode();

        m_status = 1;
        requestManualMode();
    }

    // Affectaction des d'identifiants affiliés
    QMapIterator<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> itPairs(_pairs);
    while (itPairs.hasNext())
    {
        itPairs.next();
        CT_AffiliationID *idTarget = targetMap.value(itPairs.key());
        size_t idSource = sourceMap.value(itPairs.value());
        idTarget->setValue(idSource);
    }

}

void PB_StepSetAffiliationIDFromReference::initManualMode()
{
    if(m_doc == NULL)
    {
        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument();
    }

    m_doc->removeAllItemDrawable();

}

void PB_StepSetAffiliationIDFromReference::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(!quit)
        {

            QMessageBox::information(NULL, tr("Modification des affiliations"), tr("Mode manuel."), QMessageBox::Ok);

            PB_ActionModifyAffiliations *action = new PB_ActionModifyAffiliations(&_sourceList, &_targetList, &_pairs);

            getGuiContext()->actionsManager()->removeAction(action->uniqueName());
            getGuiContext()->actionsManager()->addAction(action);
            m_doc->setCurrentAction(action);
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            getGuiContext()->actionsManager()->removeAction("PB_ActionModifyAffiliations");
            getGuiContext()->documentManager()->closeDocument(m_doc);
            m_doc = NULL;

            quitManualMode();
        }
    }
}

