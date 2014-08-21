#ifndef PB_GRID3DEXPORTER_H
#define PB_GRID3DEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid3DExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_Grid3DExporter();
    ~PB_Grid3DExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_GRID3DEXPORTER_H
