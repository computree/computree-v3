#ifndef PB_STEPEXPORTATTRIBUTESINLOOP_H
#define PB_STEPEXPORTATTRIBUTESINLOOP_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"

#ifdef USE_GDAL
#include "exporters/gdal/pb_gdalexporter.h"
#include "gdal_priv.h"
#include "ogr_core.h"
#include "ogrsf_frmts.h"

#include "ct_tools/ct_gdaltools.h"
#endif


class PB_StepExportAttributesInLoop: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepExportAttributesInLoop(CT_StepInitializeData &dataInit);

    ~PB_StepExportAttributesInLoop();

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
    void createPreConfigurationDialog();
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

#ifdef USE_GDAL
    QMap<QString, GDALDriver*> _gdalRasterDrivers;
    QMap<QString, GDALDriver*> _gdalVectorDrivers;
    QMap<QString, OGRFieldType>  _ogrTypes;
#endif

    QList<QString>          _modelsKeys;
    QMap<QString, QString>  _names;

    // Step parameters
    bool        _asciiExport;
    bool        _vectorExport;
    bool        _rasterExport;

    QString      _rasterDriverName;
    QString      _vectorDriverName;
    QString     _vectorPrefix;
    QString     _rasterPrefix;

    QStringList _outASCIIFileName;
    QStringList _outVectorFolder;
    QStringList _outRasterFolder;



};

#endif // PB_STEPEXPORTATTRIBUTESINLOOP_H
