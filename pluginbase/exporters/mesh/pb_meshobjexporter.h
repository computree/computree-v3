#ifndef PB_MESHOBJEXPORTER_H
#define PB_MESHOBJEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_MeshObjExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_MeshObjExporter();

    virtual QString getExporterCustomName() const;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();
    bool configureExport() { return true; }

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_MESHOBJEXPORTER_H
