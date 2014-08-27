#include "pb_opfexporter.h"

#include "ct_itemdrawable/ct_ttreegroup.h"
#include "ct_itemdrawable/ct_topfnodegroup.h"
#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractmetric.h"
#include "ct_itemdrawable/ct_meshmodel.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

PB_OPFExporter::PB_OPFExporter() : CT_AbstractExporter()
{
}

PB_OPFExporter::~PB_OPFExporter()
{
}

QString PB_OPFExporter::getExporterCustomName() const
{
    return "Topologie, format OPF";
}

void PB_OPFExporter::init()
{
    addNewExportFormat(FileFormat("opf", tr("Fichiers AmapStudio .opf")));
}

bool PB_OPFExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext()
            && myList.isEmpty())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_TTreeGroup*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_TTreeGroup"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

SettingsNodeGroup* PB_OPFExporter::saveExportConfiguration() const
{
    SettingsNodeGroup *root = CT_AbstractExporter::saveExportConfiguration();

    return root;
}

bool PB_OPFExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    return CT_AbstractExporter::loadExportConfiguration(root);
}

CT_AbstractExporter* PB_OPFExporter::copy() const
{
    return new PB_OPFExporter();
}

bool PB_OPFExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "opf";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl;
        txtStream << "<opf version=\"2.0\" editable=\"true\">" << endl;

        CT_TTreeGroup *topology = dynamic_cast<CT_TTreeGroup*>(itemDrawableToExport().first());

        CT_TOPFNodeGroup *root = dynamic_cast<CT_TOPFNodeGroup*>(topology->rootNode());

        bool ok = true;

        if(root != NULL)
            ok = recursiveWriteFile(txtStream, "topology", root);

        txtStream << "</opf>" << endl;

        file.close();

        return ok;
    }

    return false;
}

bool PB_OPFExporter::recursiveWriteFile(QTextStream &stream, const QString &type, CT_TOPFNodeGroup *node)
{
    bool ok = false;

    quint8 scale = node->opfModel()->opfLevel();

    stream << "<" << type << " class=\"" << node->model()->displayableName() << "\" scale=\"" << scale << "\" id=\"" << node->id() << "\">" << endl;

    CT_MeshModel *mesh = NULL;

    if(node->beginIterateItems())
    {
        CT_AbstractSingularItemDrawable *item;

        while((item = node->nextItem()) != NULL)
        {
            CT_AbstractMetric *metric = dynamic_cast<CT_AbstractMetric*>(item);

            if(metric != NULL)
                ok = writeAttribute(stream, node, metric);
            else
            {
                CT_MeshModel *oMesh = dynamic_cast<CT_MeshModel*>(item);

                if(oMesh != NULL)
                    mesh = oMesh;
            }
        }
    }

    ok = writeGeometry(stream, node, mesh);

    CT_TNodeGroup *component = node->rootComponent();

    if(component != NULL)
        ok = recursiveWriteFile(stream, "decomp", (CT_TOPFNodeGroup*)component);

    QListIterator<CT_TNodeGroup*> it(node->branches());

    while(it.hasNext())
        ok = recursiveWriteFile(stream, "branch", (CT_TOPFNodeGroup*)it.next());

    if(component != NULL)
    {
        while(component->successor() != NULL)
        {
            component = component->successor();
            ok = recursiveWriteFile(stream, "follow", (CT_TOPFNodeGroup*)component);
        }
    }

    stream << "</" + type + ">";

    return ok;
}

bool PB_OPFExporter::writeAttribute(QTextStream &stream, CT_TOPFNodeGroup *node, CT_AbstractMetric *att)
{
    Q_UNUSED(node)

    stream << "<" + att->model()->displayableName() << ">" << att->toString() << "</" << att->model()->displayableName() << ">" << endl;

    return true;
}

bool PB_OPFExporter::writeGeometry(QTextStream &stream, CT_TOPFNodeGroup *node, CT_MeshModel *mesh)
{
    stream << "<geometry class=\"Mesh\">" << endl;
    stream << "<mat>" << endl;

    QMatrix4x4 matrix = node->transformMatrix();

    stream << matrix(0,0) << "\t" << matrix(0,1) << "\t" << matrix(0,2) << "\t" << matrix(0,3) << endl;
    stream << matrix(1,0) << "\t" << matrix(1,1) << "\t" << matrix(1,2) << "\t" << matrix(1,3) << endl;
    stream << matrix(2,0) << "\t" << matrix(2,1) << "\t" << matrix(2,2) << "\t" << matrix(2,3) << endl;

    stream << "</mat>" << endl;

    // TODO
    stream << "<dUp>1.0</dUp>" << endl;
    // TODO
    stream << "<dDwn>1.0</dDwn>" << endl;

    stream << "</geometry>" << endl;

    return true;
}

