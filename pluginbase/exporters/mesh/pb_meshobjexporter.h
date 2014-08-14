#ifndef PB_MESHOBJEXPORTER_H
#define PB_MESHOBJEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_MeshObjExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_MeshObjExporter();

    virtual QString getExporterCustomName() const;

    void init();
    bool configureExport() { return true; }

    bool setItemDrawableToExport(const QList<ItemDrawable*> &list);

    virtual IExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_MESHOBJEXPORTER_H
