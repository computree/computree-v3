#ifndef PB_STEPCOMPUTEPOINTSMETRICS_H
#define PB_STEPCOMPUTEPOINTSMETRICS_H

#include "ct_step/abstract/ct_abstractstep.h"

/*!
 * \class PB_StepComputePointsMetrics
 * \ingroup Steps_PB
 * \brief <b>Calcul de métriques à partir de nuages de points.</b>
 *
 * Cette étape gère plusieurs sortes d'éléments :
- Une(des) scène(s) d'entrée
- Un gestionnaire de placettes (enprises dans un fichier, génération auto...)
- Un gestionnaire de Filtres de points (CT_AbstractXYZFilter)
- Un gestionnaire de Métriques de points (CT_AbstractXYZMetric)

Elle est conçue pour fonctionner au sein d'une boucle de script, et donc garder en mémoire les placettes incomplètes à un tour de boucle. 
 *
 *
 */

class PB_StepComputePointsMetrics: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepComputePointsMetrics(CT_StepInitializeData &dataInit);

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
    // No parameter for this step
};

#endif // PB_STEPCOMPUTEPOINTSMETRICS_H
