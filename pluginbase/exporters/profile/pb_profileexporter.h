#ifndef PB_PROFILEEXPORTER_H
#define PB_PROFILEEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_ProfileExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_ProfileExporter();
    ~PB_ProfileExporter();

    virtual QString getExporterCustomName() const;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_PROFILEEXPORTER_H
