#ifndef PB_STEPUSERITEMSELECTION_H
#define PB_STEPUSERITEMSELECTION_H

#include "ct_step/abstract/ct_abstractstep.h"

class CT_AbstractSingularItemDrawable;
class CT_AbstractItemGroup;

/*!
 * \class PB_StepUserItemSelection
 * \ingroup Steps_PB
 * \brief <b>Etape qui permet de sélectionner manuellement des ItemDrawable et génère un résultat de sortie avec les items sélectionnés.</b>
 *
 * Detailed decription of step purpose.
 * Please also give a general view of the algorithm.
 *
 *
 *
 * <b>Input Models:</b>
 *
 * - CT_ResultGroup (Result)\n
 *     - CT_StandardItemGroup...\n
 *         - CT_AbstractItemDrawableWithPointCloud (Item)\n
 *
 * <b>Output Models:</b>
 *
 * - CT_ResultGroup (Result)\n
 *     - <em>cpy CT_StandardItemGroup...</em>\n
 *         - <em>cpy CT_Scene (Item)</em>\n
 *
 */

class PB_StepUserItemSelection: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepUserItemSelection(CT_StepInitializeData &dataInit);

    /*! \brief Step description
     * 
     * Return a description of the step function
     */
    QString getStepDescription() const;

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

    void initManualMode();

    void useManualMode(bool quit = false);

private:

    // Step parameters
    QHash<CT_AbstractItemDrawable*, CT_AbstractItemGroup*>      m_itemDrawableToAdd;
    QList<CT_AbstractItemDrawable*>                             m_itemDrawableSelected;
    DocumentInterface                                           *m_doc;
    int                                                         m_status;
    bool                                                        m_removeGroupsWithoutItemResearched;
    bool                                                        m_removeParents;

    int                                                         _mode;

    void recursiveRemoveGroup(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const;
};

#endif // PB_STEPUSERITEMSELECTION_H
