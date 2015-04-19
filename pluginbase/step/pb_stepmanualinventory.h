#ifndef PB_STEPMANUALINVENTORY_H
#define PB_STEPMANUALINVENTORY_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_circle.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_tools/model/ct_autorenamemodels.h"

class CT_AbstractSingularItemDrawable;
class CT_AbstractItemGroup;

/*!
 * \class PB_StepManualInventory
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

class PB_StepManualInventory: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepManualInventory(CT_StepInitializeData &dataInit);

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
    CT_AutoRenameModels    _dbhcircle_ModelName;
    CT_AutoRenameModels    _attributes_ModelName;
    CT_AutoRenameModels    _attribute_dbh_ModelName;
    CT_AutoRenameModels    _attribute_x_ModelName;
    CT_AutoRenameModels    _attribute_y_ModelName;
    CT_AutoRenameModels    _attribute_z_ModelName;
    CT_AutoRenameModels    _attribute_h130_ModelName;
    CT_AutoRenameModels    _attribute_h_ModelName;

    // Step parameters
    QStringList                         _paramFileName;
    double                              _deltaH;
    double                              _maxCircleDist;
    QMap<QString, QStringList>          _paramData;
    QMap<QString, CT_AutoRenameModels>  _paramAutoRename;


    // Step parameters
    DocumentInterface                               *m_doc;
    int                                             m_status;

    const CT_Grid2DXY<double>                                       *_itemIn_mnt;
    QMap<const CT_Scene*, const CT_Circle*>                         *_selectedDbh;
    QMap<const CT_Scene*, const CT_Point2D*>                        _positions;
    QMap<const CT_Scene*, QMultiMap<double, const CT_Circle*> >     *_availableDbh;
    QMap<const CT_Scene*, QMap<QString, QString> >                  *_suppAttributes;
    QList<const CT_Circle*>                                         *_preferredDbh;
    QList<const CT_Scene *>                                         *_trashedScenes;
    QMap<const CT_Scene*, double>                                   *_sceneDTMValues;


    QList<CT_Circle*>                                               _temporaryCircles;

    void findBestCircleForEachScene();
    double computeMaxZ(const CT_Scene* scene);

};

#endif // PB_STEPMANUALINVENTORY_H
