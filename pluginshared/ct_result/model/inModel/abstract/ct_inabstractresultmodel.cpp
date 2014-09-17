#include "ct_inabstractresultmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_abstractstepplugin.h"

bool CT_InAbstractResultModel::FORCE_RECURSIVITY = false;

CT_InAbstractResultModel::CT_InAbstractResultModel(const QString &uniqueName,
                                                   const QString &description,
                                                   const QString &displayableName,
                                                   bool recursive) : CT_InAbstractModel(uniqueName,
                                                                                        description,
                                                                                        displayableName)
{
    m_recursive = recursive | FORCE_RECURSIVITY;
    m_backupModel = NULL;
    m_originalModel = NULL;

    setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(1);
}

void CT_InAbstractResultModel::setRecursive(bool r)
{
    setRecursiveWithoutUseForceRecursivity(r | FORCE_RECURSIVITY);
}

bool CT_InAbstractResultModel::isRecursive() const
{
    // if step of this model is a step "model" (not used in a tree)
    if((step() != NULL)
            && (step()->getPlugin()->getStepFromKey(step()->getPlugin()->getKeyForStep(*step())) == step()))
        return m_recursive | FORCE_RECURSIVITY;

    return m_recursive;
}

bool CT_InAbstractResultModel::setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(int n)
{
    return possibilitiesGroup()->setMinimumNumberOfPossibilityThatMustBeSelected(n);
}

int CT_InAbstractResultModel::minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() const
{
    return possibilitiesGroup()->minimumNumberOfPossibilityThatMustBeSelected();
}

bool CT_InAbstractResultModel::setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(int n)
{
    return possibilitiesGroup()->setMaximumNumberOfPossibilityThatCanBeSelected(n);
}

int CT_InAbstractResultModel::maximumNumberOfPossibilityThatCanBeSelectedForOneTurn() const
{
    return possibilitiesGroup()->maximumNumberOfPossibilityThatCanBeSelected();
}

bool CT_InAbstractResultModel::recursiveIsAtLeastOnePossibilitySelectedIfItDoes() const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    if(!isAtLeastOnePossibilitySelectedIfItDoes())
        return false;

    QList<CT_AbstractModel *> r;
    QList<CT_InStdModelPossibility*> lP = getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> itP(lP);

    while(itP.hasNext())
        r.append(((CT_InStdResultModelPossibility*)itP.next())->inModel());

    QListIterator<CT_AbstractModel*> it(r);

    while(it.hasNext())
    {
        // if no possibilities of this children (and recursively) is selected : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveIsAtLeastOnePossibilitySelectedIfItDoes())
            return false;
    }

    // all it's ok
    return true;
}

CT_InAbstractModel* CT_InAbstractResultModel::findModelInTreeOfModelInPossibility(int pIndex, const QString &uniqueName) const
{
    return (CT_InAbstractModel*)((CT_InStdResultModelPossibility*)possibilitiesGroup()->getPossibilities().at(pIndex))->inModel()->findModelInTree(uniqueName);
}

void CT_InAbstractResultModel::setOriginalModel(const CT_InAbstractResultModel *om)
{
    if(m_originalModel != NULL)
        disconnect(m_originalModel, NULL, this, NULL);

    m_originalModel = (CT_InAbstractResultModel*)om;

    if(m_originalModel != NULL)
        connect(m_originalModel, SIGNAL(destroyed()), this, SLOT(originalModelDestroyed()), Qt::DirectConnection);
}

CT_InAbstractResultModel *CT_InAbstractResultModel::originalModel() const
{
    if(m_originalModel == NULL)
        return const_cast<CT_InAbstractResultModel*>(this);

    return m_originalModel;
}

CT_InAbstractResultModel* CT_InAbstractResultModel::recursiveOriginalModel() const
{
    if(m_originalModel == NULL)
        return const_cast<CT_InAbstractResultModel*>(this);

    return m_originalModel->recursiveOriginalModel();
}

void CT_InAbstractResultModel::setRecursiveWithoutUseForceRecursivity(bool e)
{
    m_recursive = e;
}

void CT_InAbstractResultModel::staticSetRecursiveWithoutUseForceRecursivity(CT_InAbstractResultModel *model, bool e)
{
    model->setRecursiveWithoutUseForceRecursivity(e);
}

QList<CT_AbstractModel *> CT_InAbstractResultModel::childrensToFindPossibilities(bool savePossibilities) const
{
    if(!savePossibilities)
        return childrens();

    QList<CT_AbstractModel *> r;
    QList<CT_AbstractModel *> l = childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
        r.append(((CT_InAbstractModel*)it.next())->copy(false));

    return r;
}

QList<CT_AbstractModel *> CT_InAbstractResultModel::childrensOfPossibilities() const
{
    QList<CT_AbstractModel *> r;

    QList<CT_InStdModelPossibility*> l = getPossibilitiesSaved();
    QListIterator<CT_InStdModelPossibility*> it(l);

    while(it.hasNext())
        r.append(((CT_InStdResultModelPossibility*)it.next())->inModel());

    return r;
}

void CT_InAbstractResultModel::inModelComparisonResult(CT_AbstractModel *inModel, bool ok, bool savePossibilities)
{
    CT_InAbstractModel::inModelComparisonResult(inModel, ok, savePossibilities);

    if(savePossibilities)
    {
        delete m_backupModel;
        m_backupModel = NULL;

        if(!ok)
            delete inModel;
        else
            m_backupModel = (CT_InAbstractModel*)inModel;
    }
    else
        m_backupModel = NULL;
}

CT_InStdModelPossibility* CT_InAbstractResultModel::createNewPossibility() const
{
    return new CT_InStdResultModelPossibility();
}

void CT_InAbstractResultModel::possibilityCreated(CT_InStdModelPossibility *p)
{
    if(m_backupModel != NULL)
        dynamic_cast<CT_InStdResultModelPossibility*>(p)->setInModel(m_backupModel);

    m_backupModel = NULL;
}

void CT_InAbstractResultModel::possibilityNotCreated()
{
    delete m_backupModel;
    m_backupModel = NULL;
}

void CT_InAbstractResultModel::originalModelDestroyed()
{
    m_originalModel = NULL;
}
