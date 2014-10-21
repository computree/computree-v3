#ifndef CT_STEPENDLOOP_H
#define CT_STEPENDLOOP_H

#include "ct_step/abstract/ct_abstractstep.h"


class PLUGINSHAREDSHARED_EXPORT CT_StepEndLoop : public CT_AbstractStep
{
    Q_OBJECT
public:
    CT_StepEndLoop(CT_StepInitializeData &dataInit);

    ~CT_StepEndLoop();

    QString getStepDescription() const;

    QString getStepDetailledDescription() const;

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

    bool mustRecheckTree() const;

protected:

    void createInResultModelListProtected();

    void createOutResultModelListProtected();

    void compute();

private:
    bool        _mustRecheckTree;

};

#endif // CT_STEPENDLOOP_H
