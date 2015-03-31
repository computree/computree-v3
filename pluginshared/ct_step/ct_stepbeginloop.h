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

    virtual QString getStepDescription() const;

    virtual QString getStepDetailledDescription() const;

    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

    virtual bool isSettingsModified() const;

protected:
    QSharedPointer<CT_Counter>  _counter;


    virtual void createInResultModelListProtected();

    void createOutResultModelListProtected();
    virtual void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    void createPostConfigurationDialog();
    virtual void createPostConfigurationDialog(int &nTurns);

    void compute();

    virtual void compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group);

    inline QSharedPointer<CT_Counter> getCounter() const {return _counter;}

    void NTurnsSelected();

private:
    int                         _nTurns;
};

#endif // CT_STEPBEGINLOOP_H
