#ifndef PB_STEPGENERICEXPORTER_H
#define PB_STEPGENERICEXPORTER_H

#include "ct_step/abstract/ct_abstractstep.h"

class IExporter;

/*!
 * \class PB_StepGenericExporter
 * \ingroup Steps_PB
 * \brief <b>Generic exporter</b>
 *
 * This step can export any ItemDrawable by the way of
 * loaded exporter plugins.
 *
 */
class PB_StepGenericExporter : public CT_AbstractStep
{
    Q_OBJECT

public:

    PB_StepGenericExporter(CT_StepInitializeData &dataInit,
                           QString pluginName,
                           IExporter *exporter);
    ~PB_StepGenericExporter();

    const IExporter* exporter() const;
    const QString& pluginName() const;

    // CT_AbstractStep non obligatoire :
    void init();

    // surcharger pour avoir le nom du plugin
    QString getStepName() const;

    // CT_AbstractStep non obligatoire :
    SettingsNodeGroup* getAllSettings() const;

    // CT_AbstractStep non obligatoire :
    bool setAllSettings(const SettingsNodeGroup *settings);

    // CT_AbstractStep non obligatoire :
    QString getStepDescription() const;

    // CT_AbstractStep obligatoire :
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    // CT_AbstractStep obligatoire :
    void createInResultModelListProtected();

    // CT_AbstractStep non obligatoire :
    bool configureInputResult();

    // CT_AbstractStep non obligatoire :
    bool postConfigure();

    // CT_AbstractStep obligatoire :
    void createOutResultModelListProtected();

    // CT_AbstractStep obligatoire :
    void compute();

private:
    IExporter           *_exporter;
    QString             _pluginName;
    QString             _exportPath;
    QString             _exportFilename;

    SettingsNodeGroup   *_exporterConfiguration;

    bool configureExporter();

    void clearExporterConfiguration();
    void configureExporterFromModel();
    void setDefaultExportPath(const QString &path);

private slots:
    void exportProgressChanged(int progress);
};

#endif // PB_STEPGENERICEXPORTER_H
