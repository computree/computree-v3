#ifndef PB_STEPEXPORTPOINTSBYXYAREA_H
#define PB_STEPEXPORTPOINTSBYXYAREA_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_exporter/abstract/ct_abstractpointexporter.h"
#include "ct_itemdrawable/ct_box2d.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

class PB_StepExportPointsByXYArea: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepExportPointsByXYArea(CT_StepInitializeData &dataInit);

    ~PB_StepExportPointsByXYArea();

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
    QStringList _dir;
    QString _suffixFileName;

    QMap<QString, CT_AbstractExporter*>             _exportersMap;
    QList<CT_AbstractExporter*>                      _exportersInstancesList;

    QString                                         _exportersListValue;

    QList<CT_Box2DData*>                _areas;
    QList<CT_AbstractPointExporter*>    _areasExporters;
    QList<CT_PointCloudIndexVector*>    _areasClouds;

};

#endif // PB_STEPEXPORTPOINTSBYXYAREA_H
