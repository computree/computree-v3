#include "ct_inresultmodelgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"

#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"
#include "ct_attributes/model/inModel/ct_instditemattributemodel.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/ct_resultgroup.h"

CT_InResultModelGroup::CT_InResultModelGroup(const QString &uniqueName,
                                             CT_InAbstractGroupModel *rootGroupModel,
                                             const QString &displayableName,
                                             const QString &description,
                                             bool recursive) : CT_InAbstractResultModel(uniqueName,
                                                                                        description.isEmpty() ? displayableName : description,
                                                                                        displayableName,
                                                                                        recursive)
{
    _group = rootGroupModel;
}

QString CT_InResultModelGroup::modelTypeDisplayable() const
{
    return QString("CT_InResultModelGroup");
}

CT_InResultModelGroup::~CT_InResultModelGroup()
{
    delete _group;
}

CT_InAbstractGroupModel* CT_InResultModelGroup::rootGroup() const
{
    return _group;
}

bool CT_InResultModelGroup::setRootGroup(CT_InAbstractGroupModel *rootGroup)
{
    delete _group;
    _group = rootGroup;
    staticSetParentModelToModel(_group, this);

    return true;
}

bool CT_InResultModelGroup::setZeroOrMoreRootGroup()
{
    CT_InZeroOrMoreGroupModel *model = new CT_InZeroOrMoreGroupModel();

    return setRootGroup(model);
}

bool CT_InResultModelGroup::setRootGroup(const QString &uniqueName,
                                         const QString &displayableName,
                                         const QString &description,
                                         CT_InAbstractGroupModel::GroupChoiceMode choiceMode,
                                         CT_InAbstractGroupModel::GroupFinderMode finderMode)
{
    CT_InStdGroupModel *model = new CT_InStdGroupModel(uniqueName,
                                                       CT_AbstractItemGroup::staticGetType(),
                                                       displayableName,
                                                       description,
                                                       choiceMode,
                                                       finderMode);

    return setRootGroup(model);
}

bool CT_InResultModelGroup::addGroupModel(const QString &parentGroupUniqueName,
                                          const QString &uniqueName,
                                          const QString &groupType,
                                          const QString &displayableName,
                                          const QString &description,
                                          CT_InAbstractGroupModel::GroupChoiceMode choiceMode,
                                          CT_InAbstractGroupModel::GroupFinderMode finderMode)
{
    DEF_CT_AbstractGroupModelIn *rootModel = NULL;

    if(parentGroupUniqueName.isEmpty())
        rootModel = rootGroup();
    else
        rootModel = dynamic_cast<DEF_CT_AbstractGroupModelIn*>(findModelInTree(parentGroupUniqueName));

    if(rootModel == NULL)
        return false;

    CT_InStdGroupModel *newModel = new CT_InStdGroupModel(uniqueName, groupType, displayableName, description, choiceMode, finderMode);

    return rootModel->addGroup(newModel);
}

bool CT_InResultModelGroup::addItemModel(const QString &parentGroupUniqueName,
                                         const QString &uniqueName,
                                         const QString &itemType,
                                         const QString &displayableName,
                                         const QString &description,
                                         CT_InStdSingularItemModel::ChoiceMode choiceMode,
                                         CT_InStdSingularItemModel::FinderMode finderMode)
{
    DEF_CT_AbstractGroupModelIn *rootModel = NULL;

    if(parentGroupUniqueName.isEmpty())
        rootModel = rootGroup();
    else
        rootModel = dynamic_cast<DEF_CT_AbstractGroupModelIn*>(findModelInTree(parentGroupUniqueName));

    if(rootModel == NULL)
        return false;

    CT_InStdSingularItemModel *newModel = new CT_InStdSingularItemModel(uniqueName, itemType, displayableName, description, choiceMode, finderMode);

    return rootModel->addItem(newModel);
}

bool CT_InResultModelGroup::addItemAttributeModel(const QString &parentItemUniqueName,
                                                  const QString &uniqueName,
                                                  const QList<QString> &categoriesType,
                                                  const CT_AbstractCategory::ValueType valueType,
                                                  const QString &displayableName,
                                                  const QString &description,
                                                  CT_InAbstractModel::ChoiceMode choiceMode,
                                                  CT_InAbstractModel::FinderMode finderMode)
{
    DEF_CT_AbstractItemDrawableModelIn *parentModel = dynamic_cast<DEF_CT_AbstractItemDrawableModelIn*>(findModelInTree(parentItemUniqueName));

    if(parentModel == NULL)
        return false;

    CT_InStdItemAttributeModel *newModel = new CT_InStdItemAttributeModel(uniqueName, categoriesType, valueType, displayableName, description, choiceMode, finderMode);

    return parentModel->addItemAttribute(newModel);
}

bool CT_InResultModelGroup::addGroupModel(const QString &parentGroupUniqueName,
                                          CT_AutoRenameModels &autoUniqueName,
                                          const QString &groupType,
                                          const QString &displayableName,
                                          const QString &description,
                                          CT_InAbstractGroupModel::GroupChoiceMode choiceMode,
                                          CT_InAbstractGroupModel::GroupFinderMode finderMode)
{
    DEF_CT_AbstractGroupModelIn *rootModel = NULL;

    if(parentGroupUniqueName.isEmpty())
        rootModel = rootGroup();
    else
        rootModel = dynamic_cast<DEF_CT_AbstractGroupModelIn*>(findModelInTree(parentGroupUniqueName));

    if(rootModel == NULL)
        return false;

    CT_InStdGroupModel *newModel = new CT_InStdGroupModel("", groupType, displayableName, description, choiceMode, finderMode);

    return rootModel->addGroup(newModel, autoUniqueName);
}

bool CT_InResultModelGroup::addItemModel(const QString &parentGroupUniqueName,
                                         CT_AutoRenameModels &autoUniqueName,
                                         const QString &itemType,
                                         const QString &displayableName,
                                         const QString &description,
                                         CT_InAbstractModel::ChoiceMode choiceMode,
                                         CT_InAbstractModel::FinderMode finderMode)
{
    DEF_CT_AbstractGroupModelIn *rootModel = NULL;

    if(parentGroupUniqueName.isEmpty())
        rootModel = rootGroup();
    else
        rootModel = dynamic_cast<DEF_CT_AbstractGroupModelIn*>(findModelInTree(parentGroupUniqueName));

    if(rootModel == NULL)
        return false;

    CT_InStdSingularItemModel *newModel = new CT_InStdSingularItemModel("", itemType, displayableName, description, choiceMode, finderMode);

    return rootModel->addItem(newModel, autoUniqueName);
}

bool CT_InResultModelGroup::addItemAttributeModel(const QString &parentItemUniqueName,
                                                  CT_AutoRenameModels &autoUniqueName,
                                                  const QList<QString> &categoriesType,
                                                  const CT_AbstractCategory::ValueType valueType,
                                                  const QString &displayableName,
                                                  const QString &description,
                                                  CT_InAbstractModel::ChoiceMode choiceMode,
                                                  CT_InAbstractModel::FinderMode finderMode)
{
    DEF_CT_AbstractItemDrawableModelIn *parentModel = dynamic_cast<DEF_CT_AbstractItemDrawableModelIn*>(findModelInTree(parentItemUniqueName));

    if(parentModel == NULL)
        return false;

    CT_InStdItemAttributeModel *newModel = new CT_InStdItemAttributeModel("", categoriesType, valueType, displayableName, description, choiceMode, finderMode);

    return parentModel->addItemAttribute(newModel, autoUniqueName);
}

QList<CT_AbstractModel *> CT_InResultModelGroup::childrens() const
{
    QList<CT_AbstractModel *> l;

    if(rootGroup() != NULL)
        l.append(rootGroup());

    return l;
}

bool CT_InResultModelGroup::isEmpty() const
{
    return (rootGroup() == NULL);
}

QList<CT_AbstractResult*> CT_InResultModelGroup::searchResultsInStepForPossibilitiesSelected(const CT_VirtualAbstractStep *step) const
{
    QList<CT_AbstractResult*> retList;

    // for all possibilities saved
    QListIterator<CT_InStdModelPossibility*> itP(possibilitiesGroup()->getPossibilities());

    while(itP.hasNext())
    {
        CT_InStdModelPossibility *p = itP.next();

        // if this possibility is selected
        if(p->isSelected())
        {
            CT_AbstractResult *res = NULL;

            // if we must search the result recursively
            if(isRecursive())
                res = recursiveSearchResultFromStepForModel(step, (CT_OutAbstractResultModel*)p->outModel()); // we search recursively
            else
                res = searchResultInStepForModel(step, (CT_OutAbstractResultModel*)p->outModel()); // we search only in this step

            // if the result was not found
            if(res == NULL)
            {
                // we return an empty list
                retList.clear();
                return retList;
            }

            retList.append(res);
        }
    }

    return retList;
}

CT_AbstractResult* CT_InResultModelGroup::searchResultsInStepForThisPossibility(const CT_VirtualAbstractStep *step,
                                                                                const CT_InStdModelPossibility *possibility) const
{
    // if this possibility is not from this model
    if(possibilitiesGroup()->getPossibilities().indexOf((CT_InStdModelPossibility*)possibility) == -1)
        return NULL;

    CT_AbstractResult *res = NULL;

    // if we must search the result recursively
    if(isRecursive())
        res = recursiveSearchResultFromStepForModel(step, (CT_OutAbstractResultModel*)possibility->outModel()); // we search recursively
    else
        res = searchResultInStepForModel(step, (CT_OutAbstractResultModel*)possibility->outModel());// we search only in this step

    // return the founded result
    return res;
}

QList<SettingsNodeGroup*> CT_InResultModelGroup::getAllValues() const
{
    // get values from parent class
    QList<SettingsNodeGroup*> retList = CT_InAbstractResultModel::getAllValues();

    // create a new root node
    SettingsNodeGroup *root = new SettingsNodeGroup("CT_InResultModelGroup_Values");
    root->addValue(new SettingsNodeValue("Version", 1));
    root->addValue(new SettingsNodeValue("GroupMiniNPSelect", possibilitiesGroup()->minimumNumberOfPossibilityThatMustBeSelected()));
    root->addValue(new SettingsNodeValue("GroupMaxiNPSelect", possibilitiesGroup()->maximumNumberOfPossibilityThatCanBeSelected()));
    retList.append(root);

    return retList;
}

bool CT_InResultModelGroup::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    // TODO : verify compatibility with old script !!!

    SettingsNodeGroup *root = NULL;

    QListIterator<SettingsNodeGroup*> itS(list);

    while(itS.hasNext()
          && (root == NULL))
    {
        SettingsNodeGroup *gg = itS.next();

        if(gg->name() == "CT_InResultModelGroup_Values")
            root = gg;
    }

    if(root == NULL)
        return false;

    // Verification du nombre de possibilité à sélectionner maximum
    QList<SettingsNodeValue*> values = root->valuesByTagName("GroupMaxiNPSelect");

    if(values.isEmpty())
        return false;

    bool ok = false;
    int nMaxi = values.first()->value().toInt(&ok);

    if(!ok || (nMaxi < possibilitiesGroup()->maximumNumberOfPossibilityThatCanBeSelected()))
        return false;

    values = root->valuesByTagName("GroupMiniNPSelect");

    if(values.isEmpty())
        return false;

    int nMini = values.first()->value().toInt(&ok);

    if(!ok || (nMini > possibilitiesGroup()->minimumNumberOfPossibilityThatMustBeSelected()))
        return false;

    return CT_InAbstractResultModel::setAllValues(list);
}

CT_InAbstractModel* CT_InResultModelGroup::copy(bool withPossibilities) const
{
    CT_InAbstractGroupModel *rootCpy = NULL;

    if(rootGroup() != NULL)
        rootCpy = (CT_InAbstractGroupModel*)rootGroup()->copy(false);

    CT_InResultModelGroup *cpy = new CT_InResultModelGroup(uniqueName(), rootCpy, description(), displayableName(), isRecursive());
    cpy->setStep(step());

    if(withPossibilities)
        CT_InAbstractModel::staticCopyPossibilitiesToModel(this, cpy);

    return cpy;
}

bool CT_InResultModelGroup::canBeComparedWith(const CT_OutAbstractModel &model) const
{
    // if the OUTPUT model represent a CT_ResultGroup it's ok !
    return (dynamic_cast<const CT_OutAbstractResultModelGroup*>(&model) != NULL);
}

bool CT_InResultModelGroup::canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const
{
    // an INPUT result model accept all OUTPUT result model that represent a CT_ResultGroup to be a possibility
    return canBeComparedWith(model);
}

// PROTECTED //

CT_AbstractResult* CT_InResultModelGroup::recursiveSearchResultFromStepForModel(const CT_VirtualAbstractStep *step,
                                                                                const CT_OutAbstractResultModel *model) const
{
    if(step == NULL)
        return NULL;

    if(step == model->step())
        return searchResultInStepForModel(step, model);

    return recursiveSearchResultFromStepForModel(step->parentStep(), model);
}

CT_AbstractResult* CT_InResultModelGroup::searchResultInStepForModel(const CT_VirtualAbstractStep *step,
                                                                     const CT_OutAbstractResultModel *model) const
{
    if((step == NULL)
            || (step != model->step()))
        return NULL;

    QList<CT_ResultGroup*> list = step->getResults();

    QListIterator<CT_ResultGroup*> it(list);

    while(it.hasNext())
    {
        CT_ResultGroup *res = it.next();

        // compare uniqueNamePlusTurn for result model (only result model have a turn)
        if(res->model()->uniqueNamePlusTurn() == model->uniqueNamePlusTurn())
            return res;
    }

    return NULL;
}
