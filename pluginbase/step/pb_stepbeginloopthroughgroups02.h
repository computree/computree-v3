#ifndef PB_STEPBEGINLOOPTHROOUGHGROUPS02_H
#define PB_STEPBEGINLOOPTHROOUGHGROUPS02_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"


class PB_StepBeginLoopThroughGroups02 : public CT_StepBeginLoop
{
    Q_OBJECT
public:
    PB_StepBeginLoopThroughGroups02(CT_StepInitializeData &dataInit);

    ~PB_StepBeginLoopThroughGroups02();

    QString getStepDescription() const;

    QString getStepDetailledDescription() const;

    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);


protected:

    virtual void createInResultModelListProtected();

    virtual void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    virtual void createPostConfigurationDialog(int &nTurns);

    virtual void compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group);

private:
    QList<size_t>               _ids;

    void recursiveRemoveGroupIfEmpty(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const;
};

#endif // PB_STEPBEGINLOOPTHROOUGHGROUPS02_H
