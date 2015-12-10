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

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();

    virtual bool configureExport();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_GROUPDATAEXPORTER_H
