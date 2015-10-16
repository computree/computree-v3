#ifndef PB_STEPSELECTBBOXBYFILENAME_H
#define PB_STEPSELECTBBOXBYFILENAME_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_view/tools/ct_textfileconfigurationdialog.h"
#include "ct_tools/model/ct_autorenamemodels.h"


class PB_StepSelectBBoxByFileName: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepSelectBBoxByFileName(CT_StepInitializeData &dataInit);

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

    void createOutResultModelListProtected();

    void compute();


private:

    CT_AutoRenameModels _outBBox_ModelName;
};

#endif // PB_STEPSELECTBBOXBYFILENAME_H
