#include "ct_reader_obj.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"
#include "ct_mesh/tools/ct_meshallocator.h"
#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"

CT_Reader_OBJ::CT_Reader_OBJ() : CT_AbstractReader()
{
    m_searchHEdges = false;
    m_loadAsPointCloud = false;
}

QString CT_Reader_OBJ::GetReaderName() const
{
    return tr("Mesh ou Points, Fichier OBJ");
}

CT_StepsMenu::LevelPredefined CT_Reader_OBJ::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Meshes;
}

void CT_Reader_OBJ::setLoadAsPointCloud(bool enable)
{
    m_loadAsPointCloud = enable;
}

bool CT_Reader_OBJ::loadAsPointCloud() const
{
    return m_loadAsPointCloud;
}

void CT_Reader_OBJ::setSearchHalfEdges(bool enable)
{
    m_searchHEdges = enable;
}

bool CT_Reader_OBJ::searchHalfEdges() const
{
    return m_searchHEdges;
}

bool CT_Reader_OBJ::configure()
{
    CT_GenericConfigurableWidget dialog;
    dialog.addBool("", "", tr("Rechercher les demis arêtes ? (accélère le chargement si non coché)"), m_searchHEdges);
    dialog.addBool("", "", tr("Forcer le chargement en tant que nuage de points"), m_loadAsPointCloud);

    return (CT_ConfigurableWidgetToDialog::exec(&dialog) == QDialog::Accepted);
}

SettingsNodeGroup* CT_Reader_OBJ::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractReader::getAllSettings();

    SettingsNodeGroup *group = new SettingsNodeGroup("CT_Reader_OBJ_Settings");
    group->addValue(new SettingsNodeValue("searchHEdges", m_searchHEdges));
    group->addValue(new SettingsNodeValue("loadAsPointCloud", m_loadAsPointCloud));

    root->addGroup(group);

    return root;
}

bool CT_Reader_OBJ::setAllSettings(const SettingsNodeGroup *settings)
{
    if(CT_AbstractReader::setAllSettings(settings))
    {
        QList<SettingsNodeGroup*> listG = settings->groupsByTagName("CT_Reader_OBJ_Settings");

        if(!listG.isEmpty())
        {
            QList<SettingsNodeValue*> listV = listG.first()->valuesByTagName("searchHEdges");

            if(listV.isEmpty())
                return false;

            setSearchHalfEdges(listV.first()->value().toBool());

            listV = listG.first()->valuesByTagName("loadAsPointCloud");

            if(listV.isEmpty())
                return false;

            setLoadAsPointCloud(listV.first()->value().toBool());

            return true;
        }
    }

    return false;
}

CT_AbstractReader* CT_Reader_OBJ::copy() const
{
    return new CT_Reader_OBJ();
}

void CT_Reader_OBJ::protectedInit()
{
    addNewReadableFormat(FileFormat("obj", tr("Fichiers de géométrie 3D .obj")));

    setToolTip(tr("Charge un maillage ou un nuage de points depuis un fichier au format OBJ (Objet 3D)"));
}

void CT_Reader_OBJ::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    if(loadAsPointCloud())
        addOutItemDrawableModel(DEF_CT_Reader_OBJ_meshOrSceneOut, new CT_Scene(), tr("Scene"));
    else
        addOutItemDrawableModel(DEF_CT_Reader_OBJ_meshOrSceneOut, new CT_MeshModel(), tr("Mesh Model"));
}

bool CT_Reader_OBJ::protectedReadFile()
{
    Eigen::Vector3d min;
    min[0] = std::numeric_limits<double>::max();
    min[1] = min[0];
    min[2] = min[0];

    Eigen::Vector3d max;
    max[0] = -min[0];
    max[1] = -min[0];
    max[2] = -min[0];

    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);

            if(loadAsPointCloud()) {
                CT_NMPCIR pcir = loadFileAsPointCloud(stream, f.size(), min, max);

                if(!pcir.isNull()) {

                    CT_Scene *scene = new CT_Scene(NULL, NULL, pcir);
                    scene->setBoundingBox(min[0], min[1], min[2], max[0], max[1], max[2]);

                    addOutItemDrawable(DEF_CT_Reader_OBJ_meshOrSceneOut, scene);
                }
            } else {
                CT_Mesh *mesh = loadFileAsMesh(stream, min, max);

                if(mesh != NULL) {
                    CT_MeshModel *meshModel = new CT_MeshModel(NULL, NULL, mesh);
                    meshModel->setBoundingBox(min[0], min[1], min[2], max[0], max[1], max[2]);

                    addOutItemDrawable(DEF_CT_Reader_OBJ_meshOrSceneOut, meshModel);
                }
            }

            f.close();

            return true;
        }
    }

    return false;
}

bool CT_Reader_OBJ::checkIsAVertex(const QString &buf) const
{
    return !buf.isEmpty() && (buf[0] == 'v');
}

bool CT_Reader_OBJ::checkHasInfoOfVertex(const QString &buf) const
{
    return((buf[1] == ' ') || (buf[1] == '\t'));
}

bool CT_Reader_OBJ::loadVertex(const QString &buf, CT_Point &point) const
{
    QStringList sl = buf.split(" ", QString::SkipEmptyParts);

    if(sl.size() >= 4)
    {
        point(0) = sl.at(1).toDouble();
        point(1) = sl.at(2).toDouble();
        point(2) = sl.at(3).toDouble();

        return true;
    }

    return false;
}

bool CT_Reader_OBJ::checkIsAFace(const QString &buf) const
{
    return !buf.isEmpty() && (buf[0] == 'f');
}

bool CT_Reader_OBJ::checkHasInfoOfFace(const QString &buf) const
{
    return((buf[1] == ' ') || (buf[1] == '\t'));
}

bool CT_Reader_OBJ::loadFace(const QString &buf, const size_t &beginVertexIndex, CT_Mesh *mesh, CT_MutableFaceIterator &itFaces) const
{
    QStringList sl = buf.split(" ", QString::SkipEmptyParts);

    if(sl.size() >= 4)
    {
        size_t p0,p1,p2;

        p0 = beginVertexIndex + (sl.at(1).split("/").at(0).toInt() - 1);
        p1 = beginVertexIndex + (sl.at(2).split("/").at(0).toInt() - 1);
        p2 = beginVertexIndex + (sl.at(3).split("/").at(0).toInt() - 1);

        CT_Face &face = itFaces.next().cT();
        size_t faceIndex = itFaces.cIndex();

        CT_MutableEdgeIterator beginHe = CT_MeshAllocator::AddHEdges(mesh, 3);

        CT_Edge *twinE1 = NULL;
        CT_Edge *twinE2 = NULL;
        CT_Edge *twinE3 = NULL;

        if(m_searchHEdges)
        {
            twinE1 = findHEdgeTwin(mesh, p0, p1);
            twinE2 = findHEdgeTwin(mesh, p1, p2);
            twinE3 = findHEdgeTwin(mesh, p2, p0);
        }

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
        e2.setFace(faceIndex);
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

        if(twinE1 != NULL)
        {
            e1.setTwin(twinE1);
            twinE1->setTwin(e1Index);
        }

        if(twinE2 != NULL)
        {
            e2.setTwin(twinE2);
            twinE2->setTwin(e2Index);
        }

        if(twinE3 != NULL)
        {
            e3.setTwin(twinE3);
            twinE3->setTwin(e3Index);
        }

        return true;
    }

    return false;
}

CT_Edge* CT_Reader_OBJ::findHEdgeTwin(const CT_Mesh *mesh, const size_t &p0, const size_t &p1) const
{
    if(mesh->abstractHedge() == NULL)
        return 0;

    CT_EdgeIterator it(mesh->abstractHedge());

    while(it.hasNext())
    {
        CT_Edge &he = it.next().cT();

        if(((he.iPointAt(0) == p1)
                            && (he.iPointAt(1) == p0)))
            return &he;
    }

    return 0;
}

void CT_Reader_OBJ::updateMinMax(const CT_Point &point, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax) const
{
    bboxMin[0] = qMin(point[CT_Point::X], bboxMin[0]);
    bboxMin[1] = qMin(point[CT_Point::Y], bboxMin[1]);
    bboxMin[2] = qMin(point[CT_Point::Z], bboxMin[2]);

    bboxMax[0] = qMax(point[CT_Point::X], bboxMax[0]);
    bboxMax[1] = qMax(point[CT_Point::Y], bboxMax[1]);
    bboxMax[2] = qMax(point[CT_Point::Z], bboxMax[2]);
}

CT_Mesh* CT_Reader_OBJ::loadFileAsMesh(QTextStream &stream, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax)
{
    CT_Mesh *mesh = NULL;

    size_t nv, nf;
    nv = 0;
    nf = 0;

    QString buf;

    while(!stream.atEnd()
          && !isStopped())
    {
        buf = stream.readLine();

        if(checkIsAVertex(buf))
        {
            if(checkHasInfoOfVertex(buf))
                ++nv;
        }
        else if(checkIsAFace(buf))
        {
            if(checkHasInfoOfFace(buf))
                ++nf;
        }
    }

    if(!isStopped()
            && (nv > 0)
            && (nf > 0))
    {
        mesh = new CT_Mesh();

        CT_MutablePointIterator beginV = CT_MeshAllocator::AddVertices(mesh, nv);
        CT_MutableFaceIterator beginF = CT_MeshAllocator::AddFaces(mesh, nf);

        size_t beginVertexIndex = beginV.next().cIndex();
        beginV.toFront();

        size_t nfLoaded = 0;
        CT_Point point;
        QString buf;

        stream.seek(0);

        while(!stream.atEnd()
              && !isStopped())
        {
            buf = stream.readLine();

            if(checkIsAVertex(buf))
            {
                if(checkHasInfoOfVertex(buf))
                {
                    if(loadVertex(buf, point)) {
                        beginV.next().replaceCurrentPoint(point);
                        updateMinMax(point, bboxMin, bboxMax);
                    }
                }
            }
            else if(checkIsAFace(buf))
            {
                if(checkHasInfoOfFace(buf))
                {
                    if(loadFace(buf, beginVertexIndex, mesh, beginF)) {
                        ++nfLoaded;
                        setProgress((nfLoaded*100)/nf);
                    }
                }
            }
        }
    }

    return mesh;
}

CT_NMPCIR CT_Reader_OBJ::loadFileAsPointCloud(QTextStream &stream, const qint64 &fileSize, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax)
{
    CT_AbstractUndefinedSizePointCloud* mpcir = PS_REPOSITORY->createNewUndefinedSizePointCloud();
    CT_Point point;

    size_t p = 0;
    QString buf;

    stream.seek(0);

    while(!stream.atEnd()
          && !isStopped())
    {
        buf = stream.readLine();

        if(checkIsAVertex(buf))
        {
            if(checkHasInfoOfVertex(buf))
            {
                if(loadVertex(buf, point)) {
                    mpcir->addPoint(point);
                    updateMinMax(point, bboxMin, bboxMax);
                }
            }
        }

        p += buf.size();

        setProgress((p*100)/fileSize);
    }

    return PS_REPOSITORY->registerUndefinedSizePointCloud(mpcir);
}

