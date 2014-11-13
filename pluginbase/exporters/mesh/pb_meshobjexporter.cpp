#include "pb_meshobjexporter.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "ct_itemdrawable/ct_meshmodel.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

PB_MeshObjExporter::PB_MeshObjExporter() : CT_AbstractExporter()
{
}

QString PB_MeshObjExporter::getExporterCustomName() const
{
    return "Meshs, format OBJ";
}

void PB_MeshObjExporter::init()
{
    addNewExportFormat(FileFormat("obj", tr("Fichiers .obj")));
}

bool PB_MeshObjExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_MeshModel*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_MeshModel"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

CT_AbstractExporter* PB_MeshObjExporter::copy() const
{
    return new PB_MeshObjExporter();
}

bool PB_MeshObjExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "pbm";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&file);
        stream << "# " << QFileInfo(filePath).fileName() << endl << endl;

        int totalToExport = itemDrawableToExport().size();
        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            CT_Mesh *mesh = dynamic_cast<CT_MeshModel*>(item)->mesh();

            if(mesh != NULL)
            {
                const CT_AbstractPointCloudIndex *constPCIndex = mesh->abstractVert();
                const CT_AbstractFaceCloudIndex *constFCIndex = mesh->abstractFace();

                QHash<size_t, size_t> hashTablePoint;

                size_t totalSize = constPCIndex->size();
                size_t i = 0;

                CT_AbstractPointCloudIndex::ConstIterator beginP = constPCIndex->constBegin();
                CT_AbstractPointCloudIndex::ConstIterator endP = constPCIndex->constEnd();

                while(beginP != endP)
                {
                    const CT_Point &point = beginP.cT();
                    hashTablePoint.insert(beginP.cIndex(), i+1);

                    stream << "v " << ((double)point(0)) << " " << ((double)point(1)) << " " << ((double)point(2)) << endl;

                    ++beginP;
                    ++i;

                    setExportProgress((((i*50)/totalSize)+nExported)/totalToExport);
                }

                stream << endl;

                totalSize = constFCIndex->size();
                i = 0;

                CT_AbstractFaceCloudIndex::ConstIterator beginF = constFCIndex->constBegin();
                CT_AbstractFaceCloudIndex::ConstIterator endF = constFCIndex->constEnd();

                while(beginF != endF)
                {
                    const CT_Face &face = beginF.cT();

                    stream << "f " << hashTablePoint.value(face.iPointAt(0)) << " " << hashTablePoint.value(face.iPointAt(1)) << " " << hashTablePoint.value(face.iPointAt(2)) << endl;

                    ++beginF;
                    ++i;

                    setExportProgress(((50+((i*50)/totalSize))+nExported)/totalToExport);
                }
            }

            nExported += 100;
        }

        file.close();
        return true;
    }

    return false;
}
