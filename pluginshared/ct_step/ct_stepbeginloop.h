#ifndef CT_STEPBEGINLOOP_H
#define CT_STEPBEGINLOOP_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"


class PLUGINSHAREDSHARED_EXPORT CT_StepBeginLoop : public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT
public:
    CT_StepBeginLoop(CT_StepInitializeData &dataInit);

    ~CT_StepBeginLoop();

    QString getStepDescription() const;

    QString getStepDetailledDescription() const;

    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

    virtual bool isSettingsModified() const;

protected:

    virtual void createInResultModelListProtected();

    void createOutResultModelListProtected();
    virtual void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    virtual void createPostConfigurationDialog();
    virtual void createPostConfigurationDialog(CT_StepConfigurableDialog *configDialog);

    void compute();



    virtual void compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group);

private:
    QSharedPointer<CT_Counter>  _counter;
    int                         _nTurns;
};

#endif // CT_STEPBEGINLOOP_H
