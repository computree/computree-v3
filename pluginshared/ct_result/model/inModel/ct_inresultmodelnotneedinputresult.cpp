#include "ct_inresultmodelnotneedinputresult.h"

#include "interfaces.h"

CT_InResultModelNotNeedInputResult::CT_InResultModelNotNeedInputResult() : CT_InAbstractResultModel("CT_InResultModelNotNeedInputResult",
                                                                                                    tr("Not need input result"),
                                                                                                    "",
                                                                                                    false)
{
    setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);
}

QString CT_InResultModelNotNeedInputResult::modelTypeDisplayable() const
{
    return QString("CT_InResultModelNotNeedInputResult");
}

CT_InAbstractModel* CT_InResultModelNotNeedInputResult::copy(bool withPossibilities) const
{
    Q_UNUSED(withPossibilities)

    CT_InResultModelNotNeedInputResult *cpy = new CT_InResultModelNotNeedInputResult();
    cpy->setStep(step());
    cpy->setOriginalModel(this);

    return cpy;
}

QList<SettingsNodeGroup*> CT_InResultModelNotNeedInputResult::getAllValues() const
{
    QList<SettingsNodeGroup*> retList = CT_InAbstractResultModel::getAllValues();

    SettingsNodeGroup *root = new SettingsNodeGroup("CT_InResultModelNotNeedInputResult_Values");
    root->addValue(new SettingsNodeValue("Version", 1));
    root->addValue(new SettingsNodeValue("Type", "CT_InResultModelNotNeedInputResult"));
    retList.append(root);

    return retList;
}

bool CT_InResultModelNotNeedInputResult::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    SettingsNodeGroup *root = NULL;

    QListIterator<SettingsNodeGroup*> itS(list);

    while(itS.hasNext()
          && (root == NULL))
    {
        SettingsNodeGroup *gg = itS.next();

        if(gg->name() == "CT_InResultModelNotNeedInputResult_Values")
            root = gg;
    }

    if(root == NULL)
        return false;

    return CT_InAbstractResultModel::setAllValues(list);
}
