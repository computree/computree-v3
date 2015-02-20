#ifndef PB_STEPBEGINLOOPTHROOUGHDATASOURCE_H
#define PB_STEPBEGINLOOPTHROOUGHDATASOURCE_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"


class PB_StepBeginLoopThroughDataSource : public CT_StepBeginLoop
{
    Q_OBJECT
public:
    PB_StepBeginLoopThroughDataSource(CT_StepInitializeData &dataInit);

    ~PB_StepBeginLoopThroughDataSource();

    QString getStepDescription() const;

    QString getStepDetailledDescription() const;

    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);


protected:

    virtual void createInResultModelListProtected();

    virtual void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    virtual void createPostConfigurationDialog(int &nTurns);

    virtual void compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group);

private:
    DEF_CT_AbstractGroupModelOut*   _outModel;

    QList<CT_OutStdSingularItemModel*>  _itemModels;
    QList<CT_OutStdGroupModel*>         _groupModels;


};

#endif // PB_STEPBEGINLOOPTHROOUGHDATASOURCE_H
