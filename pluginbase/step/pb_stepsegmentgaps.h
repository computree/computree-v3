#ifndef PB_STEPSEGMENTGAPS_H
#define PB_STEPSEGMENTGAPS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_tools/model/ct_autorenamemodels.h"

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_triangulation/ct_delaunayt.h"

#include "actions/pb_actiondefineheightlayer.h"
#include "actions/pb_actionsegmentgaps.h"

#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_itemdrawable/ct_polygon2d_old.h"


class CT_AbstractSingularItemDrawable;
class CT_AbstractItemGroup;

/*!
 * \class PB_StepSegmentGaps
 * \ingroup Steps_PB
 * \brief <b>Allow to manally select cells in a 3D grid</b>
 *
 * Manual action is used to obtain a boolean grid defining selected cells in original grid.
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

class PB_StepSegmentGaps: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     *
     * Create a new instance of the step
     *
     * \param dataInit Step parameters object
     */
    PB_StepSegmentGaps(CT_StepInitializeData &dataInit);

    /*! \brief Step description
     *
     * Return a description of the step function
     */
    QString getStepDescription() const;

    /**
     * @brief Inherit this method if you want to return your own detailled description.
     *
     *        By default return a empty string
     */
    virtual QString getStepDetailledDescription() const;

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
    DocumentInterface                           *m_doc;
    int                                         m_status;
    CT_AbstractActionForGraphicsView            *_action;
    QList<CT_Scene*>                            m_itemDrawableToAdd;

    CT_ResultGroup                  *_outResult;

    PB_ActionDefineHeightLayer_gridContainer *_gridContainer;
    CT_Grid2DXY<int>                         *_clustersGrid;

    float                       _xmin;
    float                       _ymin;
    float                       _zmin;
    float                       _xmax;
    float                       _ymax;
    float                       _zmax;



    int dropEmptyClusters();

    void registerClusterCells(QMap<int, QList<QVector2D*> *> &cellsMapByCluster,
                              QMap<int, size_t> &clusterCounts);

    void computeMetrics(QMap<int, size_t> &clusterCounts, CT_StandardItemGroup *baseGroup);

};

#endif // PB_STEPSEGMENTGAPS_H
