#ifndef PB_GROUPDATAEXPORTER_H
#define PB_GROUPDATAEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_GroupDataExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_GroupDataExporter();
    ~PB_GroupDataExporter();

    virtual QString getExporterCustomName() const;

    void init();

    virtual bool configureExport();

    bool setItemDrawableToExport(const QList<ItemDrawable*> &list);

    virtual IExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_GROUPDATAEXPORTER_H
