#ifndef PB_OPFEXPORTER_H
#define PB_OPFEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class QTextStream;
class CT_TTreeGroup;
class CT_TOPFNodeGroup;
class CT_AbstractSingularItemDrawable;
class CT_MeshModel;
class CT_ItemAttributeList;
class CT_AbstractItemAttribute;
class CT_Mesh;

class PB_OPFExporter : public CT_AbstractExporter
{
    Q_OBJECT
public:
    PB_OPFExporter();
    ~PB_OPFExporter();

    virtual QString getExporterCustomName() const;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);
    bool configureExport() { return true; }

    SettingsNodeGroup* saveExportConfiguration() const;
    bool loadExportConfiguration(const SettingsNodeGroup *root);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();

private:
    QList<CT_Mesh*>        m_meshes;

    bool writeAttributesBdd(QTextStream &stream, CT_TTreeGroup *topology, const QString &prefix);
    void recursiveSearchAttributes(CT_OutAbstractModel *model, QHash<QString, QString> &out);
    bool writeMeshAndShapeBdd(QTextStream &stream, const QString &prefix);
    void recursiveSearchMesh(CT_TOPFNodeGroup *node);
    bool writeMesh(QTextStream &stream, CT_Mesh *mesh, const int &index, const QString &prefix);
    bool writeShape(QTextStream &stream, CT_Mesh *mesh, const int &index, const QString &prefix);
    bool recursiveWriteFile(QTextStream &stream, const QString &type, CT_TOPFNodeGroup *node, const QString &prefix);
    bool writeAttribute(QTextStream &stream, CT_TOPFNodeGroup *node, CT_ItemAttributeList *l, CT_AbstractItemAttribute *att, const QString &prefix);
    bool writeGeometry(QTextStream &stream, CT_TOPFNodeGroup *node, CT_MeshModel *mesh, const QString &prefix);
};

#endif // PB_OPFEXPORTER_H
