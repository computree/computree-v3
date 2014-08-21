#include "ct_abstractitemgroup.h"

#include "ct_itemdrawable/ct_container.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"

#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"

CT_AbstractItemGroup::CT_AbstractItemGroup() : CT_AbstractItemDrawable()
{
    _parentGroup = NULL;
    _context = NULL;
    _removedLater = Nothing;
    m_parentContainer = NULL;
}

CT_AbstractItemGroup::CT_AbstractItemGroup(const CT_OutAbstractGroupModel *model,
                                           const CT_AbstractResult *result) : CT_AbstractItemDrawable(model, result)
{
    _parentGroup = NULL;
    _context = NULL;
    _removedLater = Nothing;
    m_parentContainer = NULL;
}

CT_AbstractItemGroup::CT_AbstractItemGroup(const QString &modelName,
                                           const CT_AbstractResult *result) : CT_AbstractItemDrawable(modelName, result)
{
    _parentGroup = NULL;
    _context = NULL;
    _removedLater = Nothing;
    m_parentContainer = NULL;
}

CT_AbstractItemGroup::~CT_AbstractItemGroup()
{
}

QString CT_AbstractItemGroup::getType() const
{
    return staticGetType();
}

QString CT_AbstractItemGroup::staticGetType()
{
    return CT_AbstractItemDrawable::staticGetType() + "/CT_AbstractItemGroup";
}

bool CT_AbstractItemGroup::containsItemDrawableByModelName(const CT_VirtualAbstractStep *step,
                                                           const QString &modelName) const
{
    bool isIN;
    CT_AbstractModel *model = getModelInStep(step, modelName, isIN);

    Q_ASSERT_X(model != NULL, "CT_AbstractItemGroup containsItemDrawableByModelName", "You search a item with a modelName but the model was not found");

    if(isIN)
    {
        CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);

        // check if the IN model is a ItemDrawable type
        Q_ASSERT_X(inModel != NULL, "CT_AbstractItemGroup containsItemDrawableByModelName", "You search a item with a IN model but it was not a item model");

        return containsItemDrawable(inModel);
    }

    CT_OutAbstractSingularItemModel *outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model);

    // check if the OUT model is a ItemDrawable type
    Q_ASSERT_X(outModel != NULL, "CT_AbstractItemGroup containsItemDrawableByModelName", "You search a item with a OUT model but it was not a item model");

    return containsItemDrawable(outModel);
}

CT_AbstractSingularItemDrawable* CT_AbstractItemGroup::itemByOUTModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const
{
    bool isIN;
    CT_AbstractModel *model = getModelInStep(step, modelName, isIN);

    Q_ASSERT_X(model != NULL, "CT_AbstractItemGroup itemByModelName", "You search a item with a modelName but the model was not found");

    if(isIN)
    {
        CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);

        // check if the IN model is a ItemDrawable type
        Q_ASSERT_X(inModel != NULL, "CT_AbstractItemGroup itemByModelName", "You search a item with a IN model but it was not a item model");

        return firstItem(inModel);
    }

    CT_OutAbstractSingularItemModel *outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model);

    // check if the OUT model is a ItemDrawable type
    Q_ASSERT_X(outModel != NULL, "CT_AbstractItemGroup itemByModelName", "You search a item with a OUT model but it was not a item model");

    return item(outModel);
}

QList<CT_AbstractSingularItemDrawable *> CT_AbstractItemGroup::itemsByINModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const
{
    bool isIN;
    CT_AbstractModel *model = getModelInStep(step, modelName, isIN);

    Q_ASSERT_X(model != NULL, "CT_AbstractItemGroup itemsByModelName", "You search items with a modelName but the model was not found");

    if(isIN)
    {
        CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);

        // check if the IN model is a ItemDrawable type
        Q_ASSERT_X(inModel != NULL, "CT_AbstractItemGroup itemsByModelName", "You search items with a IN model but it was not a item model");

        return items(inModel);
    }

    CT_OutAbstractSingularItemModel *outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model);

    // check if the OUT model is a ItemDrawable type
    Q_ASSERT_X(outModel != NULL, "CT_AbstractItemGroup itemsByModelName", "You search items with a OUT model but it was not a item model");

    QList<CT_AbstractSingularItemDrawable *> l;
    l.append(item(outModel));

    return l;
}

CT_AbstractSingularItemDrawable* CT_AbstractItemGroup::firstItemByINModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const
{
    bool isIN;
    CT_AbstractModel *model = getModelInStep(step, modelName, isIN);

    Q_ASSERT_X(model != NULL, "CT_AbstractItemGroup firstItemByModelName", "You search the first item with a modelName but the model was not found");

    if(isIN)
    {
        CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);

        // check if the IN model is a ItemDrawable type
        Q_ASSERT_X(inModel != NULL, "CT_AbstractItemGroup firstItemByModelName", "You search the first item with a IN model but it was not a item model");

        return firstItem(inModel);
    }

    CT_OutAbstractSingularItemModel *outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model);

    // check if the OUT model is a ItemDrawable type
    Q_ASSERT_X(outModel != NULL, "CT_AbstractItemGroup firstItemByModelName", "You search the first item with a OUT model but it was not a item model");

    return item(outModel);
}

CT_AbstractItemGroup* CT_AbstractItemGroup::parentGroup() const
{
    return _parentGroup;
}

void CT_AbstractItemGroup::setParentGroup(const CT_AbstractItemGroup *pGroup)
{
    _parentGroup = (CT_AbstractItemGroup*)pGroup;
    setItemDrawableParent(_parentGroup);
}

void CT_AbstractItemGroup::setParentContainer(const CT_Container *container)
{
    m_parentContainer = (CT_Container*)container;
}

CT_Container* CT_AbstractItemGroup::parentContainer() const
{
    return m_parentContainer;
}

void CT_AbstractItemGroup::setContext(const void *context)
{
    _context = (void*)context;
}

void* CT_AbstractItemGroup::getContext() const
{
    return _context;
}

bool CT_AbstractItemGroup::willBeRemovedLater() const
{
    return _removedLater.testFlag(RemoveLater);
}

bool CT_AbstractItemGroup::atLeastOneChildMustBeRemovedLater() const
{
    return _removedLater.testFlag(ChildRemoveLater);
}

void CT_AbstractItemGroup::setBeRemovedLater()
{
    _removedLater |= RemoveLater;

    if((parentGroup() != NULL) && !parentGroup()->atLeastOneChildMustBeRemovedLater())
        parentGroup()->setAtLeastOneChildMustBeRemovedLater();

    if(parentContainer() != NULL)
        parentContainer()->setItemDrawableBeRemovedLater(this);
}

void CT_AbstractItemGroup::setAtLeastOneChildMustBeRemovedLater()
{
    if(parentGroup() != NULL)
        parentGroup()->setAtLeastOneChildMustBeRemovedLater();

    _removedLater |= ChildRemoveLater;
}

CT_AbstractModel* CT_AbstractItemGroup::getModelInStep(const CT_VirtualAbstractStep *step,
                                                       const QString &modelName,
                                                       bool &isAInModel) const
{
    isAInModel = false;

    // if the result is a OUT result of the current step
    if(((CT_AbstractResult*)result())->parentStep() == step)
    {
        // search a out model
        CT_OutAbstractItemModel *outAbsModel = dynamic_cast<CT_OutAbstractItemModel*>(step->getOutModelForCreation((CT_ResultGroup*)result(), modelName));

        // if we don't found a OUT model
        if(outAbsModel == NULL)
        {
            // check if the result is a copy
            CT_OutResultModelGroupCopy *outCopyModel = dynamic_cast<CT_OutResultModelGroupCopy*>(((CT_AbstractResult*)result())->model());

            if(outCopyModel != NULL)
            {
                // get the IN result model that correspond to this out model copy
                const CT_InResultModelGroupToCopy *inResultCopyModel = outCopyModel->inResultModelCopy();

                isAInModel = true;

                // search a IN model
                return step->getInModelForResearchIfUseCopy(inResultCopyModel->uniqueName(), modelName);
            }

            return NULL;
        }

        return outAbsModel;
    }

    // else the result is a IN result of the step
    isAInModel = true;

    // search a in model
    return step->getInModelForResearch((CT_ResultGroup*)result(), modelName);
}

QString CT_AbstractItemGroup::internalVerifyModel(const CT_OutAbstractModel *model) const
{
    if(dynamic_cast<const CT_OutAbstractGroupModel*>(model) == NULL)
        return tr("Model passed in parameter is not a CT_OutAbstractGroupModel");

    return QString();
}
