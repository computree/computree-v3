#include "ct_modelsaverestorehelper.h"

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_ModelSaveRestoreHelper::CT_ModelSaveRestoreHelper()
{
}

SettingsNodeGroup* CT_ModelSaveRestoreHelper::saveToSearchOutModel(const CT_OutAbstractModel *model, const QString &groupName)
{
    if(model == NULL)
        return NULL;

    CT_AbstractModel *originalModel = model->lastOriginalModelWithAStep();

    if((originalModel == NULL)
            || (originalModel->step() == NULL)
            || originalModel->uniqueNamePlusTurn().isEmpty())
        return NULL;

    // search the result (model) that contains this item (model)
    CT_OutAbstractResultModelGroup *mg = NULL;

    QList< QList<CT_OutAbstractResultModelGroup*> > resultsByTurn = originalModel->step()->getAllOutResultModels();
    QListIterator< QList<CT_OutAbstractResultModelGroup*> > itT(resultsByTurn);

    while(itT.hasNext() && (mg == NULL)) {
        QListIterator<CT_OutAbstractResultModelGroup*> itR(itT.next());

        while(itR.hasNext() && (mg == NULL)) {
            CT_OutAbstractResultModelGroup *modelG = itR.next();

            if(modelG->findModelInTree(originalModel->uniqueNamePlusTurn()))
                mg = modelG;
        }
    }

    if((mg == NULL) || mg->uniqueNamePlusTurn().isEmpty())
        return NULL;

    SettingsNodeGroup *root = new SettingsNodeGroup(groupName);

    root->addValue(new SettingsNodeValue("Version", "1"));

    // save the hierarchy of step
    CT_VirtualAbstractStep *step = originalModel->step();
    int i = 0;

    while(step != NULL) {
        SettingsNodeGroup *group = new SettingsNodeGroup(QString("Step%1").arg(i));

        group->addValue(new SettingsNodeValue("stepName", step->getStepName()));
        group->addValue(new SettingsNodeValue("stepPositionInParent", QString("%1").arg(step->parentStep() != NULL ? step->parentStep()->getStepChildList().indexOf(step) : -1)));

        root->addGroup(group);

        step = step->parentStep();
        ++i;
    }

    root->addValue(new SettingsNodeValue("resultModelUniqueName", mg->uniqueNamePlusTurn()));
    root->addValue(new SettingsNodeValue("modelUniqueName", originalModel->uniqueNamePlusTurn()));

    return root;
}

CT_OutAbstractModel* CT_ModelSaveRestoreHelper::searchModelFromSettings(const SettingsNodeGroup *node, const CT_VirtualAbstractStep *step)
{
    if((node == NULL) || (step == NULL))
        return NULL;

    QList<QPair<QString,int> > stepsInfo;

    QList<SettingsNodeGroup*> groups;
    QList<SettingsNodeValue*> values;
    int i = 0;

    // get the hierarchy of step
    while(!(groups = node->groupsByTagName(QString("Step%1").arg(i))).isEmpty())
    {
        values = groups.first()->valuesByTagName("stepName");

        if(values.isEmpty())
            return NULL;

        QString stepName = values.first()->value().toString();

        if(stepName.isEmpty())
            return NULL;

        values = groups.first()->valuesByTagName("stepPositionInParent");

        if(values.isEmpty())
            return NULL;

        bool ok;
        int stepPositionInParent = values.first()->value().toInt(&ok);

        if(!ok)
            return NULL;

        stepsInfo.insert(0, qMakePair(stepName, stepPositionInParent));
        ++i;
    }

    if(stepsInfo.isEmpty())
        return NULL;

    values = node->valuesByTagName("resultModelUniqueName");

    if(values.isEmpty())
        return NULL;

    QString resultModelUniqueName = values.first()->value().toString();

    values = node->valuesByTagName("modelUniqueName");

    if(values.isEmpty())
        return NULL;

    QString modelUniqueName = values.first()->value().toString();

    if(resultModelUniqueName.isEmpty() || modelUniqueName.isEmpty())
        return NULL;

    CT_VirtualAbstractStep *pStep = (CT_VirtualAbstractStep*)step;

    while(pStep->parentStep() != NULL)
        pStep = pStep->parentStep();

    i = 0;

    // verify the hierarchy of step saved
    while(i < stepsInfo.size()) {
        const QPair<QString, int> &pair = stepsInfo.at(i);

        if(i == 0 && pair.second != -1)
            return NULL;

        if(pair.second == -1)
        {
            if(pair.first != pStep->getStepName())
                return NULL;
        }
        else
        {
            if(pair.second < 0 || pair.second >= pStep->getStepChildList().size())
                return NULL;

            pStep = pStep->getStepChildList().at(pair.second);

            if(pair.first != pStep->getStepName())
                return NULL;
        }

        ++i;
    }

    // search the result model that match with the saved
    QList< QList<CT_OutAbstractResultModelGroup*> > l = pStep->getAllOutResultModels();
    QListIterator< QList<CT_OutAbstractResultModelGroup*> > it(l);

    CT_OutAbstractResultModel* resultModel = NULL;

    while(it.hasNext() && (resultModel == NULL)) {

        QList<CT_OutAbstractResultModelGroup*> subList = it.next();
        QListIterator< CT_OutAbstractResultModelGroup* > it2(subList);

        while(it2.hasNext() && (resultModel == NULL)) {

            CT_OutAbstractResultModelGroup *model = it2.next();

            if(model->uniqueNamePlusTurn() == resultModelUniqueName)
                resultModel = model;
        }
    }

    if(resultModel == NULL)
        return NULL;

    // search the model that match with the saved    
    return dynamic_cast<CT_OutAbstractModel*>(resultModel->findModelInTree(modelUniqueName));
}
