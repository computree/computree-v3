#ifndef PB_STEPMATCHITEMSPOSITIONS_H
#define PB_STEPMATCHITEMSPOSITIONS_H

#include "ct_step/abstract/ct_abstractstep.h"

/*!
 * \class PB_StepMatchItemsPositions
 * \ingroup Steps_PB
 * \brief <b>Co-registration entre deux ensembles de positions 2D.</b>
 *
 * No detailled description for this step
 *
 * \param _distThreshold 
 * \param _relativeSizeThreshold 
 * \param _minRelativeSize 
 *
 */

class PB_StepMatchItemsPositions: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepMatchItemsPositions(CT_StepInitializeData &dataInit);

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

    // Step parameters
    double    _distThreshold;    /*!<  */
    double    _relativeSizeThreshold;    /*!<  */
    double    _minRelativeSize;    /*!<  */
    double     _coef_nbRwc;
    double     _coef_nbTwc;
    double     _coef_nbSim;

};

#endif // PB_STEPMATCHITEMSPOSITIONS_H
