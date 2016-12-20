#include "pb_opfexporter.h"

#include "ct_itemdrawable/ct_ttreegroup.h"
#include "ct_itemdrawable/ct_tnodegroup.h"
#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"

#include "ct_itemdrawable/ct_itemattributelist.h"
#include "ct_itemdrawable/ct_opfmeshmodel.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"

#include "ct_point.h"
#include "ct_mesh/ct_face.h"

#include "ct_iterator/ct_pointiterator.h"
#include "ct_iterator/ct_faceiterator.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <limits>

PB_OPFExporter::PB_OPFExporter() : CT_AbstractExporter()
{
    setExportOnlyGroup(true);
}

PB_OPFExporter::~PB_OPFExporter()
{
}

QString PB_OPFExporter::getExporterCustomName() const
{
    return tr("Topologie, format OPF");
}

CT_StepsMenu::LevelPredefined PB_OPFExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Others;
}


void PB_OPFExporter::init()
{
    addNewExportFormat(FileFormat("opf", tr("Fichiers AmapStudio .opf")));

    setToolTip(tr("Exporte une topologie décrivant l'architecture de végétaux dans un fichier au format OPF, pour utilisation par le logiciel AMAPStudio.<br>"
                  "http://amapstudio.cirad.fr"));
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

        QString prefix = "";

        txtStream << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl;
        txtStream << prefix << "<opf version=\"2.0\" editable=\"true\">" << endl;

        CT_TTreeGroup *topology = dynamic_cast<CT_TTreeGroup*>(itemDrawableToExport().first());

        CT_TNodeGroup *root = topology->rootNode();

        bool ok = true;

        if(root != NULL)
        {
            m_meshes.clear();

            recursiveSearchMesh(root);

            writeMeshAndShapeBdd(txtStream, prefix + "\t");
            writeAttributesBdd(txtStream, topology, prefix + "\t");

            ok = recursiveWriteFile(txtStream, "topology", root, prefix + "\t");
        }

        txtStream << prefix << "</opf>" << endl;

        file.close();

        return ok;
    }

    return false;
}

bool PB_OPFExporter::writeAttributesBdd(QTextStream &stream, CT_TTreeGroup *topology, const QString &prefix)
{
    stream << prefix << "<attributeBDD>" << endl;

    CT_OutAbstractModel *model = topology->model();

    QHash<QString, QString> list;
    recursiveSearchAttributes(model, list);

    QHashIterator<QString, QString> it(list);

    while(it.hasNext()) {
        it.next();
        stream << prefix << "\t<attribute name=\"" << it.key() << "\" class=\"" << it.value() << "\"/>" << endl;
    }

    stream << prefix << "</attributeBDD>" << endl;

    return true;
}

void PB_OPFExporter::recursiveSearchAttributes(CT_OutAbstractModel *model, QHash<QString, QString> &out)
{
    QList<CT_AbstractModel*> child = model->childrens();
    QListIterator<CT_AbstractModel*> it(child);

    while(it.hasNext())
    {
        CT_AbstractModel *c = it.next();
        CT_OutAbstractItemAttributeModel *attModel = dynamic_cast<CT_OutAbstractItemAttributeModel*>(c);

        if(attModel != NULL)
        {
            if(!out.contains(attModel->displayableName()))
            {
                QString t = "";

                CT_AbstractCategory::ValueType type = attModel->itemAttribute()->type();

                if(type == CT_AbstractCategory::STRING)
                    t = "String";
                else
                    t = "Double";

                out.insert(attModel->displayableName(), t);
            }
        }
        else
            recursiveSearchAttributes((CT_OutAbstractModel*)c, out);
    }
}

bool PB_OPFExporter::writeMeshAndShapeBdd(QTextStream &stream, const QString &prefix)
{
    if(!m_meshes.isEmpty())
    {
        stream << prefix << "<meshBDD>" << endl;

        int i = 0;

        QListIterator<CT_Mesh*> it(m_meshes);

        while(it.hasNext())
            writeMesh(stream, it.next(), i++, prefix + "\t");

        stream << prefix << "</meshBDD>" << endl;

        stream << prefix << "<shapeBDD>" << endl;

        i = 0;

        it.toFront();

        while(it.hasNext())
            writeShape(stream, it.next(), i++, prefix + "\t");

        stream << prefix << "</shapeBDD>" << endl;
    }

    return true;
}

void PB_OPFExporter::recursiveSearchMesh(CT_TNodeGroup *node)
{
    CT_ItemIterator itI(node);

    const CT_MeshModel *mesh = NULL;

    while(itI.hasNext()
            && (mesh == NULL))
        mesh = dynamic_cast<const CT_MeshModel*>(itI.next());

    if(mesh != NULL)
    {
        if(!m_meshes.contains(mesh->mesh()))
            m_meshes.append(mesh->mesh());
    }

    CT_TNodeGroup *component = node->rootComponent();

    if(component != NULL)
        recursiveSearchMesh((CT_TNodeGroup*)component);

    QListIterator<CT_TNodeGroup*> it(node->branches());

    while(it.hasNext())
        recursiveSearchMesh((CT_TNodeGroup*)it.next());

    if(component != NULL)
    {
        while(component->successor() != NULL)
        {
            component = component->successor();
            recursiveSearchMesh((CT_TNodeGroup*)component);
        }
    }
}

bool PB_OPFExporter::writeMesh(QTextStream &stream, CT_Mesh *mesh, const int &index, const QString &prefix)
{
    CT_AbstractPointCloudIndex *pci = mesh->abstractVert();
    CT_AbstractFaceCloudIndex *fci = mesh->abstractFace();

    if((pci != NULL)
            && (fci != NULL))
    {
        stream << prefix << "<mesh Id=\"" << index << "\" enableScale=\"true\">" << endl;

        CT_PointIterator itP(pci);

        size_t pBeginIndex = std::numeric_limits<size_t>::max();

        stream << prefix << "\t<points>" << endl << prefix << "\t\t";

        while(itP.hasNext())
        {
            const CT_Point &p = itP.next().currentPoint();

            stream << p(0) << " " << p(1) << " " << p(2) << " ";

            if(itP.cIndex() < pBeginIndex)
                pBeginIndex = itP.cIndex();
        }

        stream << endl << prefix << "\t</points>" << endl;

        CT_FaceIterator itF(fci);

        stream << prefix << "\t<faces>" << endl;

        int i = 0;

        while(itF.hasNext())
        {
            const CT_Face &f = itF.next().cT();

            stream << prefix << "\t\t<face Id=\"" << i << "\">" << endl;

            stream << prefix << "\t\t\t" << f.iPointAt(0)-pBeginIndex << " " << f.iPointAt(1)-pBeginIndex << " " << f.iPointAt(2)-pBeginIndex << endl;

            stream << prefix << "\t\t</face>" << endl;

            ++i;
        }

        stream << prefix << "\t</faces>" << endl;

        stream << prefix << "</mesh>" << endl;
    }

    return true;
}

bool PB_OPFExporter::writeShape(QTextStream &stream, CT_Mesh *mesh, const int &index, const QString &prefix)
{
    CT_AbstractPointCloudIndex *pci = mesh->abstractVert();
    CT_AbstractFaceCloudIndex *fci = mesh->abstractFace();

    if((pci != NULL)
            && (fci != NULL))
    {
        stream << prefix << "<shape Id=\"" << index << "\">" << endl;
        stream << prefix << "\t<name>Mesh" << index << "</name>" << endl;
        stream << prefix << "\t<meshIndex>" << index << "</meshIndex>" << endl;
        stream << prefix << "</shape>" << endl;
    }

    return true;
}

bool PB_OPFExporter::recursiveWriteFile(QTextStream &stream, const QString &type, CT_TNodeGroup *node, const QString &prefix)
{
    bool ok = false;

    CT_TOPFNodeGroup *opfNode = dynamic_cast<CT_TOPFNodeGroup*>(node);

    quint8 scale = 0;

    if (opfNode != NULL)
    {
        scale = opfNode->opfModel()->opfLevel();
    }

    stream << prefix << "<" << type << " class=\"" << node->model()->displayableName() << "\" scale=\"" << scale << "\" id=\"" << node->id() << "\">" << endl;

    CT_MeshModel *mesh = NULL;

    CT_ItemIterator itI(node);

    while(itI.hasNext())
    {
        CT_AbstractSingularItemDrawable *item = (CT_AbstractSingularItemDrawable*)itI.next();
//        CT_ItemAttributeList *attributes = dynamic_cast<CT_ItemAttributeList*>(item);

//        if(attributes != NULL)
//        {
//            QList<CT_AbstractItemAttribute*> l = attributes->notDefaultItemAttributes();
//            QListIterator<CT_AbstractItemAttribute*> itA(l);

//            while(itA.hasNext())
//                writeAttribute(stream, node, attributes, itA.next(), prefix + "\t");
//        }
//        else
//        {
//            CT_MeshModel *oMesh = dynamic_cast<CT_MeshModel*>(item);

//            if(oMesh != NULL)
//                mesh = oMesh;
//        }



        CT_MeshModel *oMesh = dynamic_cast<CT_MeshModel*>(item);

        if(oMesh != NULL)
        {
            CT_MeshModel *oMesh = dynamic_cast<CT_MeshModel*>(item);

            if(oMesh != NULL)
                mesh = oMesh;
        }
        else
        {

            QList<CT_AbstractItemAttribute*> l = item->defaultItemAttributes();
            QListIterator<CT_AbstractItemAttribute*> itA(l);

            while(itA.hasNext())
                writeAttribute(stream, node, item, itA.next(), prefix + "\t");


            QList<CT_AbstractItemAttribute*> l2 = item->notDefaultItemAttributes();
            QListIterator<CT_AbstractItemAttribute*> itA2(l2);

            while(itA2.hasNext())
                writeAttribute(stream, node, item, itA2.next(), prefix + "\t");
        }

    }

    ok = writeGeometry(stream, node, mesh, prefix + "\t");

    CT_TNodeGroup *component = node->rootComponent();

    if(component != NULL)
        ok = recursiveWriteFile(stream, "decomp", (CT_TNodeGroup*)component, prefix + "\t");

    QListIterator<CT_TNodeGroup*> it(node->branches());

    while(it.hasNext())
        ok = recursiveWriteFile(stream, "branch", (CT_TNodeGroup*)it.next(), prefix + "\t");

    if(component != NULL)
    {
        while(component->successor() != NULL)
        {
            component = component->successor();
            ok = recursiveWriteFile(stream, "follow", (CT_TNodeGroup*)component, prefix + "\t");
        }
    }

    stream << prefix << "</" + type + ">" << endl;

    return ok;
}

bool PB_OPFExporter::writeAttribute(QTextStream &stream, CT_TNodeGroup *node, CT_AbstractSingularItemDrawable *l, CT_AbstractItemAttribute *att, const QString &prefix)
{
    Q_UNUSED(node)

    stream << prefix << "<" + att->model()->displayableName() << ">" << att->toString(l, NULL) << "</" << att->model()->displayableName() << ">" << endl;

    return true;
}

bool PB_OPFExporter::writeGeometry(QTextStream &stream, CT_TNodeGroup *node, CT_MeshModel *mesh, const QString &prefix)
{
    if(mesh != NULL)
    {
        stream << prefix << "<geometry class=\"Mesh\">" << endl;
        stream << prefix << "\t<shapeIndex>" << m_meshes.indexOf(mesh->mesh()) << "</shapeIndex>" << endl;

        stream << prefix << "\t<mat>" << endl;

        CT_TOPFNodeGroup *opfNode = dynamic_cast<CT_TOPFNodeGroup*>(node);

        QMatrix4x4 matrix;
        if (opfNode != NULL)
        {
            matrix = opfNode->opfMatrix();
        }

        stream << prefix << "\t\t" << matrix(0,0)*100.0 << "\t" << matrix(0,1)*100.0 << "\t" << matrix(0,2)*100.0 << "\t" << matrix(0,3)*100.0 << endl;
        stream << prefix << "\t\t" << matrix(1,0)*100.0 << "\t" << matrix(1,1)*100.0 << "\t" << matrix(1,2)*100.0 << "\t" << matrix(1,3)*100.0 << endl;
        stream << prefix << "\t\t" << matrix(2,0)*100.0 << "\t" << matrix(2,1)*100.0 << "\t" << matrix(2,2)*100.0 << "\t" << matrix(2,3)*100.0 << endl;

        stream << prefix << "\t</mat>" << endl;

        CT_OPFMeshModel *opfMeshModel = dynamic_cast<CT_OPFMeshModel*>(mesh);

        if(opfMeshModel != NULL)
        {
            stream << prefix << "\t<dUp>" << opfMeshModel->dUp() << "</dUp>" << endl;
            stream << prefix << "\t<dDwn>" << opfMeshModel->dDown() << "</dDwn>" << endl;
        }
        else
        {
            stream << prefix << "\t<dUp>1.0</dUp>" << endl;
            stream << prefix << "\t<dDwn>1.0</dDwn>" << endl;
        }

        stream << prefix << "</geometry>" << endl;
    }

    return true;
}

