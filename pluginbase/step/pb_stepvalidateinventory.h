#ifndef PB_STEPVALIDATEINVENTORY_H
#define PB_STEPVALIDATEINVENTORY_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_itemdrawable/ct_referencepoint.h"

class CT_AbstractSingularItemDrawable;
class CT_AbstractItemGroup;

/*!
 * \class PB_StepValidateInventory
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

class PB_StepValidateInventory: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepValidateInventory(CT_StepInitializeData &dataInit);

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

    // Declaration of autoRenames Variables (groups or items added to In models copies)
    CT_AutoRenameModels    _attributes_ModelName;
    CT_AutoRenameModels    _attribute_sp_ModelName;
    CT_AutoRenameModels    _attribute_id_ModelName;
    CT_AutoRenameModels    _attribute_idItem_ModelName;

    // Step parameters
    QStringList    _speciesFileName;
    double          _hRef;


    // Step parameters
    DocumentInterface                               *m_doc;
    int                                             m_status;

    QMap<const CT_StandardItemGroup*, float>                                         _dtmZvalues;
    QMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>           *_selectedItem;
    QMultiMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>      *_availableItem;
    QMap<const CT_StandardItemGroup*, QString>                                          *_species;
    QMap<const CT_StandardItemGroup*, QString>                                          *_ids;
    QStringList                                                                         _speciesList;

    void findBestItemForEachGroup();

};

#endif // PB_STEPVALIDATEINVENTORY_H
