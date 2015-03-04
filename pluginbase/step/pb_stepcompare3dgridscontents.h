#ifndef PB_STEPCOMPARE3DGRIDSCONTENTS_H
#define PB_STEPCOMPARE3DGRIDSCONTENTS_H

#include "ct_step/abstract/ct_abstractstep.h"

/*!
 * \class PB_StepCompare3DGridsContents
 * \ingroup Steps_PB
 * \brief <b>Compare le contenu de deux grilles 3D.</b>
 *
 * Il est préférable que les grilles aient la même résolution et le même calage spatial.
En sortie l'étape renvoie une grille contenant :
* 0 si les deux grilles sont inférieures au seuil (vide durable)
* 1 si la première grille est supérieur au seuil mais pas la seconde (perte)
* 2 si les deux grilles sont supérieures au seuil (maintien)
* 3 si la seconde grille est supérieure au seuil mais pas la première (accroissement)
 *
 * \param _threshold 
 *
 */

class PB_StepCompare3DGridsContents: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepCompare3DGridsContents(CT_StepInitializeData &dataInit);

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
    double    _threshold;    /*!<  */

};

#endif // PB_STEPCOMPARE3DGRIDSCONTENTS_H
