#ifndef PB_STEPDETECTVERTICALALIGNMENTS_H
#define PB_STEPDETECTVERTICALALIGNMENTS_H

#include "ct_step/abstract/ct_abstractstep.h"

// Inclusion of auto-indexation system
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_point.h"

#include "eigen/Eigen/Core"


class PB_StepDetectVerticalAlignments: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepDetectVerticalAlignments(CT_StepInitializeData &dataInit);

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

    struct LineData {
        LineData(const CT_Point &pLow, const CT_Point &pHigh, size_t index1, size_t index2, float phi, double bottomLevel, double topLevel)
        {
            _index1 = index1;
            _index2 = index2;
            _pLow = pLow;
            _pHigh = pHigh;
            _phi = phi;

            Eigen::Vector3d dir = pHigh - pLow;
            dir.normalize();

            double t = (bottomLevel - pLow(2)) / dir(2);

            _lowCoord(0) = pLow(0) + t*dir(0);
            _lowCoord(1) = pLow(1) + t*dir(1);
            _lowCoord(2) = bottomLevel;

            t = (topLevel - pLow(2)) / dir(2);

            _highCoord(0) = pLow(0) + t*dir(0);
            _highCoord(1) = pLow(1) + t*dir(1);
            _highCoord(2) = topLevel;
        }

        float _phi;
        size_t _index1;
        size_t _index2;
        Eigen::Vector3d _pLow;
        Eigen::Vector3d _pHigh;
        Eigen::Vector3d _lowCoord;
        Eigen::Vector3d _highCoord;


    };

    struct DistValues {
        double _min;
        double _max;
        double _mean;
        double _q25;
        double _q50;
        double _q75;
    };

    static bool lessThan(LineData *s1, LineData *s2)
    {
        return s1->_phi < s2->_phi;
    }


    // Declaration of autoRenames Variables (groups or items added to In models copies)
    CT_AutoRenameModels    _grpCluster_ModelName;
    CT_AutoRenameModels    _cluster_ModelName;
    CT_AutoRenameModels    _line_ModelName;
    CT_AutoRenameModels    _attMin_ModelName;
    CT_AutoRenameModels    _attQ25_ModelName;
    CT_AutoRenameModels    _attQ50_ModelName;
    CT_AutoRenameModels    _attQ75_ModelName;
    CT_AutoRenameModels    _attMax_ModelName;
    CT_AutoRenameModels    _attMean_ModelName;

    CT_AutoRenameModels    _grpDroppedCluster_ModelName;
    CT_AutoRenameModels    _droppedCluster_ModelName;
    CT_AutoRenameModels    _droppedLine_ModelName;


    // Step parameters
    double    _maxAngle;
    double    _distThreshold;
    double    _lineDistThreshold;
    int       _minPtsNb;
    double    _lengthThreshold;
    double    _heightThreshold;
    double    _ratioDist;

};

#endif // PB_STEPDETECTVERTICALALIGNMENTS_H
