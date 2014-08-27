#include "pb_profileexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractprofile.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_ProfileExporter::PB_ProfileExporter() : CT_AbstractExporter()
{
}

PB_ProfileExporter::~PB_ProfileExporter()
{

}

QString PB_ProfileExporter::getExporterCustomName() const
{
    return "Profiles, ACSII";
}

void PB_ProfileExporter::init()
{
    addNewExportFormat(FileFormat("hist", tr("Fichiers Profile (ASCII)")));
}

bool PB_ProfileExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);
    int nProfiles = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_AbstractProfile*>(item) != NULL)
        {
            myList.append(item);
            ++nProfiles;
        }
    }

    if(nProfiles == 0)
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_AbstractProfile"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_ProfileExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_ProfileExporter::copy() const
{
    return new PB_ProfileExporter();
}

bool PB_ProfileExporter::protectedExportToFile()
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
        CT_AbstractProfile* item = dynamic_cast<CT_AbstractProfile*>(it.next());

        QString filePath = QString("%1/%2%3.%4").arg(path).arg(baseName).arg(indice).arg(suffix);

        QFile file(filePath);

        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);

            // write header
            size_t dim = item->nCells();

            stream << "Nlevels\t" << dim << "\n";

            stream << "Xorigin\t" << item->minX() << "\n";
            stream << "Yorigin\t" << item->minY() << "\n";
            stream << "Zorigin\t" << item->minZ() << "\n";

            const QVector3D &direction = item->getDirection();

            stream << "Xdirection\t" << direction.x() << "\n";
            stream << "Ydirection\t" << direction.y() << "\n";
            stream << "Zdirection\t" << direction.z() << "\n";

            stream << "Resolution\t" << item->resolution() << "\n";
            stream << "NODATA_value\t" << item->NAAsString() << "\n";

            stream << "Level\tLength\tXc\tYc\tZc\tValue\n";

            // write data
            for (size_t index = 0 ; index < dim ; index++)
            {
                QVector3D cellCenter;
                item->getCellCenterXYZ(index, cellCenter);

                stream << index << "\t";
                stream << item->lengthForIndex(index) << "\t";
                stream << cellCenter.x() << "\t";
                stream << cellCenter.y() << "\t";
                stream << cellCenter.z() << "\t";
                stream << item->valueAtIndexAsString(index) << "\n";
            }

            file.close();
        } else {ok = false;}
        indice = QString("_%1").arg(++cpt);
    }

    return ok;
}
