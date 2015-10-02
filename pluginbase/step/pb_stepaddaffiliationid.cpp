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

#include "pb_stepaddaffiliationid.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_affiliationid.h"

#include "ct_result/ct_resultgroup.h"

#define DEF_SearchInResult      "PB_StepAffIDResult"
#define DEF_SearchInGroup       "PB_StepAffIDg"
#define DEF_SearchInItem        "PB_StepAffIDi"

PB_StepAddAffiliationID::PB_StepAddAffiliationID(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

QString PB_StepAddAffiliationID::getStepDescription() const
{
    return tr("Ajout d'un ID / groupe");
}

QString PB_StepAddAffiliationID::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

CT_VirtualAbstractStep* PB_StepAddAffiliationID::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepAddAffiliationID(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepAddAffiliationID::createInResultModelListProtected()
{
    // search a IN result that will be copied in the list of OUT results of this step
    CT_InResultModelGroupToCopy *res = createNewInResultModelForCopy(DEF_SearchInResult, tr("Résultat à affilier"), tr("Veuillez choisir le résultat auquel vous voulez ajouter un ID"));

    // we want a result with zero or more groups
    res->setZeroOrMoreRootGroup();

    // and we search in the structure a group (addGroupModel with the first parameter
    // equal to "" mean that the group is after the root group defined : here ZeroOrMoreGroups).
    // This mean that the group can be the root group or anywhere in the structure.
    res->addGroupModel("", DEF_SearchInGroup, CT_StandardItemGroup::staticGetType(), tr("Groupe à affilier"));

    // the group must contains at least one CT_AbstractSingularItemDrawable (the user will select the ItemDrawable)
    res->addItemModel(DEF_SearchInGroup,
                      DEF_SearchInItem,
                      CT_AbstractSingularItemDrawable::staticGetType(),
                      tr("Item de référence"),
                      tr("Si cet item est absent, aucun ID ne sera créé"));
}

void PB_StepAddAffiliationID::createPostConfigurationDialog()
{
    //CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
}

void PB_StepAddAffiliationID::createOutResultModelListProtected()
{
    // create a new OUT result that is a copy of the IN result selected by the user
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEF_SearchInResult);

    // in this result we add a CT_AffiliationID to the group named DEF_SearchInGroup. The name of the model of
    // the CT_AffiliationID will be generated automatically by the _outAffiliationIDModelName object.
    res->addItemModel(DEF_SearchInGroup,
                      _outAffiliationIDModelName,
                      new CT_AffiliationID(),
                      tr("ID"));
}

void PB_StepAddAffiliationID::compute()
{
    // on récupère le résultat copié
    CT_ResultGroup *outRes = getOutResultList().first();

    // create a iterator to find groups that user selected with the IN model named DEF_SearchInGroup
    CT_ResultGroupIterator it(outRes, this, DEF_SearchInGroup);

    // iterate over all groups
    while(!isStopped()
          && it.hasNext())
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)it.next();

        // search in group the first item that match with the IN model named DEF_SearchInItem
        CT_AbstractSingularItemDrawable *item = group->firstItemByINModelName(this, DEF_SearchInItem);

        // if the group contains a item
        if(item != NULL)
        {
            // add a CT_AffiliationID to the group, we create the CT_AffiliationID with the name of the OUT model : _outAffiliationIDModelName.completeName()
            group->addItemDrawable( new CT_AffiliationID(_outAffiliationIDModelName.completeName(), outRes) );
        }
    }
}
