#include "pb_pbmexporter.h"

#include "ct_itemdrawable/abstract/ct_virtualgrid2d.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_PbmExporter::PB_PbmExporter() : CT_AbstractExporter()
{
}

PB_PbmExporter::~PB_PbmExporter()
{
}

QString PB_PbmExporter::getExporterCustomName() const
{
    return "Grilles 2D, format PBM";
}

void PB_PbmExporter::init()
{
    addNewExportFormat(FileFormat("pbm", tr("Fichiers Images 2D (pbm)")));
}

bool PB_PbmExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext()
            && myList.isEmpty())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_ITemplatedData2DArray<int>*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_ITemplatedData2DArray<int>"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_PbmExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        setErrorMessage(tr("Erreur"));
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_PbmExporter::copy() const
{
    return new PB_PbmExporter();
}

bool PB_PbmExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "pbm";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

    if( itemDrawableToExport().size() > 1 )
    {
        setErrorMessage(tr("Impossible d'enregistrer plusieurs images dans le meme fichier"));
        return false;
    }

    if(file.open(QFile::WriteOnly))
    {
        QTextStream stream(&file);

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            CT_ITemplatedData2DArray<int> *grid = dynamic_cast<CT_ITemplatedData2DArray<int>*>(item);
            int width = grid->xArraySize();
            int height = grid->yArraySize();

            size_t totalSize = width*height;
            size_t i = 0;

            if(grid != NULL)
            {
                stream << "P1\n";
                stream << "# Binary image from Computree software\n";
                stream << width << " " << height << "\n";

                // write data
                for (int y = 0 ; y < height ; y++ )
                {
                    for (int x = 0 ; x < width ; x++ )
                    {
                        stream << ( grid->dataFromArray(x, height - 1 - y ) != 0 ) << " ";
                    }

                    if( y != height - 1 )
                    {
                        stream << "\n";
                    }
                }

                ++i;
                setExportProgress( i*100.0/totalSize );
            }
        }

        file.close();
        return true;
    }

    return false;
}
