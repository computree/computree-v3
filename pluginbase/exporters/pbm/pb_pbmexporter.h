#ifndef PB_PNMEXPORTER_H
#define PB_PNMEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_PbmExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_PbmExporter();
    ~PB_PbmExporter();

    virtual QString getExporterCustomName() const;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    bool configureExport();

    virtual CT_AbstractExporter* copy() const;

protected:
    bool protectedExportToFile();
};


#endif // PB_PNMEXPORTER_H
