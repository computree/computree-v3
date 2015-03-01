#ifndef PB_STEPLOADPOSITIONSFORMATCHING_H
#define PB_STEPLOADPOSITIONSFORMATCHING_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

/*!
 * \class PB_StepLoadPositionsForMatching
 * \ingroup Steps_PB
 * \brief <b>Charge deux sources de positions pour mise en correspondance.</b>
 *
 * No detailled description for this step
 *
 * \param _refFileName 
 * \param _transFileName 
 *
 */

class PB_StepLoadPositionsForMatching: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepLoadPositionsForMatching(CT_StepInitializeData &dataInit);

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
    QStringList _neededFields;

    QString _refFileName;
    QString _transFileName;

    bool _refHeader;
    bool _transHeader;

    QString _refSeparator;
    QString _transSeparator;

    QString _refDecimal;
    QString _transDecimal;

    int _refSkip;
    int _transSkip;

    QMap<QString, int> _refColumns;
    QMap<QString, int> _transColumns;

};

#endif // PB_STEPLOADPOSITIONSFORMATCHING_H
