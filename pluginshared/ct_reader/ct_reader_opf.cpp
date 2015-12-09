#include "ct_reader_opf.h"

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"
#include "ct_attributes/model/outModel/ct_outstditemattributemodel.h"
#include "ct_itemdrawable/ct_itemattributelist.h"
#include "ct_itemdrawable/ct_ttreegroup.h"
#include "ct_itemdrawable/ct_topfnodegroup.h"
#include "ct_itemdrawable/ct_opfmeshmodel.h"
#include "ct_attributes/ct_stditemattributet.h"
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

    setToolTip(tr("Charge un fichier décrivant la topologie de végétaux au format OPF, créé par le logiciel AMAPStudio.<br>http://amapstudio.cirad.fr"));
}

CT_Reader_OPF::~CT_Reader_OPF()
{
    clearDrawManagers();
    clearOtherModels();
    clearMeshes();
    clearShapes();
}


QString CT_Reader_OPF::GetReaderName() const
{
    return tr("Fichier AmapStudio - OPF");
}

CT_StepsMenu::LevelPredefined CT_Reader_OPF::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Others;
}

void CT_Reader_OPF::recursiveReadTopologyForModel(rapidxml::xml_node<> *node,
                                                  int &totalNode,
                                                  QHash<QString, CT_OPF_Type> &types,
                                                  const QHash<QString, CT_OPF_Attribute> &attributes)
{
    ++totalNode;

    QString nameT = node->first_attribute("class")->value();

    CT_OPF_Type type = types.value(nameT, CT_OPF_Type(nameT,
                                                      QString(node->first_attribute("scale")->value()).toInt(),
                                                      QString(node->first_attribute("id")->value()).toInt()));

    bool addAttributes = (type.m_attributes.size() != attributes.size());

    rapidxml::xml_node<> *nextNode = node->first_node();

    while(nextNode != NULL)
    {
        QString name = nextNode->name();

        if(TOPOLOGY_NAMES.contains(name))
        {
            recursiveReadTopologyForModel(nextNode,
                                          totalNode,
                                          types,
                                          attributes);
        }
        else if(addAttributes
                && attributes.contains(name))
        {
            if(!type.m_attributes.contains(name))
                type.m_attributes.insert(name, attributes.value(name));
        }

        nextNode = nextNode->next_sibling();
    }

    types.insert(nameT, type);
}

void CT_Reader_OPF::recursiveReadTopology(rapidxml::xml_node<> *xmlNode,
                                          CT_TOPFNodeGroup *node,
                                          int &nNodeReaded)
{
    ++nNodeReaded;

    node->setOPFID(QString(xmlNode->first_attribute("id")->value()).toInt());
    QString typeName = xmlNode->first_attribute("class")->value();

    CT_ItemAttributeList *attList = new CT_ItemAttributeList();
    attList->setModel((CT_OutAbstractItemModel*)m_models.value(typeName + "_attList", NULL));
    node->addItemDrawable(attList);

    rapidxml::xml_node<> *xmlChild = xmlNode->first_node();

    while(xmlChild != NULL)
    {
        QString name = xmlChild->name();
        CT_TOPFNodeGroup *newNode = NULL;

        if(name == "geometry")
        {
            readGeometry(xmlChild, node, typeName);
        }
        else if(((name == "decomp") || (name == "follow")))
        {
            QString typeName = xmlChild->first_attribute("class")->value();

            newNode = new CT_TOPFNodeGroup();
            newNode->setModel(static_cast<CT_OutAbstractItemModel*>(m_models.value(typeName, NULL)));

            node->addComponent(newNode);
        }
        else if(name == "branch")
        {
            QString typeName = xmlChild->first_attribute("class")->value();

            newNode = new CT_TOPFNodeGroup();
            newNode->setModel(static_cast<CT_OutAbstractItemModel*>(m_models.value(typeName, NULL)));

            node->addBranch(newNode);
        }
        else if(m_attributes.contains(name))
        {
            CT_AbstractItemAttribute *att = staticCreateAttributeForType(m_attributes.value(name).m_type, xmlChild->value());

            if(att != NULL)
            {
                CT_OutAbstractItemAttributeModel *model = static_cast<CT_OutAbstractItemAttributeModel*>(m_models.value(typeName + "_" + name, NULL));
                att->setModel(model);

                attList->addItemAttribute(att);
            }
        }

        if(newNode != NULL)
        {
            recursiveReadTopology(xmlChild,
                                  newNode,
                                  nNodeReaded);
        }

        xmlChild = xmlChild->next_sibling();

        setProgress((nNodeReaded*100)/m_totalNode);
    }
}

bool CT_Reader_OPF::setFilePath(const QString &filepath)
{
    m_filePath = filepath;

    QHash<QString, CT_OPF_Attribute>    attributes;
    QHash<QString, CT_OPF_Type>         types;
    int                                 totalNode = 0;

    // Test File validity
    if(QFile::exists(filepath))
    {
        rapidxml::file<> xmlFile(filepath.toLatin1().data());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        rapidxml::xml_node<> *root = doc.first_node("opf");

        if(root != NULL)
        {
            rapidxml::xml_node<> *node = NULL;

            node = root->first_node();

            while(node != NULL)
            {
                QString nn = node->name();

                if(nn == "attributeBDD")
                {
                    rapidxml::xml_node<> *child = node->first_node();

                    while(child  != NULL)
                    {
                        QString name = child->first_attribute("name")->value();

                        attributes.insert(name, CT_OPF_Attribute(name, child->first_attribute("class")->value()));

                        child = child->next_sibling();
                    }
                }
                else if(TOPOLOGY_NAMES.contains(nn))
                {
                    recursiveReadTopologyForModel(node,
                                                  totalNode,
                                                  types,
                                                  attributes);
                    node = NULL;
                }

                if(node != NULL)
                    node = node->next_sibling();
            }
        }
    }

    if(!types.isEmpty())
    {
        m_types = types;
        m_attributes = attributes;
        m_totalNode = totalNode;

        m_header = new CT_FileHeader(NULL, NULL);
        m_header->setFile(m_filePath);
        return true;
    }
    else {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("No types found in %1").arg(filepath));
    }

    return false;
}

CT_AbstractReader* CT_Reader_OPF::copy() const
{
    return new CT_Reader_OPF();
}

CT_AbstractItemAttribute* CT_Reader_OPF::staticCreateAttributeForType(const QString &type, const QString &value)
{
    CT_AbstractItemAttribute *att = NULL;

    if((type == "String") || (type == "Color"))
    {
        att = new CT_StdItemAttributeT<QString>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(CT_AbstractCategory::DATA_VALUE), NULL, value);
    }
    else if(type == "Integer")
    {
        att = new CT_StdItemAttributeT<int>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(CT_AbstractCategory::DATA_VALUE), NULL, value.toInt());
    }
    else if((type == "Double")
            || (type == "Metre")
            || (type == "Centimetre")
            || (type == "Millimetre")
            || (type == "10E-5 Metre"))
    {
        att = new CT_StdItemAttributeT<double>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(CT_AbstractCategory::DATA_VALUE), NULL, value.toDouble());
    }
    else if(type == "Boolean")
    {
        att = new CT_StdItemAttributeT<bool>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(CT_AbstractCategory::DATA_VALUE), NULL, value.toLower() == "true");
    }
    else
    {
        PS_LOG->addWarningMessage(LogInterface::reader, "CT_Reader_OPF::staticCreateAttributeForType ( type = " + type + " ) => Unknown Type");
    }

    return att;
}

void CT_Reader_OPF::clearOtherModels()
{
    qDeleteAll(m_attributesOriginalModels.begin(), m_attributesOriginalModels.end());
    m_attributesOriginalModels.clear();

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

void CT_Reader_OPF::clearDrawManagers()
{
    qDeleteAll(m_drawManager.begin(), m_drawManager.end());
    m_drawManager.clear();
}

void CT_Reader_OPF::readMesh(rapidxml::xml_node<> *xmlNode)
{
    int id = QString(xmlNode->first_attribute("Id")->value()).toInt();

    CT_OPF_Mesh *mesh = new CT_OPF_Mesh();
    mesh->m_mesh = new CT_Mesh();

    size_t debPointIndex = 0;

    rapidxml::xml_node<> *xmlChild = xmlNode->first_node();

    while(xmlChild != NULL)
    {
        QString nn = xmlChild->name();

        if(nn == "points")
        {
            QString tmp = QString(xmlChild->value()).trimmed();
            QStringList points = tmp.split(QRegExp("\\s+"));

            QListIterator<QString> it(points);

            int size = points.size()/3;

            CT_MutablePointIterator itP = CT_MeshAllocator::AddVertices(mesh->m_mesh, size);
            debPointIndex = itP.next().cIndex();

            CT_Point pAdded;

            int i = 0;

            while(i<size)
            {
                pAdded(0) = it.next().toDouble();
                pAdded(1) = it.next().toDouble();
                pAdded(2) = it.next().toDouble();

                itP.replaceCurrentPoint(pAdded);

                ++i;
                itP.next();
            }
        }
        else if(nn == "faces")
        {
            rapidxml::xml_node<> *xmlFace = xmlChild->first_node("face");

            while(xmlFace != NULL)
            {
                QString tmp = QString(xmlFace->value()).trimmed();
                QStringList points = tmp.split(QRegExp("\\s+"));

                CT_MutableFaceIterator itF = CT_MeshAllocator::AddFaces(mesh->m_mesh, 1);

                CT_Face &face = itF.next().cT();

                CT_MutableEdgeIterator beginHe = CT_MeshAllocator::AddHEdges(mesh->m_mesh, 3);

                size_t faceIndex = itF.cIndex();

                size_t p0 = points.at(0).toInt() + debPointIndex;
                size_t p1 = points.at(1).toInt() + debPointIndex;
                size_t p2 = points.at(2).toInt() + debPointIndex;

                size_t e1Index = beginHe.next().cIndex();
                size_t e2Index;
                size_t e3Index;

                face.setEdge(e1Index);

                CT_Edge &e1 = beginHe.cT();
                e1.setPoint0(p0);
                e1.setPoint1(p1);
                e1.setFace(faceIndex);

                CT_Edge &e2 = beginHe.next().cT();
                e2.setPoint0(p1);
                e2.setPoint1(p2);
                e1.setFace(faceIndex);
                e2Index = beginHe.cIndex();

                CT_Edge &e3 = beginHe.next().cT();
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

                xmlFace = xmlFace->next_sibling();
            }
        }

        xmlChild = xmlChild->next_sibling();
    }

    m_meshes.insert(id, mesh);
}

void CT_Reader_OPF::readShape(rapidxml::xml_node<> *xmlNode)
{
    int id = QString(xmlNode->first_attribute("Id")->value()).toInt();

    rapidxml::xml_node<> *xmlChild = xmlNode->first_node("meshIndex");

    if(xmlChild != NULL)
        m_shapes.insert(id, m_meshes.value(QString(xmlChild->value()).toInt(), NULL));
}

void CT_Reader_OPF::readGeometry(rapidxml::xml_node<> *xmlNode, CT_TOPFNodeGroup *node, const QString &typeName)
{
    CT_OPF_Mesh *mesh = NULL;
    double dUp = 1, dDwn = 1;

    rapidxml::xml_node<> *xmlChild = xmlNode->first_node();

    while(xmlChild != NULL)
    {
        QString nn = xmlChild->name();

        if(nn == "shapeIndex")
        {
            mesh = m_shapes.value(QString(xmlChild->value()).toInt(), NULL);
        }
        else if(nn == "mat")
        {
            QStringList mat = QString(xmlChild->value()).simplified().split(QRegExp("\\s+"));
            QMatrix4x4 matrix(mat.at(0).toDouble()/100.0, mat.at(1).toDouble()/100.0, mat.at(2).toDouble()/100.0, mat.at(3).toDouble()/100.0,
                              mat.at(4).toDouble()/100.0, mat.at(5).toDouble()/100.0, mat.at(6).toDouble()/100.0, mat.at(7).toDouble()/100.0,
                              mat.at(8).toDouble()/100.0, mat.at(9).toDouble()/100.0, mat.at(10).toDouble()/100.0, mat.at(11).toDouble()/100.0,
                              0,0,0,1);

            node->setOPFMatrix(matrix);
        }
        else if(nn == "dUp")
        {
            dUp = QString(xmlChild->value()).toDouble();
        }
        else if(nn == "dDwn")
        {
            dDwn = QString(xmlChild->value()).toDouble();
        }

        xmlChild = xmlChild->next_sibling();
    }

    if(mesh != NULL)
    {
        Eigen::Vector3d min, max;
        mesh->getBoundingBox(min, max);

        transformAndCreateMesh(mesh->m_mesh, min, max, node, dUp, dDwn, typeName);
    }
    else
    {
        if(m_cylinderMesh.m_mesh == NULL)
        {
            m_cylinderMesh.m_mesh = new CT_Mesh();
            m_cylinderMesh.m_mesh->createCylinder(0.5,0.5,10);
        }

        Eigen::Vector3d min, max;
        m_cylinderMesh.getBoundingBox(min, max);

        transformAndCreateMesh(m_cylinderMesh.m_mesh, min, max, node, dUp, dDwn, typeName);
    }

}

void CT_Reader_OPF::transformAndCreateMesh(CT_Mesh *mesh, Eigen::Vector3d &min, Eigen::Vector3d &max, CT_TOPFNodeGroup *node, const double &dUp, const double &dDwn, const QString &typeName)
{
    CT_Mesh *newMesh = new CT_Mesh();

    double deltaX = max.x() - min.x();

    if(deltaX == 0)
        return;

    const QMatrix4x4 &matrix = node->opfMatrix();

    double deltaD = dDwn - dUp;

    size_t nP = mesh->vert().size();
    size_t nF = mesh->face().size();

    CT_MutablePointIterator itP = CT_MeshAllocator::AddVertices(newMesh, nP);
    CT_MutableFaceIterator itF = CT_MeshAllocator::AddFaces(newMesh, nF);

    CT_MutablePointIterator itOP(mesh->pVert());
    CT_MutableFaceIterator itOF(mesh->pFace());

    if(itP.hasNext()){

        itP.next();

        size_t debPointIndex = itP.cIndex();

        itP.toFront();

        while(itP.hasNext())
        {
            itOP.next();
            itP.next();

            const CT_Point &oPoint = itOP.currentPoint();
            CT_Point point;

            float dx = oPoint[CT_Point::X] - min.x();
            float factorW = dDwn - (deltaD * (dx / deltaX));

            point.setX(oPoint[CT_Point::X]);
            point.setY(oPoint[CT_Point::Y] * factorW);
            point.setZ(oPoint[CT_Point::Z] * factorW);

            CT_MathPoint::transform(matrix, point);

            point.setX(point[CT_Point::X]/100.0);
            point.setY(point[CT_Point::Y]/100.0);
            point.setZ(point[CT_Point::Z]/100.0);

            itP.replaceCurrentPoint(point);
        }

        while(itF.hasNext())
        {
            itOF.next();
            itF.next();

            const CT_Face &oFace = itOF.cT();
            CT_Face &face = itF.cT();

            CT_MutableEdgeIterator itE = CT_MeshAllocator::AddHEdges(newMesh, 3);
            itE.next();

            size_t faceIndex = itF.cIndex();

            size_t p0 = oFace.iPointAt(0) + debPointIndex;
            size_t p1 = oFace.iPointAt(1) + debPointIndex;
            size_t p2 = oFace.iPointAt(2) + debPointIndex;

            size_t e1Index = itE.cIndex();
            size_t e2Index;
            size_t e3Index;

            face.setEdge(e1Index);

            CT_Edge &e1 = itE.cT();
            e1.setPoint0(p0);
            e1.setPoint1(p1);
            e1.setFace(faceIndex);
            itE.next();

            CT_Edge &e2 = itE.cT();
            e2.setPoint0(p1);
            e2.setPoint1(p2);
            e1.setFace(faceIndex);
            e2Index = itE.cIndex();
            itE.next();

            CT_Edge &e3 = itE.cT();
            e3.setPoint0(p2);
            e3.setPoint1(p0);
            e3.setFace(faceIndex);
            e3Index = itE.cIndex();

            e1.setNext(e2Index);
            e1.setPrevious(e3Index);
            e2.setNext(e3Index);
            e2.setPrevious(e1Index);
            e3.setNext(e1Index);
            e3.setPrevious(e2Index);
        }
    }

    CT_OPFMeshModel *meshModel = new CT_OPFMeshModel((CT_OutAbstractSingularItemModel*)m_models.value(typeName +  + "_mesh", NULL), NULL, newMesh);
    meshModel->setDUp(dUp);
    meshModel->setDDown(dDwn);

    node->addItemDrawable(meshModel);
}

void CT_Reader_OPF::protectedInit()
{
    addNewReadableFormat(FileFormat("opf", tr("Fichiers AmapStudio .opf")));
}

void CT_Reader_OPF::protectedCreateOutItemDrawableModelList()
{
    clearOtherModels();

    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

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

        // Mesh model
        CT_OutStdSingularItemModel *mesh = new CT_OutStdSingularItemModel(type.m_name + "_mesh", new CT_OPFMeshModel(), tr("Mesh"));
        m_models.insert(mesh->uniqueName(), mesh);


        // Attribute list
        CT_OutStdSingularItemModel *attList = new CT_OutStdSingularItemModel(type.m_name + "_attList", new CT_ItemAttributeList(), type.m_name);
        m_models.insert(attList->uniqueName(), attList);

        QHashIterator<QString, CT_OPF_Attribute> itA(type.m_attributes);

        while(itA.hasNext())
        {
            itA.next();

            const CT_OPF_Attribute &attribute = itA.value();

            CT_AbstractItemAttribute *att = staticCreateAttributeForType(attribute.m_type);

            if(att != NULL)
            {
                CT_OutAbstractItemAttributeModel *oAttModel = m_attributesOriginalModels.value(attribute.m_name, NULL);

                if(oAttModel == NULL)
                {
                    // Attribute
                    oAttModel = new CT_OutStdItemAttributeModel(attribute.m_name, att, attribute.m_name);
                    m_attributesOriginalModels.insert(oAttModel->uniqueName(), oAttModel);
                }

                oAttModel = (CT_OutAbstractItemAttributeModel*)oAttModel->copy();
                oAttModel->setUniqueName(type.m_name + "_" + attribute.m_name);

                m_models.insert(oAttModel->uniqueName(), oAttModel);

                attList->addItemAttribute(oAttModel);
            }
        }

        node->addItem(mesh);
        node->addItem(attList);
        topology->addGroup(node);
    }

    addOutGroupModel(topology);
}

bool CT_Reader_OPF::protectedReadFile()
{
    clearMeshes();
    clearShapes();
    clearDrawManagers();

    // Test File validity
    if(QFile::exists(filepath()))
    {
        rapidxml::file<> xmlFile(filepath().toLatin1().data());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        rapidxml::xml_node<> *xmlRoot = doc.first_node("opf");

        int nNodeReaded = 0;

        if(xmlRoot != NULL)
        {
            CT_TTreeGroup *tree = new CT_TTreeGroup();
            tree->setModel(outGroupsModel().first());

            rapidxml::xml_node<> *xmlNode = NULL;

            xmlNode = xmlRoot->first_node();

            while(xmlNode != NULL)
            {
                CT_TOPFNodeGroup *node = NULL;

                QString nn = xmlNode->name();

                if(nn == "meshBDD")
                {
                    rapidxml::xml_node<> *xmlMesh = xmlNode->first_node("mesh");

                    while(xmlMesh != NULL)
                    {
                        readMesh(xmlMesh);
                        xmlMesh = xmlMesh->next_sibling();
                    }
                }
                else if(nn == "shapeBDD")
                {
                    rapidxml::xml_node<> *xmlShape = xmlNode->first_node("shape");

                    while(xmlShape != NULL)
                    {
                        readShape(xmlShape);
                        xmlShape = xmlShape->next_sibling();
                    }
                }
                else if(nn == "topology")
                {
                    QString typeName = xmlNode->first_attribute("class")->value();

                    node = new CT_TOPFNodeGroup();
                    node->setModel(static_cast<CT_OutAbstractItemModel*>(m_models.value(typeName, NULL)));

                    tree->setRootNode(node);
                }

                if(node != NULL)
                {
                    recursiveReadTopology(xmlNode,
                                          node,
                                          nNodeReaded);
                }

                xmlNode = xmlNode->next_sibling();
            }

            addOutGroup(DEF_CT_Reader_OPF_topologyOut, tree);
        }

        return true;
    }

    return false;
}

// CT_OPF_Mesh //

void CT_OPF_Mesh::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    if(!m_bboxOK)
    {
        m_min(0) = std::numeric_limits<double>::max();
        m_min(1) = m_min.x();
        m_min(2) = m_min.x();

        m_max(0) = -m_min.x();
        m_max(1) = m_max.x();
        m_max(2) = m_max.x();

        CT_PointIterator it(m_mesh->abstractVert());

        while(it.hasNext())
        {
            const CT_Point &p = it.next().currentPoint();

            if(p(0) > m_max.x())
                m_max(0) = p(0);

            if(p(1) > m_max.y())
                m_max(1) = p(1);

            if(p(2) > m_max.z())
                m_max(2) = p(2);

            if(p(0) < m_min.x())
                m_min(0) = p(0);

            if(p(1) < m_min.y())
                m_min(1) = p(1);

            if(p(2) < m_min.z())
                m_min(2) = p(2);
        }

        m_bboxOK = true;
    }

    min = m_min;
    max = m_max;
}
