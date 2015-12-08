#include "pb_grid2dexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractimage2d.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_Grid2DExporter::PB_Grid2DExporter() : CT_AbstractExporter()
{
}

PB_Grid2DExporter::~PB_Grid2DExporter()
{

}

QString PB_Grid2DExporter::getExporterCustomName() const
{
    return tr("Grilles 2D, ACSII ESRI Grid");
}

void PB_Grid2DExporter::init()
{
    addNewExportFormat(FileFormat("ASC", tr("Fichiers Grid 2D (ASCII)")));
}

bool PB_Grid2DExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);
    int nGrids = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_AbstractImage2D*>(item) != NULL)
        {
            myList.append(item);
            ++nGrids;
        }
    }

    if(nGrids == 0)
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_AbstractImage2D"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_Grid2DExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_Grid2DExporter::copy() const
{
    return new PB_Grid2DExporter();
}

bool PB_Grid2DExporter::protectedExportToFile()
{
    bool ok = true;

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "asc";

    QString indice = "";
    if (itemDrawableToExport().size() > 1) {indice = "_0";}
    int cpt = 0;

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
    while (it.hasNext())
    {
        CT_AbstractImage2D* item = dynamic_cast<CT_AbstractImage2D*>(it.next());
        if (item != NULL)
        {
            QString filePath = QString("%1/%2%3.%4").arg(path).arg(baseName).arg(indice).arg(suffix);

            QFile file(filePath);

            if(file.open(QFile::WriteOnly))
            {
                QTextStream stream(&file);

                // write header
                size_t xdim = item->colDim();
                size_t ydim = item->linDim();

                stream << "ncols\t" << xdim << "\n";
                stream << "nrows\t" << ydim << "\n";

                stream << "xllcorner\t" << item->minX() << "\n";
                stream << "yllcorner\t" << item->minY() << "\n";

                stream << "cellsize\t" << item->resolution() << "\n";
                stream << "NODATA_value\t" << item->NAAsString() << "\n";

                // write data
                for (size_t yy = 0 ; yy < ydim ; yy++)
                {
                    for (size_t xx = 0 ; xx < xdim ; xx++)
                    {
                        size_t sIndex;
                        item->index(xx, yy, sIndex);
                        stream << item->valueAtIndexAsString(sIndex);

                        if (xx < xdim - 1) {stream << "\t";}
                    }
                    stream << "\n";
                }

                file.close();
            } else {ok = false;}
            indice = QString("_%1").arg(++cpt);
        }
    }

    return ok;
}
