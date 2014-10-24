#ifndef PB_STEPSLICEPOINTCLOUD_H
#define PB_STEPSLICEPOINTCLOUD_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_itemdrawable/ct_scene.h"
#include "actions/pb_actionslicepointcloud.h"

/*!
 * \class PB_StepSlicePointCloud
 * \ingroup Steps_PB
 * \brief <b>Découpe une scène en tranche.</b>
 *
 * Action manuelle permettant de découper une scène d'entrée en tranches horizontales.
Il est possible d'en régler intéractivement :
- Le Z minimum (<b>_zMin</b>)
- Le Z maximum (<b>_zMax</b>)
- L'épaisseur (<b>_thickness</b>)
- L'espacement entre deux tranches (<b>_spacing</b>)

N.B. : Cette étape peut également fonctionner en mode non interactif, avec les paramètres choisis dans la boite de configuration. 
 *
 *
 */

class PB_StepSlicePointCloud: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepSlicePointCloud(CT_StepInitializeData &dataInit);

    ~PB_StepSlicePointCloud();

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
    double       _xmin;
    double       _ymin;
    double       _zmin;
    double       _xmax;
    double       _ymax;
    double       _zmax;
    bool         _manual;

    PB_ActionSlicePointCloud_dataContainer*     _dataContainer;

    DocumentInterface*      _m_doc;
    int                     _m_status;
    QList<CT_Scene*>* _sceneList;

};

#endif // PB_STEPSLICEPOINTCLOUD_H
