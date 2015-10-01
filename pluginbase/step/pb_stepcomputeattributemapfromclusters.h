#ifndef PB_STEPCOMPUTEATTRIBUTEMAPFROMCLUSTERS_H
#define PB_STEPCOMPUTEATTRIBUTEMAPFROMCLUSTERS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/ct_image2d.h"

// Inclusion of auto-indexation system
#include "ct_tools/model/ct_autorenamemodels.h"


class PB_StepComputeAttributeMapFromClusters: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepComputeAttributeMapFromClusters(CT_StepInitializeData &dataInit);

    /*! \brief Step description
     * 
     * Return a description of the step function
     */
    QString getStepDescription() const;

    /*! \brief Step detailled description
     * 
     * Return a detailled description of the step function
     */
    QString getStepDetailledDescription() const;

    /*! \brief Step URL
     * 
     * Return a URL of a wiki for this step
     */
    QString getStepURL() const;

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

    /*! \brief Parameters DialogBox
     * 
     * DialogBox asking for step parameters
     */
    void createPostConfigurationDialog();

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

private:

    double                  _naValue;
    double                  _defaultValue;

    CT_AutoRenameModels    _attMap_ModelName;

};

#endif // PB_STEPCOMPUTEATTRIBUTEMAPFROMCLUSTERS_H
