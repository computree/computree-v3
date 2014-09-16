#include "ct_inabstractmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include <QDebug>

CT_InAbstractModel::CT_InAbstractModel(const QString &uniqueName,
                                       const QString &description,
                                       const QString &displayableName) : CT_AbstractModel(uniqueName,
                                                                                          description,
                                                                                          displayableName)
{
    m_choiceMode = CT_InAbstractModel::C_DontChoose;
    m_finderMode = CT_InAbstractModel::F_IsObligatory;

    m_possibilitiesGroup = new CT_InStdModelPossibilityGroup();
    m_nPossibilitiesWithoutSaving = 0;
}

CT_InAbstractModel::~CT_InAbstractModel()
{
    clearPossibilitiesSaved();

    delete m_possibilitiesGroup;
}

CT_InAbstractModel::ChoiceMode CT_InAbstractModel::choiceMode() const
{
    return m_choiceMode;
}

CT_InAbstractModel::FinderMode CT_InAbstractModel::finderMode() const
{
    return m_finderMode;
}

void CT_InAbstractModel::startSaveCycle()
{
    m_saveCycles.append(CT_InModelSaveCycle());
}

void CT_InAbstractModel::finishSaveCycle()
{
}

int CT_InAbstractModel::nSaveCycle() const
{
    return m_saveCycles.size();
}

bool CT_InAbstractModel::needOutputModel() const
{
    return true;
}

bool CT_InAbstractModel::recursiveFindOnePossibilityInModel(const CT_OutAbstractModel &model,
                                                            bool savePossibilities)
{
    m_nPossibilitiesWithoutSaving = 0;

    return recursiveFindPossibilitiesInModel(model, savePossibilities, false) != 0;
}

bool CT_InAbstractModel::recursiveFindAllPossibilitiesInModel(const CT_OutAbstractModel &model,
                                                              bool savePossibilities)
{
    m_nPossibilitiesWithoutSaving = 0;

    return recursiveFindPossibilitiesInModel(model, savePossibilities, true) != 0;
}

bool CT_InAbstractModel::isAtLeastOnePossibilitySelected() const
{
    // if we have no possibilities : we return false
    if(nPossibilitiesSaved() == 0)
        return false;

    bool ok = false;

    QListIterator<CT_InStdModelPossibility*> itP(m_possibilitiesGroup->getPossibilities());

    while(itP.hasNext())
    {
        CT_InStdModelPossibility *possibility = itP.next();

        if(possibility->isSelected())
            ok = true;
    }

    // if no possibilities of this model was selected : we return false
    if(!ok)
        return false;

    return true;
}

bool CT_InAbstractModel::recursiveIsAtLeastOnePossibilitySelected() const
{
    if(!isAtLeastOnePossibilitySelected())
        return false;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        // if no possibilities of this children (and recursively) is selected : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveIsAtLeastOnePossibilitySelected())
            return false;
    }

    // all it's ok
    return true;
}

bool CT_InAbstractModel::isAtLeastOnePossibilitySelectedIfItDoes() const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    // if this model is optionnal : we return true
    if(finderMode() == CT_InAbstractModel::F_IsOptional)
        return true;

    bool ok = false;

    QListIterator<CT_InStdModelPossibility*> itP(m_possibilitiesGroup->getPossibilities());

    while(itP.hasNext())
    {
        CT_InStdModelPossibility *possibility = itP.next();

        if(possibility->isSelected())
            ok = true;
    }

    // if no possibilities of this model was selected : we return false
    return ok;
}

bool CT_InAbstractModel::recursiveIsAtLeastOnePossibilitySelectedIfItDoes() const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    if(!isAtLeastOnePossibilitySelectedIfItDoes())
        return false;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        // if no possibilities of this children (and recursively) is selected : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveIsAtLeastOnePossibilitySelectedIfItDoes())
            return false;
    }

    // all it's ok
    return true;
}

bool CT_InAbstractModel::canSelectPossibilitiesByDefault() const
{
    // if we have more or less than one possibility : we return false
    if(needOutputModel() && (nPossibilitiesSaved() != 1))
        return false;

    return true;
}

bool CT_InAbstractModel::recursiveCanSelectPossibilitiesByDefault() const
{
    if(!canSelectPossibilitiesByDefault())
        return false;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        // if children (recursively) cannot select possibilities by default : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveCanSelectPossibilitiesByDefault())
            return false;
    }

    return true;
}

bool CT_InAbstractModel::canSelectPossibilitiesByDefault(const QList<int> &possibilitiesIndex, bool selectChildrensTooRecursively) const
{
    int n = possibilitiesIndex.size();
    int nt = nPossibilitiesSaved();

    if(((n > 1) && m_possibilitiesGroup->isExclusive())
            || (nt < n)
            || !m_possibilitiesGroup->isEnable())
        return false;

    // check if an index is not valid
    foreach (int v, possibilitiesIndex) {
        if((v < 0) || (v >= nt))
            return false;
    }

    if(selectChildrensTooRecursively)
    {
        QList<CT_AbstractModel*> l = childrensOfPossibilities();

        // check if a children is not valid
        foreach (int v, possibilitiesIndex) {
            if(!((CT_InAbstractModel*)l.at(v))->recursiveCanSelectPossibilitiesByDefault())
                return false;
        }
    }

    return true;
}

bool CT_InAbstractModel::selectAllPossibilitiesByDefault()
{
    if(!canSelectPossibilitiesByDefault())
        return false;

    const QList<CT_InStdModelPossibility*> &l = m_possibilitiesGroup->getPossibilities();

    if(!l.isEmpty())
        l.first()->setSelected(true);

    return true;
}

bool CT_InAbstractModel::recursiveSelectAllPossibilitiesByDefault()
{
    if(!canSelectPossibilitiesByDefault())
        return false;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        // if children (recursively) cannot select possibilities by default : we return false
        if(!((CT_InAbstractModel*)it.next())->recursiveSelectAllPossibilitiesByDefault())
            return false;
    }

    return selectAllPossibilitiesByDefault();
}

bool CT_InAbstractModel::selectPossibilitiesByDefault(const QList<int> &possibilitiesIndex, bool selectChildrensTooRecursively)
{
    if(!canSelectPossibilitiesByDefault(possibilitiesIndex, selectChildrensTooRecursively))
        return false;

    if(selectChildrensTooRecursively)
    {
        QList<CT_AbstractModel*> l = childrensOfPossibilities();

        foreach (int v, possibilitiesIndex) {
            if(!((CT_InAbstractModel*)l.at(v))->recursiveSelectAllPossibilitiesByDefault())
                return false;
        }
    }

    return true;
}

int CT_InAbstractModel::nPossibilitiesSaved() const
{
    return m_possibilitiesGroup->nPossibilities();
}

QList<CT_InStdModelPossibility *> CT_InAbstractModel::getPossibilitiesSaved() const
{
    return m_possibilitiesGroup->getPossibilities();
}

QList<CT_InStdModelPossibility *> CT_InAbstractModel::getPossibilitiesSavedSelected() const
{
    QList<CT_InStdModelPossibility*> l;

    QListIterator<CT_InStdModelPossibility*> it(m_possibilitiesGroup->getPossibilities());

    while(it.hasNext())
    {
        CT_InStdModelPossibility *p = it.next();

        if(p->isSelected())
            l.append(p);
    }

    return l;
}

void CT_InAbstractModel::clearPossibilitiesSaved()
{
    while(deleteLastSaveCycle());
}

void CT_InAbstractModel::recursiveClearPossibilitiesSaved()
{
    clearPossibilitiesSaved();

    QList<CT_AbstractModel*> child = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(child);

    while(it.hasNext())
        ((CT_InAbstractModel*)it.next())->recursiveClearPossibilitiesSaved();
}

QList<SettingsNodeGroup *> CT_InAbstractModel::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *root = new SettingsNodeGroup("CT_InAbstractModel_Values");
    root->addValue(new SettingsNodeValue("Version", 1));
    root->addValue(new SettingsNodeValue("ModelName", uniqueNamePlusTurn()));
    root->addValue(new SettingsNodeValue("ModelType", metaObject()->className()));
    root->addValue(new SettingsNodeValue("ChoiceMode", (int)choiceMode()));
    root->addValue(new SettingsNodeValue("FinderMode", (int)finderMode()));
    retList.append(root);

    QListIterator<CT_InStdModelPossibility*> it(possibilitiesGroup()->getPossibilities());

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();

        SettingsNodeGroup *groupResPossibility = new SettingsNodeGroup("Possibility");
        groupResPossibility->addValue(new SettingsNodeValue("IsSelected", possibility->isSelected() ? "true" : "false"));
        groupResPossibility->addValue(new SettingsNodeValue("OutModelName", possibility->outModel()->uniqueNamePlusTurn()));
        groupResPossibility->addValue(new SettingsNodeValue("OutModelType", possibility->outModel()->metaObject()->className()));
        root->addGroup(groupResPossibility);
    }

    QList<CT_AbstractModel*> c = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> itC(c);

    int i = 0;

    while(itC.hasNext())
    {
        SettingsNodeGroup *cGroup = new SettingsNodeGroup(QString("Children_%1").arg(i));
        root->addGroup(cGroup);

        QList<SettingsNodeGroup*> groups = ((CT_InAbstractModel*)itC.next())->getAllValues();

        foreach (SettingsNodeGroup *gr, groups) {
            cGroup->addGroup(gr);
        }

        ++i;
    }

    return retList;
}

bool CT_InAbstractModel::setAllValues(const QList<SettingsNodeGroup *> &list)
{
    // TODO : verify compatibility with old script !!!

    SettingsNodeGroup *root = NULL;

    QListIterator<SettingsNodeGroup*> itS(list);

    while(itS.hasNext()
          && (root == NULL))
    {
        SettingsNodeGroup *gg = itS.next();

        if(gg->name() == "CT_InAbstractModel_Values")
            root = gg;
    }

    if(root == NULL)
        return false;

    QList<SettingsNodeValue*> values = root->valuesByTagName("ModelName");

    // Verification du modelName
    if(values.isEmpty()
            || (values.first()->value().toString() != uniqueNamePlusTurn()))
        return false;

    // Verification du choiceMode
    values = root->valuesByTagName("ChoiceMode");

    if(values.isEmpty())
        return false;

    bool ok;
    int choiceM = values.first()->value().toInt(&ok);

    if(!ok || (choiceM != (int)choiceMode()))
        return false;

    // Verification du nombre de possibilité
    QList<SettingsNodeGroup*> groups = root->groupsByTagName("Possibility");

    if(groups.size() != nPossibilitiesSaved())
        return false;

    // Vérification des possibilités
    int i=0;
    QListIterator<CT_InStdModelPossibility*> it(possibilitiesGroup()->getPossibilities());

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();
        SettingsNodeGroup *groupPossibility = groups.at(i);

        // Verification du modelName du outModel de la possibilité
        values = groupPossibility->valuesByTagName("OutModelName");

        if(values.isEmpty()
            || values.first()->value().toString() != possibility->outModel()->uniqueNamePlusTurn())
            return false;

        // Verification et selection de la possibilité
        values = groupPossibility->valuesByTagName("IsSelected");

        if(values.isEmpty())
            return false;

        possibility->setSelected(values.first()->value().toString() == "true");

        ++i;
    }

    QList<CT_AbstractModel*> c = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> itC(c);

    i=0;

    while(itC.hasNext())
    {
        groups = root->groupsByTagName(QString("Children_%1").arg(i));

        if(groups.isEmpty())
            return false;

        if(!((CT_InAbstractModel*)itC.next())->setAllValues(groups.first()->groups()))
            return false;

        ++i;
    }

    return true;
}

bool CT_InAbstractModel::deleteLastSaveCycle()
{
    if(!m_saveCycles.isEmpty())
    {
        CT_InModelSaveCycle cycle = m_saveCycles.takeLast();

        while(!cycle.isEmpty())
        {
            CT_InStdModelPossibility *p = cycle.takeLast();
            m_possibilitiesGroup->removePossibility(p);
            delete p;
        }

        return true;
    }

    return false;
}

void CT_InAbstractModel::addToPossibility(const CT_OutAbstractModel &model)
{
    //qDebug() << uniqueName() << " => " << "p : " << model.uniqueName() << " for " << uniqueName();

    ++m_nPossibilitiesWithoutSaving;

    if(m_saveCycles.isEmpty())
        m_saveCycles.append(CT_InModelSaveCycle());

    CT_InStdModelPossibility *possibility = createNewPossibility();
    possibility->setOutModel(model);

    m_saveCycles.last().append(possibility);
    m_possibilitiesGroup->addPossibility(possibility);

    possibilityCreated(possibility);
}

CT_InStdModelPossibility* CT_InAbstractModel::createNewPossibility() const
{
    return new CT_InStdModelPossibility();;
}

bool CT_InAbstractModel::existModelInPossibilities(const CT_OutAbstractModel &model) const
{
    QListIterator<CT_InStdModelPossibility*> it(possibilitiesGroup()->getPossibilities());

    while(it.hasNext())
    {
        CT_OutAbstractModel *pOutModel = it.next()->outModel();
        if((pOutModel->uniqueName() == model.uniqueName())
                && (pOutModel->step() == model.step()))
            return true;
    }

    return false;
}

void CT_InAbstractModel::internalSetChoiceMode(CT_InAbstractModel::ChoiceMode mode)
{
    m_possibilitiesGroup->setEnable(true);

    if(mode == CT_InAbstractModel::C_ChooseOneIfMultiple)
        m_possibilitiesGroup->setExclusive(true);
    else if(mode == CT_InAbstractModel::C_ChooseMultipleIfMultiple)
        m_possibilitiesGroup->setExclusive(false);
    else if(mode != CT_InAbstractModel::C_DontChoose)
        m_possibilitiesGroup->setEnable(false);

    m_choiceMode = mode;
}

void CT_InAbstractModel::internalSetFinderMode(CT_InAbstractModel::FinderMode mode)
{
    m_finderMode = mode;
}

void CT_InAbstractModel::staticCopyPossibilitiesToModel(const CT_InAbstractModel *src, CT_InAbstractModel *dst)
{
    dst->clearPossibilitiesSaved();

    QListIterator< CT_InModelSaveCycle > it(src->m_saveCycles);

    while(it.hasNext())
    {
        CT_InModelSaveCycle newCycle;
        QListIterator<CT_InStdModelPossibility*> itP(it.next());

        while(itP.hasNext())
        {
            CT_InStdModelPossibility *cpy = itP.next()->copy();

            dst->m_possibilitiesGroup->addPossibility(cpy);
            newCycle.append(cpy);
        }

        dst->m_saveCycles.append(newCycle);

        itP = newCycle;

        while(itP.hasNext())
            dst->possibilityCreated(itP.next());
    }
}

CT_InStdModelPossibilityGroup* CT_InAbstractModel::possibilitiesGroup() const
{
    return m_possibilitiesGroup;
}

int CT_InAbstractModel::recursiveFindPossibilitiesInModel(const CT_OutAbstractModel &model, bool savePossibilities, bool searchMultiple)
{
    if(!needOutputModel())
        return 1;

    int ok = 0;

    //qDebug() << uniqueName() << " => " << "model : " << uniqueName() << " / canBeComparedWith(" << model.uniqueName() << ") ?";

    // if this model can be compared with the out model
    if(canBeComparedWith(model))
    {
        //qDebug() << uniqueName() << " => " << "yes";

        bool recursiveOk = false;

        // we select if we can go down in childrens of the out model and must compare them with this INPUT model
        if(mustCompareThisWithChildrenOfOutModel())
        {
            //qDebug() << uniqueName() << " => " << "recursive down";

            bool continueLoop = true;

            // get children of the output model
            QList<CT_AbstractModel*> child = model.childrens();
            QListIterator<CT_AbstractModel*> it(child);

            // call this method with the next children
            while(it.hasNext()
                  && continueLoop)
            {
                CT_OutAbstractModel *oam = (CT_OutAbstractModel*)it.next();

                // go down
                if(recursiveFindPossibilitiesInModel(*oam, savePossibilities, searchMultiple) != 0)
                {
                    recursiveOk = true;

                    if(!searchMultiple)
                        continueLoop = false;
                }
            }

            if(!continueLoop && (m_nPossibilitiesWithoutSaving > 0))
                return 2;

            if(recursiveOk)
                ok = 2;
        }

        //qDebug() << uniqueName() << " => " << " canAddPossibilityForThisModel(" << model.uniqueName() << ") ?";

        // we select if this model can be a possibility for this model
        if(canAddPossibilityForThisModel(model))
        {
            //qDebug() << uniqueName() << " => " << "yes";

            // get children of this model
            QList<CT_AbstractModel*> iChild = childrensToFindPossibilities(savePossibilities);
            QListIterator<CT_AbstractModel*> itI(iChild);

            bool atLeastOneChildrenIsObligatory = false;

            while(itI.hasNext())
            {
                CT_InAbstractModel *iam = (CT_InAbstractModel*)itI.next();

                // if this model is obligatory we set the atLeastOneObligatory variable to inform that
                // at least one children is obligatory
                if(iam->finderMode() == CT_InAbstractModel::F_IsObligatory)
                    atLeastOneChildrenIsObligatory = true;
            }

            //qDebug() << uniqueName() << " => " << "mustCompareOutModelWithChildrenOfThisModel ?";

            // if we must compare the current ouput model with childrens of this model
            if(mustCompareOutModelWithChildrenOfThisModel())
            {
                //qDebug() << uniqueName() << " => " << "yes";

                bool continueLoop = true;
                QList<bool> deleteLastSaveCycleList;

                itI.toFront();

                // we compare all children model of this model with the current output model
                while(itI.hasNext()
                       && continueLoop)
                {
                    CT_InAbstractModel *iam = (CT_InAbstractModel*)itI.next();
                    bool tmpOk = false;

                    inModelToBeCompared(iam, savePossibilities);

                    // we start a new save cycle
                    iam->startSaveCycle();

                    inModelToBeCompared(iam, &model, savePossibilities);

                    // if output model match criteria for the children and recursively
                    if(iam->recursiveFindPossibilitiesInModel(model, savePossibilities, searchMultiple) != 0)
                        tmpOk = true;
                    else if(iam->finderMode() == CT_InAbstractModel::F_IsObligatory)
                        continueLoop = false;

                    deleteLastSaveCycleList.append(tmpOk);

                    inModelCompared(iam, &model, savePossibilities);

                    // finish the save cycle
                    iam->finishSaveCycle();

                    if(!tmpOk)
                        iam->deleteLastSaveCycle();

                    inModelComparisonResult(iam, tmpOk, savePossibilities);
                }

                if(!continueLoop)
                {
                    itI.toFront();

                    QListIterator<bool> itL(deleteLastSaveCycleList);

                    while(itI.hasNext() && itL.hasNext())
                    {
                        if(itL.next() == true)
                            ((CT_InAbstractModel*)itI.next())->deleteLastSaveCycle();
                    }

                    //qDebug() << uniqueName() << " => " << "one children is not ok but continue";
                }
                else
                {
                    ok = 1;
                }
            }

            //qDebug() << uniqueName() << " => " << "mustCompareChildrenWithChildrenOfOutModel ?";

            // we select if we must compare children of this model with children of the output model
            if(mustCompareChildrenWithChildrenOfOutModel())
            {
                //qDebug() << uniqueName() << " => " << "yes";

                bool allOk = true;
                QList<bool> deleteLastSaveCycleList;

                itI.toFront();

                // get children of the output model
                QList<CT_AbstractModel*> oChild = model.childrens();
                QListIterator<CT_AbstractModel*> itO(oChild);

                // we compare all children model of this model with all children model of the output model
                while(itI.hasNext()
                      && allOk)
                {
                    CT_InAbstractModel *iam = (CT_InAbstractModel*)itI.next();
                    bool tmpOk = false;
                    bool continueLoop = true;

                    inModelToBeCompared(iam, savePossibilities);

                    // we start a new save cycle
                    iam->startSaveCycle();

                    itO.toFront();

                    while(itO.hasNext()
                          && continueLoop)
                    {
                        CT_OutAbstractModel *oam = (CT_OutAbstractModel*)itO.next();

                        inModelToBeCompared(iam, oam, savePossibilities);

                        // if output model match criteria for the children and recursively
                        if(iam->recursiveFindPossibilitiesInModel(*oam, savePossibilities, searchMultiple) != 0)
                        {
                            tmpOk = true;

                            if(!searchMultiple)
                                continueLoop = false;
                        }

                        inModelCompared(iam, oam, savePossibilities);
                    }

                    // finish the save cycle
                    iam->finishSaveCycle();

                    if(!tmpOk)
                    {
                        iam->deleteLastSaveCycle();

                        if(iam->finderMode() == CT_InAbstractModel::F_IsObligatory)
                            allOk = false;
                    }

                    deleteLastSaveCycleList.append(tmpOk);

                    inModelComparisonResult(iam, tmpOk, savePossibilities);
                }

                if(allOk)
                {
                    ok = 1;
                }
                else
                {
                    itI.toFront();

                    QListIterator<bool> itL(deleteLastSaveCycleList);

                    while(itI.hasNext() && itL.hasNext())
                    {
                        if(itL.next() == true)
                            ((CT_InAbstractModel*)itI.next())->deleteLastSaveCycle();
                    }

                    //qDebug() << uniqueName() << " => " << "one children is not ok";
                    //qDebug() << uniqueName() << " => " << "return nPossibilitiesSaved() > 0 : " << m_nPossibilitiesWithoutSaving << " > 0";
                    possibilityNotCreated();

                    return ok;
                    //return (m_nPossibilitiesWithoutSaving > 0) || ok;
                }
            }

            // if this model have no children or if always are optionnal we consider that is ok
            if(!atLeastOneChildrenIsObligatory)
                ok = 1;
        }

        //qDebug() << uniqueName() << " => " << "ok == " << ok;

        // if all criteria are met
        if(ok == 1)
        {
            if(searchMultiple || (nPossibilitiesSaved() == 0))
            {
                // if this model already exist in possibilities
                if(existModelInPossibilities(model))
                    possibilityNotCreated();
                else if(savePossibilities) // if we must save possibilities, we add this model to possibilities
                    addToPossibility(model);
                else if(!savePossibilities)
                {
                    possibilityNotCreated();
                    ++m_nPossibilitiesWithoutSaving;
                }
            }
            else
            {
                possibilityNotCreated();
            }
        }

        // if this model is optionnal we consider that is ok but
        // we don't have saved the model to possibilities
        //if(finderMode() == CT_InAbstractModel::F_IsOptional)
            //ok = true;

        if(recursiveOk)
            ok = 2;
    }

    //qDebug() << uniqueName() << " => " << "return " << ok;

    return ok;
}
