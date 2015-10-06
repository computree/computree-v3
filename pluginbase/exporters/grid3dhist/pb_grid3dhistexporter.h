#ifndef PB_GRID3DHISTEXPORTER_H
#define PB_GRID3DHISTEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid3DHistExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_Grid3DHistExporter();
    ~PB_Grid3DHistExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_GRID3DHISTEXPORTER_H
