#ifndef STEPTESTMODEL1_H
#define STEPTESTMODEL1_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_tools/model/ct_autorenamemodels.h"

#define DEF_SearchInResult          "ri"
#define DEF_SearchInSectionGroup    "sg"
#define DEF_SearchInClusterGroup    "cg"
#define DEF_SearchInPointCluster    "p"

class StepTestModel1 : public CT_AbstractStep
{
    // IMPORTANT in order to obtain step name
    Q_OBJECT

public:

    /*! \brief Step constructor
     *
     * Create a new instance of the step
     *
     * \param dataInit Step parameters object
     */
    StepTestModel1(CT_StepInitializeData &dataInit);

    /*! \brief Step copy
     *
     * Step copy, used when a step is added by step contextual menu
     */
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    /*! \brief Input results specification
     *
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected();

    /*! \brief Output results specification
     *
     * Specification of output results models created by the step (OUT)
     */
    void createOutResultModelListProtected();

    /*! \brief Algorithm of the step
     *
     * Step computation, using input results, and creating output results
     */
    void compute();

public:

    CT_AutoRenameModels                 _autoSearchOutClusterGroup;
    CT_AutoRenameModels                 _autoSearchOutPointCluster;
};

#endif // STEPTESTMODEL1_H
