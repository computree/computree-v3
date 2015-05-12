#ifndef PB_MULTIXYBEXPORTER_H
#define PB_MULTIXYBEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporterpointattributesselection.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h"
#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

class PB_MultiXYBExporter : public CT_AbstractExporterPointAttributesSelection
{
    Q_OBJECT
public:
    PB_MultiXYBExporter();
    ~PB_MultiXYBExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    SettingsNodeGroup* saveExportConfiguration() const;
    bool loadExportConfiguration(const SettingsNodeGroup *root);

    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > getBuilders() const;
    void setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const;
    bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
    void clearWorker();

private:
    void exportPoints(QDataStream &stream,
                      const CT_AbstractPointCloudIndex *constPCIndex,
                      const CT_AbstractColorCloud *cc);
};

#endif // PB_MULTIXYBEXPORTER_H
