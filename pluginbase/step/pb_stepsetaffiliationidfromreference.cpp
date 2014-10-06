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

#include "ct_itemdrawable/ct_affiliationid.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

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
    return tr("Mise en correspondance d'ID de deux résultats");
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
    CT_InResultModelGroupToCopy *inResultRefCopy = createNewInResultModelForCopy(DEF_SearchInSourceResult, tr("Résultat de référence"), "", true);
    inResultRefCopy->setZeroOrMoreRootGroup();
    inResultRefCopy->addGroupModel("", DEF_SearchInSourceGroup, CT_AbstractItemGroup::staticGetType(), tr("Groupe de référence"), "", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple);
    inResultRefCopy->addItemModel(DEF_SearchInSourceGroup, DEF_SearchInSourceId, CT_AffiliationID::staticGetType(), tr("ID de référence"));
    inResultRefCopy->addItemModel(DEF_SearchInSourceGroup, DEF_SearchInSourceItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item de référence"));

    CT_InResultModelGroupToCopy *inResultAffCopy = createNewInResultModelForCopy(DEF_SearchInTargetResult, tr("Résultat à affilier"), "", true);
    inResultAffCopy->setZeroOrMoreRootGroup();
    inResultRefCopy->addGroupModel("", DEF_SearchInTargetGroup, CT_AbstractItemGroup::staticGetType(), tr("Groupe à affilier"), "", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple);
    inResultRefCopy->addItemModel(DEF_SearchInTargetGroup, DEF_SearchInTargetId, CT_AffiliationID::staticGetType(), tr("ID à affilier"));
    inResultRefCopy->addItemModel(DEF_SearchInTargetGroup, DEF_SearchInTargetItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item à affilier"));
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

    createNewOutResultModelToCopy(DEF_SearchInSourceResult);
    createNewOutResultModelToCopy(DEF_SearchInTargetResult);
}

void PB_StepSetAffiliationIDFromReference::compute()
{
    QList<CT_ResultGroup*> resultList = getOutResultList();
    CT_ResultGroup *sourceRes = resultList.at(0);
    CT_InAbstractGroupModel* refgroupSourceModel = (CT_InAbstractGroupModel*)PS_MODELS->searchModel(DEF_SearchInSourceGroup, sourceRes, this);

    CT_ResultGroup *targetRes = resultList.at(1);
    CT_InAbstractGroupModel* refgroupTargetModel = (CT_InAbstractGroupModel*)PS_MODELS->searchModel(DEF_SearchInTargetGroup, targetRes, this);

    QMap<CT_AbstractSingularItemDrawable*, size_t> sourceMap;

    CT_ResultGroupIterator itR(sourceRes, refgroupSourceModel);

    // Parcours des groupes contenant les scènes à filtrer
    while(itR.hasNext()
          && !isStopped())
    {
        const CT_AbstractItemGroup *groupSource = itR.next();
        CT_AffiliationID* idSource = (CT_AffiliationID*) groupSource->firstItemByINModelName(this, DEF_SearchInSourceItem);
        CT_AbstractSingularItemDrawable* refItemSource = groupSource->firstItemByINModelName(this, DEF_SearchInSourceItem);

        if (idSource!=NULL && refItemSource!=NULL)
        {
            sourceMap.insert(refItemSource, idSource->getValue());
            _sourceList.append(refItemSource);
        }
    }


    QMap<CT_AbstractSingularItemDrawable*, CT_AffiliationID*> targetMap;
    QMap<float, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractSingularItemDrawable*> > correspondances;

    CT_ResultGroupIterator itR2(targetRes, refgroupTargetModel);

    // Parcours des groupes
    while(itR2.hasNext()
          && !isStopped())
    {
        const CT_AbstractItemGroup *groupTarget = itR2.next();
        CT_AffiliationID* idTarget = (CT_AffiliationID*) groupTarget->firstItemByINModelName(this, DEF_SearchInTargetId);
        CT_AbstractSingularItemDrawable* refItemTarget = groupTarget->firstItemByINModelName(this, DEF_SearchInTargetItem);

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

