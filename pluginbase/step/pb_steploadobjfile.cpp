#include "pb_steploadobjfile.h"

#include "ct_global/ct_context.h"

#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"
#include "ct_mesh/tools/ct_meshallocatort.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

#define DEF_SearchResult                "r"
#define DEF_SearchGroup                 "g"
#define DEF_SearchMesh                  "m"

PB_StepLoadObjFile::PB_StepLoadObjFile(CT_StepInitializeData &data) : CT_AbstractStepLoadFile(data)
{
    m_searchHEdges = false;
}

QString PB_StepLoadObjFile::getStepDescription() const
{
    return tr("Charge un fichier .obj dans un Mesh");
}

CT_VirtualAbstractStep* PB_StepLoadObjFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadObjFile(dataInit);
}

QList<QString> PB_StepLoadObjFile::getFileExtensionAccepted() const
{
    QList<QString> list;
    list << ".obj";

    return list;
}

void PB_StepLoadObjFile::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *dialog = newStandardPreConfigurationDialog();
    dialog->addBool("", "", tr("Rechercher les demis arêtes ? (accélère le chargement si non coché)"), m_searchHEdges);
}

void PB_StepLoadObjFile::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

// Creation and affiliation of OUT models
void PB_StepLoadObjFile::createOutResultModelListProtected()
{
    // nouvelle méthode
    CT_OutResultModelGroup *res = createNewOutResultModel("", tr("Mesh Document"));
    res->setRootGroup(DEF_SearchGroup);
    res->addItemModel(DEF_SearchGroup, DEF_SearchMesh, new CT_MeshModel(), tr("Mesh Model"));

    // ancienne méthode
    /*CT_OutStandardGroupModel *root = new CT_OutStandardGroupModel(DEF_SearchGroup);
    CT_OutStandardItemDrawableModel *meshModel = new CT_OutStandardItemDrawableModel(DEF_SearchMesh, new CT_MeshModel(), "MeshModel");

    root->addItem(meshModel);

    addOutResultModel(new CT_OutResultModelGroup(DEF_SearchResult, root, "MeshDocument"));*/
}

CT_Edge* findHEdgeTwin(const CT_Mesh *mesh, const size_t &p0, const size_t &p1)
{
    if(mesh->abstractHedge() == NULL)
        return 0;

    CT_Mesh::HEdgeIndexIterator it = mesh->hedge().begin();
    CT_Mesh::HEdgeIndexIterator end = mesh->hedge().end();

    while(it != end)
    {
        CT_Edge &he = it.cT();

        if(((he.iPointAt(0) == p1)
                            && (he.iPointAt(1) == p0)))
        {
            return &he;
        }

        ++it;
    }

    return 0;
}

void PB_StepLoadObjFile::compute()
{
    CT_ResultGroup *out_res = getOutResultList().at(0);

    CT_Mesh *mesh = new CT_Mesh();

    setErrorCode(-1);

    bool error = false;

    if(QFile::exists(getFilePath()))
    {
        QFile f(getFilePath());

        if(f.open(QIODevice::ReadOnly))
        {
            setErrorCode(0);

            size_t  nv, nf;
            nv = 0;
            nf = 0;

            QTextStream stream(&f);
            QString buf;

            while(!stream.atEnd()
                  && !isStopped())
            {
                buf = f.readLine();

                if(buf[0] == 'v')
                {
                    if(buf[1] == ' ' || buf[1] == '\t')
                        ++nv;
                }
                else if(buf[0] == 'f')
                {
                    if(buf[1] == ' ' || buf[1] == '\t')
                        ++nf;
                }
            }

            if(!isStopped())
            {

                CT_Mesh::VertexIndexIterator beginV = CT_MeshAllocatorT<CT_Mesh>::AddVertices(*mesh, nv);
                CT_Mesh::FaceIndexIterator beginF = CT_MeshAllocatorT<CT_Mesh>::AddFaces(*mesh, nf);

                CT_Mesh::VertexIndexIterator beginVOrig = beginV;

                size_t nfLoaded = 0;

                stream.seek(0);

                while(!error
                      && !stream.atEnd()
                      && !isStopped())
                {
                    buf = f.readLine();

                    if(buf[0] == 'v')
                    {
                        if(buf[1] == ' ' || buf[1] == '\t')
                        {
                            QStringList sl = buf.split(" ", QString::SkipEmptyParts);

                            if(sl.size() >= 4)
                            {
                                CT_Point &p = beginV.cT();
                                p.data[0] = sl.at(1).toFloat();
                                p.data[1] = sl.at(2).toFloat();
                                p.data[2] = sl.at(3).toFloat();

                                ++beginV;
                            }
                        }
                    }
                    else if(buf[0] == 'f')
                    {
                        QStringList sl = buf.split(" ", QString::SkipEmptyParts);

                        if(sl.size() >= 4)
                        {
                            size_t p0,p1,p2;

                            p0 = (beginVOrig + (sl.at(1).split("/").at(0).toInt() - 1)).cIndex();
                            p1 = (beginVOrig + (sl.at(2).split("/").at(0).toInt() - 1)).cIndex();
                            p2 = (beginVOrig + (sl.at(3).split("/").at(0).toInt() - 1)).cIndex();

                            CT_Face &face = beginF.cT();
                            size_t faceIndex = beginF.cIndex();

                            CT_Mesh::HEdgeIndexIterator beginHe = CT_MeshAllocatorT<CT_Mesh>::AddHEdges(*mesh, 3);

                            CT_Edge *twinE1 = NULL;
                            CT_Edge *twinE2 = NULL;
                            CT_Edge *twinE3 = NULL;

                            if(m_searchHEdges)
                            {
                                twinE1 = findHEdgeTwin(mesh, p0, p1);
                                twinE2 = findHEdgeTwin(mesh, p1, p2);
                                twinE3 = findHEdgeTwin(mesh, p2, p0);
                            }

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

                            ++beginF;
                            ++nfLoaded;

                            setProgress((nfLoaded*100)/nf);
                        }
                    }
                }
            }

            f.close();
        }
    }

    CT_StandardItemGroup *group = new CT_StandardItemGroup(DEF_SearchGroup, out_res);
    group->addItemDrawable(new CT_MeshModel(DEF_SearchMesh, out_res, mesh));
    out_res->addGroup(group);
}
