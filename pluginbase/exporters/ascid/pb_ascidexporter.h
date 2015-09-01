#ifndef PB_ASCIDEXPORTER_H
#define PB_ASCIDEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporterpointattributesselection.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h"
#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

class PB_ASCIDExporter : public CT_AbstractExporterPointAttributesSelection
{
    Q_OBJECT
public:
    PB_ASCIDExporter();
    ~PB_ASCIDExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
};

#endif // PB_ASCIDEXPORTER_H
