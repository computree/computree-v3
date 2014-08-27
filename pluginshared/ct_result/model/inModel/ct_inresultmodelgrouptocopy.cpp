#include "ct_inresultmodelgrouptocopy.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

#include "ct_outresultmodelgrouptocopypossibility.h"

CT_InResultModelGroupToCopy::CT_InResultModelGroupToCopy(const QString &uniqueName,
                                                         CT_InAbstractGroupModel *rootGroupModel,
                                                         const QString &displayableName,
                                                         const QString &description,
                                                         bool recursive) : CT_InResultModelGroup(uniqueName,
                                                                                                 rootGroupModel,
                                                                                                 displayableName,
                                                                                                 description,
                                                                                                 recursive)
{
}

QString CT_InResultModelGroupToCopy::modelTypeDisplayable() const
{
    return QString("CT_InResultModelGroupToCopy");
}

QList<CT_OutResultModelGroupToCopyPossibility*> CT_InResultModelGroupToCopy::getOutResultModelGroupsSelectedToCopy() const
{
    QList<CT_OutResultModelGroupToCopyPossibility*> retList;

    QList<CT_InStdModelPossibility*> checked = getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> it(checked);

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibilityForSearch = it.next();

        // on copie la possibilité
        CT_InStdModelPossibility *possibility = possibilityForSearch->copy();
        // ainsi que son outModel. On fait ça car l'utilisateur va rajouter des choses
        // dans le outModel puis il peut décider qu'il veut changer ses modèles d'entrées et
        // recréer des modèles de sorties. Avant de recréer les modèles de sorties les anciens
        // tours seront supprimés de la mémoire ce qui va supprimer de la mémoire ses possibilités et les
        // nouveaux modèles ajoutés et SURTOUT remettre à zéro les CT_AutoRenameModels !
        possibility->setOutModel(*(possibility->outModel()->copy()));
        possibility->setAutoDeleteOutModel(true);

        CT_OutResultModelGroupToCopyPossibility *copy = new CT_OutResultModelGroupToCopyPossibility();
        // la possibilité sera supprimé de la mémoire lorsque la copy sera supprimé de la mémoire donc pas de problème
        copy->setInModelAndPossibility(this, possibility, possibilityForSearch);

        retList.append(copy);
    }

    return retList;
}

QList<const CT_OutAbstractResultModelGroup*> CT_InResultModelGroupToCopy::getOutResultModelForSearchInModel() const
{
    QList<const CT_OutAbstractResultModelGroup*> list;

    QList<CT_OutResultModelGroupToCopyPossibility*> pList = getOutResultModelGroupsSelectedToCopy();

    while(!pList.isEmpty())
    {
        CT_OutResultModelGroupToCopyPossibility *pToDelete = pList.takeFirst();

        list.append(pToDelete->outModelForSearch());

        delete pToDelete;
    }

    return list;
}

QList<SettingsNodeGroup *> CT_InResultModelGroupToCopy::getAllValues() const
{
    // get values from parent class
    QList<SettingsNodeGroup*> retList = CT_InResultModelGroup::getAllValues();

    // create a new root node
    SettingsNodeGroup *root = new SettingsNodeGroup("CT_InResultModelGroupToCopy_Values");
    root->addValue(new SettingsNodeValue("Version", 1));
    retList.append(root);

    return retList;
}

bool CT_InResultModelGroupToCopy::setAllValues(const QList<SettingsNodeGroup *> &list)
{
    // TODO : verify compatibility with old script !!!

    SettingsNodeGroup *root = NULL;

    QListIterator<SettingsNodeGroup*> itS(list);

    while(itS.hasNext()
          && (root == NULL))
    {
        SettingsNodeGroup *gg = itS.next();

        if(gg->name() == "CT_InResultModelGroupToCopy_Values")
            root = gg;
    }

    if(root == NULL)
        return false;

    return CT_InResultModelGroup::setAllValues(list);
}

CT_InAbstractModel* CT_InResultModelGroupToCopy::copy(bool withPossibilities) const
{
    CT_InAbstractGroupModel *rootCpy = NULL;

    if(rootGroup() != NULL)
        rootCpy = (CT_InAbstractGroupModel*)rootGroup()->copy(false);

    CT_InResultModelGroupToCopy *cpy = new CT_InResultModelGroupToCopy(uniqueName(), rootCpy, displayableName(), description(), isRecursive());
    cpy->setStep(step());

    if(withPossibilities)
        CT_InAbstractModel::staticCopyPossibilitiesToModel(this, cpy);

    return cpy;
}

// PROTECTED //

void CT_InResultModelGroupToCopy::possibilityCreated(CT_InStdModelPossibility *p)
{
    CT_InResultModelGroup::possibilityCreated(p);

    CT_OutAbstractModel *model = p->outModel()->copy();

    // on copie le modèle de sortie de la possibilité (qui est seulement une référence)
    // puisqu'il pourra être modifié
    p->setOutModel(*model);

    // on précise à la possibilité de supprimer son outModel de la mémoire
    p->setAutoDeleteOutModel(true);
}
