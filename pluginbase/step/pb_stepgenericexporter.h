#ifndef PB_STEPGENERICEXPORTER_H
#define PB_STEPGENERICEXPORTER_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"

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
                           const QString &pluginName,
                           CT_AbstractExporter *exporter);
    ~PB_StepGenericExporter();

    const CT_AbstractExporter* exporter() const;
    const QString& pluginName() const;

    // CT_AbstractStep non obligatoire :
    void init();

    // surcharger pour ajouter PB_StepGenericExporter__
    QString getStepName() const;

    // surcharger pour afficher le nom de l'exporter
    QString getStepDisplayableName() const;

    QString getStepDetailledDescription() const;

    // CT_AbstractStep non obligatoire :
    SettingsNodeGroup* getAllSettings() const;

    // CT_AbstractStep non obligatoire :
    bool setAllSettings(const SettingsNodeGroup *settings);

    // CT_AbstractStep non obligatoire :
    QString getStepDescription() const;

    // CT_AbstractStep obligatoire :
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    void createPreConfigurationDialog();

    // CT_AbstractStep obligatoire :
    void createInResultModelListProtected();

    // CT_AbstractStep non obligatoire :
    bool configureInputResult(bool forceReadOnly = false);

    // CT_AbstractStep non obligatoire :
    bool postConfigure();

    // CT_AbstractStep obligatoire :
    void createOutResultModelListProtected();

    // CT_AbstractStep obligatoire :
    void compute();

private:
    CT_AbstractExporter *_exporter;
    QString             _pluginName;
    QString             _exportPath;
    QString             _exportFilename;

    bool                _adaptative;

    SettingsNodeGroup   *_exporterConfiguration;

    bool configureExporter();

    void clearExporterConfiguration();
    void configureExporterFromModel();
    void setDefaultExportPath(const QString &path);

    QString replaceBadCharacters(const QString &name) const;
private slots:
    void exportProgressChanged(int progress);
};

#endif // PB_STEPGENERICEXPORTER_H
