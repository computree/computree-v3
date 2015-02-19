#ifndef PB_STEPFILTERITEMSBYPOSITION_H
#define PB_STEPFILTERITEMSBYPOSITION_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

/*!
 * \class PB_StepFilterItemsByPosition
 * \ingroup Steps_PB
 * \brief <b>Garde les items proches d'une coordonnée.</b>
 *
 * No detailled description for this step
 *
 * \param _xcoord 
 * \param _ycoord 
 * \param _radius 
 *
 */

class PB_StepFilterItemsByPosition: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepFilterItemsByPosition(CT_StepInitializeData &dataInit);

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
    double    _xcoord;
    double    _ycoord;
    double    _radius;

    void recursiveRemoveGroupIfEmpty(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const;

};

#endif // PB_STEPFILTERITEMSBYPOSITION_H
