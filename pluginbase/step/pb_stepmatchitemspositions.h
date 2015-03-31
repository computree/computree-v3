#ifndef PB_STEPMATCHITEMSPOSITIONS_H
#define PB_STEPMATCHITEMSPOSITIONS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include <Eigen/Eigen>
#include <math.h>

/*!
 * \class PB_StepMatchItemsPositions
 * \ingroup Steps_PB
 * \brief <b>Co-registration entre deux ensembles de positions 2D.</b>
 *
 * No detailled description for this step
 *
 * \param _distThreshold 
 * \param _relativeSizeThreshold 
 * \param _minRelativeSize 
 *
 */

class PB_StepMatchItemsPositions: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepMatchItemsPositions(CT_StepInitializeData &dataInit);

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
     *
     * Adapted from :
     * Marius Hauglin, Vegard Lien, Erik Næsset & Terje Gobakken (2014)
     * Geo-referencing forest field plots by co-registration of terrestrial and airborne laser scanning data,
     * International Journal of Remote Sensing, 35:9, 3135-3149, DOI: 10.1080/01431161.2014.903440
     */
    void compute();

private:

    // Step parameters
    double      _distThreshold;
    double      _relativeSizeThreshold;
    double      _minRelativeSize;
    double      _maxTheta;
    bool        _possiblyInvertedDirection;
    double      _coef_nbRwc;
    double      _coef_nbTwc;
    double      _coef_nbSim;
    int         _drawMode;
    int         _relativeMode;
    double      _minval;
    double      _maxval;
    bool        _exportReport;
    QStringList _reportFileName;
    bool        _exportData;
    QStringList _transformedDataFileName;
    QStringList _transformationDataFileName;


    template <typename T> int sgn(T val)
    {
        return (val > T(0)) - (val < T(0));
    }

    Eigen::Matrix3d Kabsch(Eigen::MatrixXd &refPositions, Eigen::MatrixXd &transPositions, int pointsNumber, Eigen::Vector3d &center, Eigen::Vector3d &translation);

    Eigen::Matrix3d computeTransfMatrix2D(const Eigen::Vector2d &center, const Eigen::Vector2d &tranlation, const Eigen::Matrix2d &rotMat);

};

#endif // PB_STEPMATCHITEMSPOSITIONS_H
