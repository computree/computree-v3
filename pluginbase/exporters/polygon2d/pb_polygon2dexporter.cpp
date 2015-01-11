#include "pb_polygon2dexporter.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_Polygon2DExporter::PB_Polygon2DExporter() : CT_AbstractExporter()
{
    setCanExportItems(true);
    setCanExportPoints(false);
}

PB_Polygon2DExporter::~PB_Polygon2DExporter()
{

}

QString PB_Polygon2DExporter::getExporterCustomName() const
{
    return "Polygones 2D, ACSII";
}

void PB_Polygon2DExporter::init()
{
    addNewExportFormat(FileFormat("txt", tr("Polygones 2D (ASCII)")));
}

bool PB_Polygon2DExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);
    int nProfiles = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_Polygon2D*>(item) != NULL)
        {
            myList.append(item);
            ++nProfiles;
        }
    }

    if(nProfiles == 0)
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_Polygon2D"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_Polygon2DExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_Polygon2DExporter::copy() const
{
    return new PB_Polygon2DExporter();
}

bool PB_Polygon2DExporter::protectedExportToFile()
{
    bool ok = true;

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "txt";

    QString filePath = QString("%1/%2.%3").arg(path).arg(baseName).arg(suffix);
    QFile file(filePath);

    if(file.open(QFile::WriteOnly))
    {
        QTextStream stream(&file);

        // write header
        stream << "# 2D Polygons export from Computree v3\n";
        stream << "# Format:\n";
        stream << "PolygonId\tAreaOfPolygon\tXcenter\tYcenter\tNumberOfVertice(N)\tXvertice1\tYvertice1\tXvertice2\tYvertice2\t...\tXverticeN\tYverticeN\n";

        CT_AbstractCoordinateSystem::realIm x, y, z;
        CT_AbstractCoordinateSystem::realEx xc, yc, zc;

        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
        while (it.hasNext())
        {
            CT_Polygon2D* item = dynamic_cast<CT_Polygon2D*>(it.next());
            item->computeCentroid();

            x = item->getCenterX();
            y = item->getCenterY();
            z = item->getCenterZ();

            PS_COORDINATES_SYS->convertExport(x, y, z, xc, yc, zc);

            stream << item->id() << "\t";
            stream << item->getArea() << "\t";
            stream << (double)xc << "\t";
            stream << (double)yc << "\t";
            stream << item->getNumberOfVertices();

            if (item->getNumberOfVertices() > 0)
            {
                const QVector<Eigen::Vector2d*> &vertices = item->getVertices();
                for (int i = 0 ; i < vertices.size() ; i++)
                {
                    Eigen::Vector2d* vertice = vertices.at(i);
                    x = (*vertice)(0);
                    y = (*vertice)(1);
                    z = item->getCenterZ();

                    PS_COORDINATES_SYS->convertExport(x, y, z, xc, yc, zc);

                    stream << "\t";
                    stream << (double)xc << "\t";
                    stream << (double)yc;
                }
            }
            stream << "\n";
        }
        file.close();
    } else {ok = false;}

    return ok;
}
