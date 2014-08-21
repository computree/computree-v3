#ifndef PB_OPFEXPORTER_H
#define PB_OPFEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class QTextStream;
class CT_TOPFNodeGroup;
class CT_AbstractSingularItemDrawable;
class CT_MeshModel;
class CT_AbstractMetric;

class PB_OPFExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_OPFExporter();
    ~PB_OPFExporter();

    virtual QString getExporterCustomName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);
    bool configureExport() { return true; }

    SettingsNodeGroup* saveExportConfiguration() const;
    bool loadExportConfiguration(const SettingsNodeGroup *root);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();

private:
    bool recursiveWriteFile(QTextStream &stream, const QString &type, CT_TOPFNodeGroup *node);
    bool writeAttribute(QTextStream &stream, CT_TOPFNodeGroup *node, CT_AbstractMetric *att);
    bool writeGeometry(QTextStream &stream, CT_TOPFNodeGroup *node, CT_MeshModel *mesh);
};

#endif // PB_OPFEXPORTER_H
