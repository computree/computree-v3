#include "pb_ascidexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"
#include "ct_tools/ct_numerictostringconversiont.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"


PB_ASCIDExporter::PB_ASCIDExporter() : CT_AbstractExporterPointAttributesSelection()
{
}

PB_ASCIDExporter::~PB_ASCIDExporter()
{

}

QString PB_ASCIDExporter::getExporterCustomName() const
{
    return "Points + ID item, ASCII(X,Y,Z)";
}

void PB_ASCIDExporter::init()
{
    addNewExportFormat(FileFormat("asc", tr("Fichier asc")));
}

bool PB_ASCIDExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if (dynamic_cast<CT_AbstractItemDrawableWithPointCloud*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_AbstractItemDrawableWithPointCloud"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

CT_AbstractExporter* PB_ASCIDExporter::copy() const
{
    return new PB_ASCIDExporter();
}

bool PB_ASCIDExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "asc";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "ID\tX\tY\tZ\n";

        int totalToExport = itemDrawableToExport().size();
        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();
            size_t id = item->id();

            CT_PointIterator itP(dynamic_cast<CT_AbstractItemDrawableWithPointCloud*>(item)->getPointCloudIndex());

            size_t totalSize = itP.size();
            size_t i = 0;

            while(itP.hasNext())
            {
                const CT_Point &point = itP.next().currentPoint();

                txtStream << id << "\t";
                txtStream << CT_NumericToStringConversionT<double>::toString(point(0)) << "\t";
                txtStream << CT_NumericToStringConversionT<double>::toString(point(1)) << "\t";
                txtStream << CT_NumericToStringConversionT<double>::toString(point(2)) << "\n";

                ++i;

                setExportProgress((((i*100)/totalSize)+nExported)/totalToExport);
            }

            nExported += 100;
        }

        file.close();
        return true;
    }

    return false;
}
