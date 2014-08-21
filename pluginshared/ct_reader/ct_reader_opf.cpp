#include "ct_reader_opf.h"

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"
#include "ct_itemdrawable/ct_metrict.h"
#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_itemdrawable/ct_ttreegroup.h"
#include "ct_itemdrawable/ct_topfnodegroup.h"
#include "ct_mesh/tools/ct_meshallocatort.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"
#include "ct_math/ct_mathpoint.h"
#include "ct_color.h"

#include <QFile>
#include <QtXml>
#include <limits>

const QVector<QString> CT_Reader_OPF::TOPOLOGY_NAMES = QVector<QString>() << "topology" << "decomp" << "follow" << "branch";

CT_Reader_OPF::CT_Reader_OPF() : CT_AbstractReader()
{
}

CT_Reader_OPF::~CT_Reader_OPF()
{
    clearOtherModels();
    clearMeshes();
    clearShapes();
}

bool CT_Reader_OPF::setFilePath(const QString &filepath)
{
    bool valid = false;

    QHash<QString, CT_OPF_Attribute>    attributes;
    QHash<QString, CT_OPF_Type>         types;
    int                                 totalNode = 0;

    QXmlStreamReader xml;

    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            xml.setDevice(&f);

            while (!xml.atEnd())
            {
                if(xml.readNextStartElement())
                {
                    QString nn = xml.name().toString();

                    if(nn == "attributeBDD")
                    {
                        bool continueLoop = true;

                        while(!xml.atEnd()
                                && continueLoop)
                        {
                            xml.readNext();

                            if(xml.isStartElement())
                            {
                                QString name = xml.attributes().value("name").toString();

                                attributes.insert(name, CT_OPF_Attribute(name, xml.attributes().value("class").toString()));
                            }
                            else if(xml.isEndElement() && xml.name() == "attributeBDD")
                            {
                                continueLoop = false;
                            }
                        }
                    }
                    else if(TOPOLOGY_NAMES.contains(nn))
                    {
                        ++totalNode;

                        while(!xml.atEnd())
                        {
                            QString nameT = xml.attributes().value("class").toString();

                            CT_OPF_Type type = types.value(nameT, CT_OPF_Type(nameT,
                                                                              xml.attributes().value("scale").toString().toInt(),
                                                                              xml.attributes().value("id").toString().toInt()));

                            bool addAttributes = (type.m_attributes.size() != attributes.size());
                            bool continueLoop = true;

                            while(!xml.atEnd()
                                   && continueLoop)
                            {
                                if(xml.readNextStartElement())
                                {
                                    QString name = xml.name().toString();

                                    if(TOPOLOGY_NAMES.contains(name))
                                    {
                                        ++totalNode;
                                        continueLoop = false;
                                    }
                                    else if(addAttributes
                                            && attributes.contains(name))
                                    {
                                        if(!type.m_attributes.contains(name))
                                            type.m_attributes.insert(name, attributes.value(name));
                                    }
                                }
                            }

                            types.insert(nameT, type);
                        }
                    }
                }
            }

            f.close();
        }
        else
            PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to open file %1").arg(filepath));
    }

    if(!types.isEmpty())
    {
        m_types = types;
        m_attributes = attributes;
        m_totalNode = totalNode;
        valid = CT_AbstractReader::setFilePath(filepath);
    }
    else
        PS_LOG->addErrorMessage(LogInterface::reader, tr("No types found in %1").arg(filepath));

    return valid;
}

CT_AbstractReader* CT_Reader_OPF::copy() const
{
    return new CT_Reader_OPF();
}

CT_AbstractMetric* CT_Reader_OPF::staticCreateMetricForType(const QString &type, const QString &value)
{
    CT_AbstractMetric *metric = NULL;

    if(type == "String")
    {
        metric = new CT_MetricT<QString>();
        ((CT_MetricT<QString>*)metric)->setValue(value);
    }
    else if(type == "Integer")
    {
        metric = new CT_MetricT<int>();

        if(!value.isEmpty())
            ((CT_MetricT<int>*)metric)->setValue(value.toInt());
    }
    else if((type == "Double")
            || (type == "Metre")
            || (type == "Centimetre")
            || (type == "Millimetre")
            || (type == "10E-5 Metre"))
    {
        metric = new CT_MetricT<double>();

        if(!value.isEmpty())
            ((CT_MetricT<double>*)metric)->setValue(value.toDouble());
    }
    else if(type == "Boolean")
    {
        metric = new CT_MetricT<bool>();

        if(!value.isEmpty())
            ((CT_MetricT<bool>*)metric)->setValue(value.toLower() == "true");
    }
    /*else if(type == "Color")
    {
        metric = new CT_MetricT<CT_Color>();
    }
    else
    {
        qWarning("CT_Reader_OPF::staticCreateMetricForType => Unknown Type");
    }*/

    return metric;
}

void CT_Reader_OPF::clearOtherModels()
{
    m_models.clear();
}

void CT_Reader_OPF::clearMeshes()
{
    qDeleteAll(m_meshes.begin(), m_meshes.end());
    m_meshes.clear();
}

void CT_Reader_OPF::clearShapes()
{
    m_shapes.clear();
}

void CT_Reader_OPF::readMesh(QXmlStreamReader &xml)
{
    int id = xml.attributes().value("Id").toString().toInt();

    CT_OPF_Mesh *mesh = new CT_OPF_Mesh();
    bool continueLoop = true;

    while(!xml.atEnd()
          && continueLoop)
    {
        xml.readNext();

        QString nn = xml.name().toString();

        if(nn == "points" && xml.isStartElement())
        {
            QString tmp = xml.readElementText().trimmed();
            QStringList points = tmp.split(QRegExp("\\s+"));

            QListIterator<QString> it(points);

            int size = points.size()/3;
            mesh->m_points.resize(size);

            int i = 0;

            while(i<size)
            {
                CT_Point &p = mesh->m_points[i];
                p.setX(it.next().toFloat());
                p.setY(it.next().toFloat());
                p.setZ(it.next().toFloat());

                ++i;
            }
        }
        else if(nn == "face" && xml.isStartElement())
        {
            CT_OPF_Face face;

            QString tmp = xml.readElementText().trimmed();
            QStringList points = tmp.split(QRegExp("\\s+"));

            for(int i=0; i<3; ++i)
                face.m_pointsIndex[i] = points.at(i).toInt();

            mesh->m_faces.push_back(face);
        }
        else if(nn == "mesh")
        {
            continueLoop = false;
        }
    }

    m_meshes.insert(id, mesh);
}

void CT_Reader_OPF::readShape(QXmlStreamReader &xml)
{
    int id = xml.attributes().value("Id").toString().toInt();

    bool continueLoop = true;

    while(!xml.atEnd()
          && continueLoop)
    {
        xml.readNext();

        QString nn = xml.name().toString();

        if(nn == "meshIndex" && xml.isStartElement())
            m_shapes.insert(id, m_meshes.value(xml.readElementText().toInt(), NULL));
        else if(nn == "shape")
            continueLoop = false;
    }
}

void CT_Reader_OPF::readGeometry(QXmlStreamReader &xml, CT_TOPFNodeGroup *node, const QString &typeName)
{
    CT_OPF_Mesh *mesh = NULL;
    bool continueLoop = true;
    double dUp = 1, dDwn = 1;

    while(!xml.atEnd()
          && continueLoop)
    {
        xml.readNext();

        QString nn = xml.name().toString();

        if(nn == "shapeIndex" && xml.isStartElement())
        {
            mesh = m_shapes.value(xml.readElementText().toInt(), NULL);
        }
        else if(nn == "mat" && xml.isStartElement())
        {
            QStringList mat = xml.readElementText().simplified().split(QRegExp("\\s+"));
            QMatrix4x4 matrix(mat.at(0).toDouble(), mat.at(1).toDouble(), mat.at(2).toDouble(), mat.at(3).toDouble(),
                              mat.at(4).toDouble(), mat.at(5).toDouble(), mat.at(6).toDouble(), mat.at(7).toDouble(),
                              mat.at(8).toDouble(), mat.at(9).toDouble(), mat.at(10).toDouble(), mat.at(11).toDouble(),
                              0,0,0,1);

            node->setOPFMatrix(matrix);
        }
        else if(nn == "dUp" && xml.isStartElement())
        {
            dUp = xml.readElementText().toDouble();
        }
        else if(nn == "dDwn" && xml.isStartElement())
        {
            dDwn = xml.readElementText().toDouble();
        }
        else if(nn == "geometry")
            continueLoop = false;
    }

    if(mesh != NULL)
        transformAndCreateMesh(mesh, node, dUp, dDwn, typeName);
}

void CT_Reader_OPF::transformAndCreateMesh(CT_OPF_Mesh *mesh, CT_TOPFNodeGroup *node, const double &dUp, const double &dDwn, const QString &typeName)
{
    CT_Mesh *newMesh = new CT_Mesh();

    QVector3D min, max;
    mesh->getBoundingBox(min, max);

    double deltaX = max.x() - min.x();

    if(deltaX == 0)
        return;

    const QMatrix4x4 &matrix = node->transformMatrix();

    double deltaD = dDwn - dUp;

    size_t nP = mesh->m_points.size();
    size_t nF = mesh->m_faces.size();

    CT_Mesh::VertexIndexIterator itP = CT_MeshAllocatorT<CT_Mesh>::AddVertices(*newMesh, nP);
    CT_Mesh::FaceIndexIterator itF = CT_MeshAllocatorT<CT_Mesh>::AddFaces(*newMesh, nF);
    std::vector<CT_Point>::const_iterator itO = mesh->m_points.begin();
    std::list<CT_OPF_Face>::const_iterator itOF = mesh->m_faces.begin();

    size_t debPointIndex = itP.cIndex();

    for(size_t i=0; i<nP; ++i)
    {
        const CT_Point &oPoint = *itO;
        CT_Point &point = itP.cT();

        float dx = point.getX() - min.x();
        float factorW = dDwn - (deltaD * (dx / deltaX));

        point.setX(oPoint.getX());
        point.setY(oPoint.getY() * factorW);
        point.setZ(oPoint.getZ() * factorW);

        CT_MathPoint::transform(matrix, point);

        point.setX(point.getX()/100.0);
        point.setY(point.getY()/100.0);
        point.setZ(point.getZ()/100.0);

        ++itP;
        ++itO;
    }

    for(size_t i=0; i<nF; ++i)
    {
        const CT_OPF_Face &oFace = *itOF;
        CT_Face &face = itF.cT();

        CT_Mesh::HEdgeIndexIterator beginHe = CT_MeshAllocatorT<CT_Mesh>::AddHEdges(*newMesh, 3);

        size_t faceIndex = itF.cIndex();

        size_t p0 = oFace.m_pointsIndex[0] + debPointIndex;
        size_t p1 = oFace.m_pointsIndex[1] + debPointIndex;
        size_t p2 = oFace.m_pointsIndex[2] + debPointIndex;

        size_t e1Index = beginHe.cIndex();
        size_t e2Index;
        size_t e3Index;

        face.setEdge(e1Index);

        CT_Edge &e1 = beginHe.cT();
        e1.setPoint0(p0);
        e1.setPoint1(p1);
        e1.setFace(faceIndex);
        ++beginHe;

        CT_Edge &e2 = beginHe.cT();
        e2.setPoint0(p1);
        e2.setPoint1(p2);
        e1.setFace(faceIndex);
        e2Index = beginHe.cIndex();
        ++beginHe;

        CT_Edge &e3 = beginHe.cT();
        e3.setPoint0(p2);
        e3.setPoint1(p0);
        e3.setFace(faceIndex);
        e3Index = beginHe.cIndex();

        e1.setNext(e2Index);
        e1.setPrevious(e3Index);
        e2.setNext(e3Index);
        e2.setPrevious(e1Index);
        e3.setNext(e1Index);
        e3.setPrevious(e2Index);

        ++itOF;
        ++itF;
    }

    node->addItemDrawable(new CT_MeshModel((CT_OutAbstractSingularItemModel*)m_models.value(typeName +  + "_mesh", NULL), NULL, newMesh));
}

void CT_Reader_OPF::protectedInit()
{
    addNewReadableFormat(FileFormat("opf", tr("Fichiers AmapStudio .opf")));
}

void CT_Reader_OPF::protectedCreateOutItemDrawableModelList()
{
    clearOtherModels();

    // Tree Group
    CT_OutStdGroupModel *topology = new CT_OutStdGroupModel(DEF_CT_Reader_OPF_topologyOut, new CT_TTreeGroup(), tr("Topologie"));
    m_models.insert(topology->uniqueName(), topology);

    QHashIterator<QString, CT_OPF_Type> it(m_types);

    while(it.hasNext())
    {
        it.next();

        const CT_OPF_Type &type = it.value();

        // Node Group
        CT_OutOPFNodeGroupModel *node = new CT_OutOPFNodeGroupModel(type.m_name, new CT_TOPFNodeGroup(), type.m_name);
        node->setOPFLevel(type.m_level);
        m_models.insert(node->uniqueName(), node);

        QHashIterator<QString, CT_OPF_Attribute> itA(type.m_attributes);

        while(itA.hasNext())
        {
            itA.next();

            const CT_OPF_Attribute &attribute = itA.value();

            CT_AbstractMetric *metric = staticCreateMetricForType(attribute.m_type);

            if(metric != NULL)
            {
                // Metric for attribute
                CT_OutStdSingularItemModel *att = new CT_OutStdSingularItemModel(type.m_name + "_" + attribute.m_name, metric, attribute.m_name);
                m_models.insert(att->uniqueName(), att);

                node->addItem(att);
            }
        }

        // Mesh
        CT_OutStdSingularItemModel *mesh = new CT_OutStdSingularItemModel(type.m_name + "_mesh", new CT_MeshModel());
        m_models.insert(mesh->uniqueName(), mesh);

        node->addItem(mesh);
        topology->addGroup(node);
    }

    addOutGroupModel(topology);
}

bool CT_Reader_OPF::protectedReadFile()
{
    clearMeshes();
    clearShapes();

    QXmlStreamReader xml;

    QFile f(filepath());

    if(f.open(QIODevice::ReadOnly))
    {
        int nNodeReaded = 0;

        CT_TTreeGroup *tree = new CT_TTreeGroup();
        tree->setModel(outGroupsModel().first());

        QStack<CT_TOPFNodeGroup*> stack;

        xml.setDevice(&f);

        if(!xml.atEnd() && xml.readNextStartElement())
        {
            while (!xml.atEnd())
            {
                QString nn = xml.name().toString();
                CT_TOPFNodeGroup *node = NULL;

                if(nn == "mesh" && xml.isStartElement())
                {
                    readMesh(xml);
                }
                else if(nn == "shape" && xml.isStartElement())
                {
                    readShape(xml);
                }
                else if(nn == "topology" && xml.isStartElement())
                {
                    QString typeName = xml.attributes().value("class").toString();

                    node = new CT_TOPFNodeGroup();
                    node->setModel(m_models.value(typeName, NULL));

                    tree->setRootNode(node);
                }
                else if(((nn == "decomp") || (nn == "follow")) && xml.isStartElement())
                {
                    QString typeName = xml.attributes().value("class").toString();

                    node = new CT_TOPFNodeGroup();
                    node->setModel(m_models.value(typeName, NULL));

                    CT_TOPFNodeGroup *lastNode = stack.top();

                    lastNode->addComponent(node);
                }
                else if(nn == "branch" && xml.isStartElement())
                {
                    QString typeName = xml.attributes().value("class").toString();

                    node = new CT_TOPFNodeGroup();
                    node->setModel(m_models.value(typeName, NULL));

                    CT_TOPFNodeGroup *lastNode = stack.top();

                    lastNode->addBranch(node);
                }
                else
                {
                    if(xml.isEndElement() && TOPOLOGY_NAMES.contains(nn))
                        stack.pop();

                    xml.readNext();
                }

                if(node != NULL)
                {
                    ++nNodeReaded;

                    node->setOPFID(xml.attributes().value("id").toString().toInt());
                    QString typeName = xml.attributes().value("class").toString();
                    stack.push(node);

                    bool continueLoop = true;

                    while(!xml.atEnd()
                           && continueLoop)
                    {
                        xml.readNext();

                        if(xml.isStartElement())
                        {
                            QString name = xml.name().toString();

                            if(name == "geometry")
                            {
                                readGeometry(xml, node, typeName);
                            }
                            else if(TOPOLOGY_NAMES.contains(name))
                            {
                                continueLoop = false;
                            }
                            else if(m_attributes.contains(name))
                            {
                                CT_AbstractMetric *metric = staticCreateMetricForType(m_attributes.value(name).m_type, xml.readElementText());

                                if(metric != NULL)
                                {
                                    CT_OutAbstractItemModel *model = m_models.value(typeName + "_" + name, NULL);
                                    metric->setModel(model);

                                    node->addItemDrawable(metric);
                                }
                            }
                        }
                        else if(xml.isEndElement() && TOPOLOGY_NAMES.contains(xml.name().toString()))
                        {
                            stack.pop();
                            xml.readNext();
                            continueLoop = false;
                        }
                    }

                    setProgress((nNodeReaded*100)/m_totalNode);
                }
            }
        }

        addOutGroup(DEF_CT_Reader_OPF_topologyOut, tree);

        f.close();

        return true;
    }

    return false;
}

// CT_OPF_Mesh //

void CT_OPF_Mesh::getBoundingBox(QVector3D &min, QVector3D &max) const
{
    min.setX(std::numeric_limits<double>::max());
    min.setY(min.x());
    min.setZ(min.x());

    max.setX(-min.x());
    max.setY(max.x());
    max.setZ(max.x());

    std::vector<CT_Point>::const_iterator it = m_points.begin();
    std::vector<CT_Point>::const_iterator end = m_points.end();

    while(it != end)
    {
        const CT_Point &p = *it;

        if(p.getX() > max.x())
            max.setX(p.getX());

        if(p.getY() > max.y())
            max.setY(p.getY());

        if(p.getZ() > max.z())
            max.setZ(p.getZ());

        if(p.getX() < min.x())
            min.setX(p.getX());

        if(p.getY() < min.y())
            min.setY(p.getY());

        if(p.getZ() < min.z())
            min.setZ(p.getZ());

        ++it;
    }
}
