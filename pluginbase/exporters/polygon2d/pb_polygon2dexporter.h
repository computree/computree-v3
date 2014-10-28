#ifndef PB_POLYGON2DEXPORTER_H
#define PB_POLYGON2DEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Polygon2DExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_Polygon2DExporter();
    ~PB_Polygon2DExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_POLYGON2DEXPORTER_H
