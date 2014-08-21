#include "pb_ascrgbexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_ASCRGBExporter::PB_ASCRGBExporter() : CT_AbstractExporterPointAttributesSelection()
{
}

PB_ASCRGBExporter::~PB_ASCRGBExporter()
{

}

QString PB_ASCRGBExporter::getExporterCustomName() const
{
    return "Ascii (X,Y,Z,R,G,B)";
}

void PB_ASCRGBExporter::init()
{
    addNewExportFormat(FileFormat("asc", tr("Fichier asc")));
}

bool PB_ASCRGBExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_IAccessPointCloud*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_IAccessPointCloud"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

CT_AbstractExporter* PB_ASCRGBExporter::copy() const
{
    return new PB_ASCRGBExporter();
}

bool PB_ASCRGBExporter::protectedExportToFile()
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

        txtStream << "X Y Z R G B\n";

        CT_AbstractColorCloud *cc = createColorCloudBeforeExportToFile();

        float r = 1;
        float g = 1;
        float b = 1;

        int totalToExport = itemDrawableToExport().size();
        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            const CT_AbstractPointCloudIndex *constPCIndex = ((CT_IAccessPointCloud*)item)->getPointCloudIndex();

            size_t totalSize = constPCIndex->size();
            size_t i = 0;

            CT_AbstractPointCloudIndex::ConstIterator begin = constPCIndex->constBegin();
            CT_AbstractPointCloudIndex::ConstIterator end = constPCIndex->constEnd();

            while(begin != end)
            {
                const CT_Point &point = begin.cT();

                txtStream << point.data[0] << " ";
                txtStream << point.data[1] << " ";
                txtStream << point.data[2] << " ";

                if(cc == NULL)
                {
                    txtStream << "0 0 0";
                }
                else
                {
                    const CT_Color &co = cc->constColorAt(begin.cIndex());
                    r = (quint16)co.r / 255.0;
                    g = (quint16)co.g / 255.0;
                    b = (quint16)co.b / 255.0;

                    txtStream << r << " ";
                    txtStream << g << " ";
                    txtStream << b << "\n";
                }

                ++begin;
                ++i;

                setExportProgress((((i*100)/totalSize)+nExported)/totalToExport);
            }

            nExported += 100;
        }

        clearWorker();

        file.close();
        return true;
    }

    clearWorker();

    return false;
}
