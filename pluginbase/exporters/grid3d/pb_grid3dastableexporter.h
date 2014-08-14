#ifndef PB_GRID3DASTABLEEXPORTER_H
#define PB_GRID3DASTABLEEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid3DAsTableExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_Grid3DAsTableExporter();
    ~PB_Grid3DAsTableExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<ItemDrawable*> &list);

    bool configureExport();

    virtual IExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_GRID3DASTABLEEXPORTER_H
