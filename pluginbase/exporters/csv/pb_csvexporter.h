#ifndef PB_CSVEXPORTER_H
#define PB_CSVEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_CSVExporterConfiguration;

class PB_CSVExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_CSVExporter();
    ~PB_CSVExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<ItemDrawable*> &list);

    bool configureExport();

    SettingsNodeGroup* saveExportConfiguration() const;

    bool loadExportConfiguration(const SettingsNodeGroup *root);

    virtual IExporter* copy() const;

protected:

    bool protectedExportToFile();

private:

    PB_CSVExporterConfiguration                             *_configuration;
    QMap<const IItemDataRefList*, QList<ItemDrawable*>* >   _mapItemToExport;
    bool                                                    _mapKeyChanged;

    void clearMap();
    const IItemDataRefList* getItemDataRefListByName(const QString &name) const;
    IItemDataRef* getItemDataRefByName(const IItemDataRefList *refList, const QString &name) const;
};

#endif // PB_CSVEXPORTER_H
