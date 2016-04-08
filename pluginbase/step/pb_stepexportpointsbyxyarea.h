#ifndef PB_STEPEXPORTPOINTSBYXYAREA_H
#define PB_STEPEXPORTPOINTSBYXYAREA_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
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

    struct AreaData
    {
        AreaData(CT_AreaShape2DData* area, CT_AbstractExporter* exporter)
        {
            _area = area;
            _exporter = exporter;
            _cloudIndex = NULL;
        }

        ~AreaData()
        {
            delete _area;
            delete _exporter;
        }

        void setPointCloudIndex(CT_PointCloudIndexVector* cloudIndex)
        {
            if (_cloudIndex != NULL)
            {
                delete _cloudIndex;
            }
            _cloudIndex = cloudIndex;
        }


        void deletePointCloudIndex()
        {
            delete _cloudIndex;
            _cloudIndex = NULL;
        }

        CT_AreaShape2DData* _area;
        CT_AbstractExporter* _exporter;
        CT_PointCloudIndexVector* _cloudIndex;
    };

    // Step parameters
    QStringList _dir;
    QString _suffixFileName;

    QMap<QString, CT_AbstractExporter*>             _exportersMap;
    QList<CT_AbstractExporter*>                      _exportersInstancesList;

    QString                                         _exportersListValue;

    QList<AreaData*>                _areas;

};

#endif // PB_STEPEXPORTPOINTSBYXYAREA_H
