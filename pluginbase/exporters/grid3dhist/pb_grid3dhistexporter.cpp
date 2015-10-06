#include "pb_grid3dhistexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_Grid3DHistExporter::PB_Grid3DHistExporter() : CT_AbstractExporter()
{
}

PB_Grid3DHistExporter::~PB_Grid3DHistExporter()
{

}

QString PB_Grid3DHistExporter::getExporterCustomName() const
{
    return "Grid3D Hist, ACSII";
}

void PB_Grid3DHistExporter::init()
{
    addNewExportFormat(FileFormat("hist", tr("Fichiers hist Grid3D (ASCII)")));
}

bool PB_Grid3DHistExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);
    int nProfiles = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_AbstractGrid3D*>(item) != NULL)
        {
            myList.append(item);
            ++nProfiles;
        }
    }

    if(nProfiles == 0)
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_AbstractGrid3D"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_Grid3DHistExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_Grid3DHistExporter::copy() const
{
    return new PB_Grid3DHistExporter();
}

bool PB_Grid3DHistExporter::protectedExportToFile()
{
    bool ok = true;

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "hist";

    QString indice = "";
    if (itemDrawableToExport().size() > 1) {indice = "_0";}
    int cpt = 0;

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
    while (it.hasNext())
    {
        CT_AbstractGrid3D* item = dynamic_cast<CT_AbstractGrid3D*>(it.next());


        QMap<double, size_t> hist;
        for (size_t index = 0 ; index < item->nCells() ; index++)
        {
            double value = item->valueAtIndexAsDouble(index);

            size_t count = hist.value(value, 0);
            hist.insert(value, ++count);
        }

        QString filePath = QString("%1/%2%3.%4").arg(path).arg(baseName).arg(indice).arg(suffix);

        QFile file(filePath);

        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);

            // write header

            stream << "Xorigin\t" << item->minX() << "\n";
            stream << "Yorigin\t" << item->minY() << "\n";
            stream << "Zorigin\t" << item->minZ() << "\n";

            stream << "DimX\t" << item->xdim() << "\n";
            stream << "DimY\t" << item->ydim() << "\n";
            stream << "DimZ\t" << item->zdim() << "\n";

            stream << "Resolution\t" << item->resolution() << "\n";
            stream << "NODATA_value\t" << item->NAAsString() << "\n";

            stream << "Value\tCount\n";

            // write data
            QMapIterator<double, size_t> it(hist);

            while (it.hasNext())
            {
                it.next();
                stream << it.key() << "\t";
                stream << it.value() << "\n";

            }

           file.close();
        } else {ok = false;}
        indice = QString("_%1").arg(++cpt);
    }

    return ok;
}
