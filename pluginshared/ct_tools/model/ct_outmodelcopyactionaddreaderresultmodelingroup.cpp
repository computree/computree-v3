#include "ct_outmodelcopyactionaddreaderresultmodelingroup.h"

#include "ct_virtualabstractstep.h"
#include "ct_outresultmodelgroup.h"
#include "ct_outresultmodelgrouptocopypossibility.h"

#include "ct_result/model/outModel/tools/ct_iteratorgroupsmodelforresultmodelgroup.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_reader/abstract/ct_abstractreader.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_OutModelCopyActionAddReaderResultModelInGroup::CT_OutModelCopyActionAddReaderResultModelInGroup(const QString &inParentGroupModelName,
                                                                                                   const QString &inReaderModelName)
{
    _inParentGroupModelName = inParentGroupModelName;
    _inReaderModelName = inReaderModelName;
    _reader = NULL;
    _inAutoRenameGroupModel = NULL;
}

CT_OutModelCopyActionAddReaderResultModelInGroup::CT_OutModelCopyActionAddReaderResultModelInGroup(const CT_AutoRenameModels &inParentGroupModelAutoRename,
                                                                                                   CT_AbstractReader *reader)
{
    _inAutoRenameGroupModel = (CT_AutoRenameModels*)&inParentGroupModelAutoRename;
    _reader = reader;
}

CT_OutModelCopyActionAddReaderResultModelInGroup::~CT_OutModelCopyActionAddReaderResultModelInGroup()
{
    QHashIterator<CT_AbstractModel*, QHash<QString, CT_AutoRenameModels*> > it(m_autoRenameLinks);

    while(it.hasNext()) {
        const QHash<QString, CT_AutoRenameModels*> &h = it.next().value();
        qDeleteAll(h.begin(), h.end());
    }
}

bool addItem(DEF_CT_AbstractGroupModelOut *outModel, CT_OutAbstractSingularItemModel *itemModel, QHash<QString, CT_AutoRenameModels*> &autoRenameLinks)
{
    // on crée un objet permettant de renommer l'item au cas où le nom
    // du modèle existe déjà (obligatoire dans ce cas puisqu'on ne connait pas les noms
    // des modèles du résultat que l'on copie et il risque d'y avoir conflit)
    CT_AutoRenameModels* autoRename = new CT_AutoRenameModels();

    // on le garde en mémoire en faisant un lien entre le nom unique du modèle du reader et l'objet autorename nouvellement créé
    autoRenameLinks.insert(itemModel->uniqueName(), autoRename);

    // et on ajoute l'item
    if(!outModel->addItem((CT_OutAbstractSingularItemModel*)itemModel->copy(), *autoRename))
        return false;

    return true;
}

bool recursiveAddGroup(DEF_CT_AbstractGroupModelOut *outModel, CT_OutStdGroupModel *groupModel, QHash<QString, CT_AutoRenameModels*> &autoRenameLinks)
{
    CT_OutStdGroupModel* groupModelCopy = (CT_OutStdGroupModel*)groupModel->copy();
    groupModelCopy->clear();

    // on crée un objet permettant de renommer le groupe au cas où le nom
    // du modèle existe déjà (obligatoire dans ce cas puisqu'on ne connait pas les noms
    // des modèles du résultat que l'on copie et il risque d'y avoir conflit)
    CT_AutoRenameModels* autoRename = new CT_AutoRenameModels();

    // on le garde en mémoire en faisant un lien entre le nom unique du modèle du reader et l'objet autorename nouvellement créé
    autoRenameLinks.insert(groupModelCopy->uniqueName(), autoRename);

    // et on ajoute l'item
    if(!outModel->addGroup(groupModelCopy, *autoRename))
        return false;

    // ajout des groupe fils
    QListIterator<DEF_CT_AbstractGroupModelOut*> it(groupModel->groups());

    while(it.hasNext()) {
        CT_OutStdGroupModel *groupModelChild = dynamic_cast<CT_OutStdGroupModel*>(it.next());

        assert(groupModelChild != NULL);

        if(!recursiveAddGroup(groupModelCopy, groupModelChild, autoRenameLinks))
            return false;
    }

    // ajout des items fils
    QListIterator<CT_OutAbstractSingularItemModel*> itI(groupModel->items());

    while(itI.hasNext()) {
        if(!addItem(groupModelCopy, itI.next(), autoRenameLinks))
            return false;
    }

    return true;
}

bool CT_OutModelCopyActionAddReaderResultModelInGroup::staticAddElementOfReaderToGroup(CT_AbstractReader *reader,
                                                                                       DEF_CT_AbstractGroupModelOut *outGroupModel,
                                                                                       QHash<CT_AbstractModel*, QHash<QString, CT_AutoRenameModels*> > &outLinks)
{
    QHash<QString, CT_AutoRenameModels*> autoRenameLinks;

    QList<CT_OutStdGroupModel*> groupFromReaderModels = reader->outGroupsModel();

    // ajout des groupes en récursif
    QListIterator<CT_OutStdGroupModel*> itG(groupFromReaderModels);

    while (itG.hasNext()) {
        if(!recursiveAddGroup(outGroupModel, itG.next(), autoRenameLinks)) {
            outLinks.insert(outGroupModel, autoRenameLinks);
            return false;
        }
    }

    // ajout des items
    QList<CT_OutStdSingularItemModel*> itemFromReaderModels = reader->outItemDrawableModels();

    QListIterator<CT_OutStdSingularItemModel*> itIM(itemFromReaderModels);
    while (itIM.hasNext())
    {
        CT_OutStdSingularItemModel* itemModel = itIM.next();

        if(!addItem(outGroupModel, itemModel, autoRenameLinks)) {
            outLinks.insert(outGroupModel, autoRenameLinks);
            return false;
        }
    }

    outLinks.insert(outGroupModel, autoRenameLinks);
    return true;
}

bool CT_OutModelCopyActionAddReaderResultModelInGroup::execute(const CT_VirtualAbstractStep *step,
                                                               CT_OutResultModelGroupToCopyPossibility *modelCopy)
{
    // on récupère le résultat modèle de sortie à copier/modifier
    CT_OutAbstractResultModelGroup *outResModel = modelCopy->outModelForModification();

    CT_AbstractReader* reader = _reader;
    DEF_CT_AbstractGroupModelIn *inGroupModel = NULL;

    if(_inAutoRenameGroupModel == NULL) {
        // on récupère le groupe défini dans "createInResultModelListProtected()"
        // pour la recherche
        inGroupModel = dynamic_cast< DEF_CT_AbstractGroupModelIn* >(PS_MODELS->searchModel(_inParentGroupModelName, modelCopy->outModelForSearch(), step));

        assert(inGroupModel != NULL);

        // on récupère le modèle in du ct_itemreader défini dans "createInResultModelListProtected()"
        // pour la recherche
        CT_InAbstractSingularItemModel *inReaderModel = dynamic_cast< CT_InAbstractSingularItemModel* >(PS_MODELS->searchModel(_inReaderModelName, modelCopy->outModelForSearch(), step));

        assert(inReaderModel != NULL);
        assert(!inReaderModel->getPossibilitiesSavedSelected().isEmpty());

        // on récupère sa première possibilité (on ne peut utiliser plusieurs possiblités ici !)
        CT_OutAbstractSingularItemModel *outReaderModel = dynamic_cast< CT_OutAbstractSingularItemModel* >(inReaderModel->getPossibilitiesSavedSelected().first()->outModel());

        assert(outReaderModel != NULL);

        // on récupère le ct_itemreader prototype
        CT_ReaderItem *readerItemPrototype = dynamic_cast<CT_ReaderItem*> (outReaderModel->itemDrawable());

        assert(readerItemPrototype != NULL);

        // on récupère le reader qu'il utilise
        reader = readerItemPrototype->getReader();

        assert(reader != NULL);
    }

    // et on lui demande de créer ses éléments de sortie (ATTENTION son filepath et ses paramètres doivent ête configuré pour que ça fonctionne !)
    if(reader->outItemDrawableModels().isEmpty() && reader->outGroupsModel().isEmpty())
        reader->createOutItemDrawableModelList();

    if(_inAutoRenameGroupModel == NULL) {

        // on recherche tous les groupes du type qui ont été sélectionnés
        // par l'utilisateur
        CT_IteratorGroupsModelForResultModelGroup it(outResModel);

        if(it.recursiveBeginIterateGroups(inGroupModel))
        {
            DEF_CT_AbstractGroupModelOut *outGroupModel;

            while((outGroupModel = it.recursiveNextGroup()) != NULL)
            {
                if(!staticAddElementOfReaderToGroup(reader, outGroupModel, m_autoRenameLinks)) {
                    return false;
                }
            }
        }
    } else {
        // on récupère le groupe déjà ajouter au résultat modèle
        QList<CT_AbstractModel*> models = outResModel->recursiveGetAllModels();
        QListIterator<CT_AbstractModel*> it(models);

        while(it.hasNext())
        {
            CT_AbstractModel *pModel = it.next();

            if(pModel->uniqueName() == _inAutoRenameGroupModel->completeName())
            {
                DEF_CT_AbstractGroupModelOut *outGroupModel = dynamic_cast<DEF_CT_AbstractGroupModelOut*>(pModel);

                assert(outGroupModel != NULL);

                if(!staticAddElementOfReaderToGroup(reader, outGroupModel, m_autoRenameLinks)) {
                    return false;
                }
            }
        }
    }

    return true;
}

QHash<CT_AbstractModel *, QHash<QString, CT_AutoRenameModels *> > CT_OutModelCopyActionAddReaderResultModelInGroup::takeAutoRenameLinksByGroupModel()
{
    QHash<CT_AbstractModel *, QHash<QString, CT_AutoRenameModels *> > h = m_autoRenameLinks;
    m_autoRenameLinks.clear();

    return h;
}
