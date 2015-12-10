#ifndef PB_CSVEXPORTER_H
#define PB_CSVEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_CSVExporterConfiguration;
class CT_OutAbstractSingularItemModel;
class CT_OutAbstractItemAttributeModel;

class PB_CSVExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_CSVExporter();
    ~PB_CSVExporter();

    virtual QString getExporterCustomName() const;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    SettingsNodeGroup* saveExportConfiguration() const;

    bool loadExportConfiguration(const SettingsNodeGroup *root);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();

    bool configureExportWithLastConfigurationAndNewItemToExport();

private:

    PB_CSVExporterConfiguration                                                 *_configuration;
    QMap<CT_OutAbstractSingularItemModel*, QList<CT_AbstractItemDrawable*>* >   _mapItemToExport;
    bool                                                                        _mapKeyChanged;

    void clearMap();
    CT_OutAbstractSingularItemModel* getItemModelByName(const QString &name) const;
    CT_OutAbstractItemAttributeModel* getItemAttributeModelByName(CT_OutAbstractSingularItemModel *sItem, const QString &name) const;
};

#endif // PB_CSVEXPORTER_H
