#include "ct_outmodelcopyactionaddmodelitemattributeinitem.h"

#include "ct_generateoutresultmodelname.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h"

#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractitemdrawablemodelout.h"
#include "ct_result/model/outModel/tools/ct_iteratoritemsmodelforresultmodelgroup.h"

CT_OutModelCopyActionAddModelItemAttributeInItem::CT_OutModelCopyActionAddModelItemAttributeInItem(const QString &inItemModelNameToSearch,
                                                                                                   CT_OutAbstractItemAttributeModel *itemAttributeModelToAdd,
                                                                                                   CT_AutoRenameModels &autoRenameItemAttributeModelToAdd)
{
    m_inItemModelNameToSearch = inItemModelNameToSearch;
    m_autoRenameItemModelNameToSearch = NULL;

    Q_ASSERT_X(itemAttributeModelToAdd != NULL, "CT_OutModelCopyActionAddModelItemAttributeInItem constructor", "The item attribute model to add cannot be NULL !");

    m_itemAttributeModelToAdd = itemAttributeModelToAdd;
    m_autoRenameItemAttributeModelToAdd = &autoRenameItemAttributeModelToAdd;
}

CT_OutModelCopyActionAddModelItemAttributeInItem::CT_OutModelCopyActionAddModelItemAttributeInItem(const CT_AutoRenameModels &inItemModelNameToSearch,
                                                                                                   CT_OutAbstractItemAttributeModel *itemAttributeModelToAdd,
                                                                                                   CT_AutoRenameModels &autoRenameItemAttributeModelToAdd)
{
    m_autoRenameItemModelNameToSearch = (CT_AutoRenameModels*)&inItemModelNameToSearch;

    Q_ASSERT_X(itemAttributeModelToAdd != NULL, "CT_OutModelCopyActionAddModelItemAttributeInItem constructor", "The item attribute model to add cannot be NULL !");

    m_itemAttributeModelToAdd = itemAttributeModelToAdd;
    m_autoRenameItemAttributeModelToAdd = &autoRenameItemAttributeModelToAdd;
}

CT_OutModelCopyActionAddModelItemAttributeInItem::~CT_OutModelCopyActionAddModelItemAttributeInItem()
{
    delete m_itemAttributeModelToAdd;
}

bool CT_OutModelCopyActionAddModelItemAttributeInItem::execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy)
{
    // on récupère le résultat modèle de sortie à copier/modifier
    CT_OutAbstractResultModelGroup *outResModel = modelCopy->outModelForModification();

    if(m_autoRenameItemModelNameToSearch == NULL)
    {
        // on récupère l'item défini dans "createInResultModelListProtected()"
        // pour la recherche
        CT_InAbstractSingularItemModel *inItemModel = dynamic_cast< CT_InAbstractSingularItemModel* >(step->getInModelForResearch(modelCopy->outModelForSearch(), m_inItemModelNameToSearch));

        assert(inItemModel != NULL);

        // on recherche tous les items du type qui ont été sélectionnés
        // par l'utilisateur

        CT_IteratorItemsModelForResultModelGroup it(outResModel);

        if(it.recursiveBeginIterateItems(*inItemModel))
        {
            CT_OutAbstractSingularItemModel *outModel;

            while((outModel = it.recursiveNextItem()) != NULL)
            {
                DEF_CT_AbstractItemDrawableModelOut *outItemModel = dynamic_cast<DEF_CT_AbstractItemDrawableModelOut*>(outModel);

                if(outItemModel != NULL)
                {
                    // on passe un objet permettant de renommer l'item au cas où le nom
                    // du modèle existe déjà (obligatoire dans ce cas puisqu'on ne connait pas les noms
                    // des modèles du résultat que l'on copie et il risque d'y avoir conflit)
                    if(!outItemModel->addItemAttribute((CT_OutAbstractItemAttributeModel*)m_itemAttributeModelToAdd->copy(), *m_autoRenameItemAttributeModelToAdd))
                        return false;
                }
            }
        }
    }
    else
    {
        // on récupère l'item déjà ajouté au résultat modèle
        QList<CT_AbstractModel*> models = outResModel->recursiveGetAllModels();
        QListIterator<CT_AbstractModel*> it(models);

        while(it.hasNext())
        {
            CT_AbstractModel *pModel = it.next();

            if(pModel->uniqueName() == m_autoRenameItemModelNameToSearch->completeName())
            {
                DEF_CT_AbstractItemDrawableModelOut *outItemModel = dynamic_cast<DEF_CT_AbstractItemDrawableModelOut*>(pModel);

                assert(outItemModel != NULL);

                if(!outItemModel->addItemAttribute((CT_OutAbstractItemAttributeModel*)m_itemAttributeModelToAdd->copy(), *m_autoRenameItemAttributeModelToAdd))
                    return false;
            }
        }
    }

    return true;
}
